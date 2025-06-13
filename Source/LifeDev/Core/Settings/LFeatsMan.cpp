// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LFeatsMan.h"

#include "EnhancedInputComponent.h"
#include "Engine/PostProcessVolume.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "Kismet/GameplayStatics.h"
#include "WorldPartition/DataLayer/DataLayerAsset.h" // needed to load testdl
#include "InputMappingContext.h" // for the action object. rider doesn't detect it

#include "Interact/CInteract.h"
#include "Interact/CInteractor.h"
#include "Interact/Interact.h"
#include "Inventory/Flags.h"
#include "Inventory/Inventory.h"
#include "JUtils/Misc/JUtilsMisc.h"
#include "JUtils/Misc/JUtilsSys.h"
#include "Story/Story.h"
#include "Eval.h"

#include "LifeDev/Game/Char/LChar.h"
#include "LifeDev/Game/Flashback/Flashback.h"
#include "LifeDev/Game/Sys/LGGameMode.h"
#include "UI/LOverlayUI.h"
#include "LSettings.h"
#include "LSettingsUI.h"
#include "Diags/Diags.h"
#include "LifeDev/Core/Sentry.h"

DEFINE_LOG_CATEGORY_STATIC(LogLFeatsMan, Log, Log);

ALFeatsMan::ALFeatsMan() {
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	static ConstructorHelpers::FObjectFinder<UMaterialParameterCollection>
		CMPC(TEXT("/Game/LifeDev/Game/Flashback/Flashback_MPC"));
	MPC = CMPC.Object;

	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		CSpeedMat(TEXT("/Game/LifeDev/Game/Flashback/FlashbackVel_MI"));
	SpeedMat = CSpeedMat.Object;
	
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		CFBMat(TEXT("/Game/LifeDev/Game/Flashback/Flashback_MI"));
	FBMat = CFBMat.Object;

	static ConstructorHelpers::FClassFinder<ULOverlayUI>
		COver(TEXT("/Game/LifeDev/Core/Settings/Feats/LOverlayUI_W.LOverlayUI_W_C"));
	OverlayUIClass = COver.Succeeded() ? COver.Class.Get() : ULOverlayUI::StaticClass();
	static ConstructorHelpers::FClassFinder<UUserWidget>
		CSettingsUI(TEXT("/Game/LifeDev/Core/Settings/SettingsUI_W"));
	SettingsUIClass = CSettingsUI.Succeeded() ? CSettingsUI.Class.Get() : ULSettingsUI::StaticClass();
	
	static ConstructorHelpers::FObjectFinder<UInputAction>
		CActionMenu(TEXT("/Game/LifeDev/Core/Settings/Input/Menu_IA"));
	ActionMenu = CActionMenu.Object.Get();

	static ConstructorHelpers::FObjectFinder<UInputMappingContext>
	DefaultMapping(TEXT("/Game/LifeDev/Core/Settings/Input/Menu_IMC"));
	Mapping = DefaultMapping.Object;
}

ALFeatsMan* ALFeatsMan::Instance(const UObject* const O) {
	if (UNLIKELY(!IsValid(O))) return nullptr;

	const UWorld* const World = O->GetWorld();
	if (UNLIKELY(!IsValid(World))) return nullptr;

	const ALGGameMode* const Mode = Cast<ALGGameMode>(World->GetAuthGameMode());
	if (LIKELY(IsValid(Mode)))
		return Mode->FeatsMan;

	return Cast<ALFeatsMan>(UGameplayStatics::GetActorOfClass(World, ALFeatsMan::StaticClass()));
}

void ALFeatsMan::LoadMPC() {
	const UWorld* const W = GetWorld();
	if (UNLIKELY(!W)) return;
	
	if (UNLIKELY(!IsValid(MPC))) {
		UE_LOG(LogLFeatsMan, Warning, TEXT("%hs Could not get the MPC. Skip"),
				__func__);
		return;
	}
	
	MPCI = W->GetParameterCollectionInstance(MPC);
	UE_CLOG(UNLIKELY(!IsValid(MPCI)), LogLFeatsMan, Warning,
		TEXT("%hs Could not get the MPCInst."), __func__);
}

void ALFeatsMan::BeginPlay() {
	Super::BeginPlay();

	UWorld* const W = GetWorld(); // don't make it const or CreateWidget will fail compilation
	if (UNLIKELY(!W)) return;

	AGameModeBase* const AGMB = W->GetAuthGameMode();
	GM = Cast<ALGGameMode>(AGMB);
	Eval = W->GetSubsystem<UEval>();
	Settings = ULSettings::Instance(this);
	// force initialize
	LoadMPC();
	
	UClass* const Class = OverlayUIClass.Get();
	if (LIKELY(IsValid(Class))) {
		OverlayUI = CreateWidget<ULOverlayUI>(W, Class);
		if (LIKELY(IsValid(OverlayUI))) {
			OverlayUI->AddToViewport(ZOrder);
			// OverlayUI->OnDone.AddUniqueDynamic(this, &ADiagMan::Hidden);
		}
	}

	UClass* const SClass = SettingsUIClass.Get();
	if (LIKELY(IsValid(SClass))) {
		SettingsUI = CreateWidget<ULSettingsUI>(W, SClass);
		if (LIKELY(SettingsUI)) {
			// important to add to the viewport otherwise the GC will delete our bindings :')
			SettingsUI->AddToViewport(9999);
			SettingsUI->OnDone.AddUniqueDynamic(this, &ALFeatsMan::MenuDone);
			MenuDone(); // hide
		}
	}
}

void ALFeatsMan::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	UJUtilsSys::ToggleMapping(this, Mapping, -1, false);
	UEnhancedInputComponent* const Input = UJUtilsSys::GetEInput(this);
	if (LIKELY(Input)) Input->ClearBindingsForObject(this);


	if (LIKELY(Settings)) {
		Settings->OnFeatUpdate.RemoveAll(this);
		Settings->OnFeatUpdateVisual.RemoveAll(this);
		Settings->OnFeatUpdateUnreal.RemoveAll(this);
		Settings->OnFeatUpdateDebug.RemoveAll(this);
	}

	if (LIKELY(Eval)) {
		Eval->OnGetVar.Clear();
		Eval->OnSetVar.Clear();
		Eval->OnSetVarId.Clear();
	}

	if (LIKELY(IsValid(SettingsUI))) {
		SettingsUI->OnDone.RemoveAll(this);
		SettingsUI->RemoveFromParent();
	}

	if (LIKELY(OverlayUI)) OverlayUI->RemoveFromParent();
	SettingsUI = nullptr;
	OverlayUI = nullptr;
	GM = nullptr;
	MPCI = nullptr;
	MPC = nullptr;
	SpeedMat = nullptr; // free as a bird
	FBMat = nullptr;
	Eval = nullptr;
	Settings = nullptr;

	Super::EndPlay(EndPlayReason);
}

void ALFeatsMan::Init() {
	if (LIKELY(Settings)) {
		Settings->OnFeatUpdateVisual.AddUniqueDynamic(this, &ALFeatsMan::FeatUpVisual);
		Settings->OnFeatUpdateUnreal.AddUniqueDynamic(this, &ALFeatsMan::FeatUpUnreal);
		Settings->OnFeatUpdateDebug.AddUniqueDynamic(this, &ALFeatsMan::FeatUpDbg);
	}

	if (LIKELY(Eval)) {
		Eval->OnGetVar.BindDynamic(this, &ALFeatsMan::GetVar);
		Eval->OnSetVar.BindDynamic(this, &ALFeatsMan::SetVar);
		Eval->OnSetVarId.BindDynamic(this, &ALFeatsMan::SetVarId);
	}

	LoadFeats();

	if (LIKELY(OverlayUI)) OverlayUI->Show();

	UEnhancedInputComponent* const Input = UJUtilsSys::GetEInput(this);
	if (LIKELY(Input))
		Input->BindAction(ActionMenu, ETriggerEvent::Triggered, this, &ALFeatsMan::ActMenu);
	UJUtilsSys::ToggleMapping(this, Mapping, InputPrio, true);
}

void ALFeatsMan::ActMenu() { // no const
	UE_LOG(LogTemp, Warning, TEXT("%hs"), __func__);
	if (UNLIKELY(!IsValid(SettingsUI))) return;

	if (SettingsUI->IsVisible()) { // toggle
		MenuDone();
		return;
	}

	SettingsUI->Show();
}

void ALFeatsMan::MenuDone() {
	if (UNLIKELY(!IsValid(SettingsUI))) return;
	SettingsUI->Hide();
}

#pragma region feats
void ALFeatsMan::LoadFeats() {
	FeatUpVisual(EFeat::V_LUMEN, Settings && Settings->GetFeat(EFeat::V_LUMEN));
	FeatUpVisual(EFeat::V_SPEED, Settings && Settings->GetFeat(EFeat::V_SPEED));
	FeatUpVisual(EFeat::V_STROBE, Settings && Settings->GetFeat(EFeat::V_STROBE));
	FeatUpVisual(EFeat::V_FLASHBACK, Settings && Settings->GetFeat(EFeat::V_FLASHBACK));
	FeatUpVisual(EFeat::V_AUTO_EXP, Settings && Settings->GetFeat(EFeat::V_AUTO_EXP));
	BlurReset();
	
	FeatUpUnreal(EFeat::U_TICK_BATCH, Settings && Settings->GetFeat(EFeat::U_TICK_BATCH));
	FeatUpUnreal(EFeat::U_TICK_CON, Settings && Settings->GetFeat(EFeat::U_TICK_CON));
	FeatUpDbg(EFeat::DBG_TESTDL, Settings && Settings->GetFeat(EFeat::DBG_TESTDL));
	FeatUpDbg(EFeat::G_STATUS, Settings && Settings->GetFeat(EFeat::G_STATUS));
}

void ALFeatsMan::FeatUpVisual(const EFeat Feat, const bool Enabled) {
	if (UNLIKELY(!IsValid(GM) || UNLIKELY(!IsValid(GM->PostProcess)))) {
		UE_LOG(LogLFeatsMan, Warning, TEXT("%hs Could not find the post process or game mode!"), __func__);
		const USentry* const Sentry = USentry::Instance(this);
		if (LIKELY(Sentry)) Sentry->AddMsg("Could not find the post process or game mode.", ESentryLevel::Warning);
		return;
	}

	// Important:
	// these properties on the editor have a checkbox next to them.
	// i DO need to check them for the engine to pay attention to them,
	// otherwise the changes here make no difference.
	// https://forums.unrealengine.com/t/how-can-i-control-post-processing-volume-settings-using-c/465187/2?u=nande
	// disable lumen on runtime https://forums.unrealengine.com/t/is-there-a-way-to-add-an-option-to-enable-disable-lumen-for-in-game-settings/613756

	// if i were to have an array of pointer i could get rid of all these branches
	APostProcessVolume* const Post = GM->PostProcess;
	if (Feat == EFeat::V_LUMEN) {
		// needed to allow the flag to override project settings
		Post->Settings.bOverride_DynamicGlobalIlluminationMethod = true;
		Post->Settings.bOverride_ReflectionMethod = true;
		Post->Settings.DynamicGlobalIlluminationMethod =
			Enabled ?
			EDynamicGlobalIlluminationMethod::Lumen : EDynamicGlobalIlluminationMethod::None;
		Post->Settings.ReflectionMethod =
			Enabled ? EReflectionMethod::Lumen : EReflectionMethod::None;
	} else if (Feat == EFeat::V_MLIGHTS) {
		// needed to allow the flag to override project settings
		Post->Settings.bOverride_bMegaLights = true;
		Post->Settings.bMegaLights = Enabled;
	} else if (Feat == EFeat::V_BLUR) {
		Post->Settings.MotionBlurAmount = Enabled ? MotionBlurAmount: 0;
		Post->Settings.MotionBlurMax = Enabled ? MotionBlurMax: 0;
		Post->Settings.SceneFringeIntensity = Enabled ? FringeIntensity: 0;
	} else if (Feat == EFeat::V_AUTO_EXP) {
		// https://forums.unrealengine.com/t/how-do-i-disable-eye-adaptation-auto-exposure/286811/6
		// TODO fix the need to have an autoexposure of -6 instead of 0 apparently should be like... -2
		// https://forums.unrealengine.com/t/lighting-exposure-and-intensity-units-confusion/211375/2?u=nande
		Post->Settings.AutoExposureMinBrightness = Enabled ? -2.5 : -2;
		Post->Settings.AutoExposureMaxBrightness = Enabled ? 0 : -2;
		Post->Settings.bOverride_AutoExposureMinBrightness = true; //!Enabled;
		Post->Settings.bOverride_AutoExposureMaxBrightness = true;
		// don't set to manual method ot it will be full black
	} else if (Feat == EFeat::V_FLASHBACK) {
		if (UNLIKELY(!FBMat)) return;
		if (Enabled)
			Post->Settings.AddBlendable(FBMat, 1);
		else
			Post->Settings.RemoveBlendable(FBMat);
	} else if (Feat == EFeat::V_NANITE) {
		UE_LOG(LogLFeatsMan, Log, TEXT("%hs Nanite=%i"), __func__, Enabled);
		IConsoleVariable* const Variable =
			IConsoleManager::Get().FindConsoleVariable(TEXT("r.Nanite"));
		if (UNLIKELY(!Variable)) {
			UE_LOG(LogLFeatsMan, Warning, TEXT("%hs Can't find r.Nanite var. Stop"), __func__);
			return;
		}
		Variable->Set(Enabled?1:0);
	} else {
		if (UNLIKELY(!MPCI)) return; // on purpose like this, to not make a mistake myself.
		const float v = Enabled ? 1: 0;
		if (Feat == EFeat::V_STROBE) {
			static const FName N("Strobe");
			MPCI->SetScalarParameterValue(N, v);
		}
		else if (Feat == EFeat::V_SPEED) {
			static const FName N("Speed");
			MPCI->SetScalarParameterValue(N, v);
			if (UNLIKELY(!SpeedMat)) return;
			if (Enabled)
				Post->Settings.AddBlendable(SpeedMat, 1);
			else
				Post->Settings.RemoveBlendable(SpeedMat);
		}
	}

	// } else if (Feat == EFeat::V_DOF) {
	// // cant do this. it will break the fb post process since it does a blur pass before the fb
	// and can't move the fb to before dof pass without breaking everything.
	// Post->Settings.DepthOfFieldScale = bEnabled ? 1:0;
}

void ALFeatsMan::FeatUpUnreal(const EFeat Feat, const bool Enabled) {
	if (Feat == EFeat::U_TICK_BATCH) {
		IConsoleVariable* const CVar =
			IConsoleManager::Get().FindConsoleVariable(TEXT("tick.AllowBatchedTicks"));
		if (LIKELY(CVar)) CVar->Set(Enabled ? 1 : 0, EConsoleVariableFlags::ECVF_SetByCode);
	} else if (Feat == EFeat::U_TICK_CON) {
		IConsoleVariable* const CVar =
			IConsoleManager::Get().FindConsoleVariable(TEXT("tick.AllowConcurrentTickQueue"));
		if (LIKELY(CVar)) CVar->Set(Enabled ? 1 : 0, EConsoleVariableFlags::ECVF_SetByCode);
	}
}

void ALFeatsMan::FeatUpDbg(const EFeat Feat, const bool Enabled) {
	if (Feat == EFeat::DBG_TESTDL) {
		UJUtilsMisc::ToggleDataLayer(this, TestDL.LoadSynchronous(), Enabled);
	} else if (Feat == EFeat::DBG_D_WARN) {
		UDiags* const Diags = UDiags::Instance(this);
		if (LIKELY(Diags)) Diags->UseWarning = Enabled;
	}
}

void ALFeatsMan::BlurReset() {
	FeatUpVisual(EFeat::V_BLUR, Settings && Settings->GetFeat(EFeat::V_BLUR));
}

#pragma endregion

#pragma region Eval
double ALFeatsMan::GetVar(const FName& Name) {
	// important to keep this function optimized since a single Diags->AddId could trigger a bunch of evals, which could trigger an N*M flags get
	// so it's important to cache the FNames, otherwise we'll incur in string parsing every time!

	// unfortunately this needs to access _everything_. good thing we can access the gm here that has most of the stuff.
	// but still it will put a load on this class (the includes at least)
	const FString NameS = Name.ToString();
	UE_LOG(LogLFeatsMan, Log, TEXT("%hs Name=%s"), __func__, *NameS);
	#define I64ToD(X) *reinterpret_cast<const double*>(&X)

	if (NameS.StartsWith("#")) {
		const FName Actual = FName(NameS.RightChop(1)); // remove the @
		// FMemory::Memcpy(&Test, &Actual, sizeof(uint64));
		const uint64 Int = Actual.ToUnstableInt();
		// https://stackoverflow.com/a/61661893/260242
		// have to actually return a double or loose precision. int uses 64 bit. double uses 57.
		// const double Val = *reinterpret_cast<const double*>(&Int);
		const double Val = I64ToD(Int);
		// FMemory::Memcpy(&Val, &Int, sizeof(uint64));
		UE_LOG(LogLFeatsMan, Log, TEXT("%hs Fname Val Name=%s Val=%lf"), __func__, *NameS, Val);
		return Val;
	}

	if (!NameS.StartsWith("V.")) {
		if (UNLIKELY(!GM->Flags)) return -1;

		const float Val = GM->Flags->Get(Name, -1);// default to -1 to help me detect issues while i get used to this.
		UE_LOG(LogLFeatsMan, Log,
			TEXT("%hs: Flag: Name=%s Val=%.5f"), __func__, *Name.ToString(), Val);
		UE_CLOG(UNLIKELY(!GM->Flags->IsSet(Name)), LogLFeatsMan, Warning,
			TEXT("%hs Flag is not found. Name=%s"), __func__, *Name.ToString());
		return Val;
	}

	// checking against names first, intentionally for performance
	static const FName NAME_FBVal("V.FB.Val");
	static const FName NAME_FBValTo("V.FB.ValTo");
	static const FName NAME_StoryStepCur("V.Story.Step.Cur");
	static const FName NAME_SysDebug("V.Sys.IsDebug");
	static const FName NAME_SysEditor("V.Sys.IsEditor");
	if (Name == NAME_FBVal)
		return LIKELY(GM->Flashback) ? GM->Flashback->GetVal() : -1;
	if (Name == NAME_FBValTo)
		return LIKELY(GM->Flashback) ? GM->Flashback->GetValTo() : -1;
	if (Name == NAME_StoryStepCur) {
		const uint64 I = LIKELY(GM->Story) ? GM->Story->GetCurrent().ToUnstableInt() : -1;
		return I64ToD(I);
	}
	if (Name == NAME_SysDebug)
		return UJUtilsSys::IsDebug() ? 1:0;
	if (Name == NAME_SysEditor)
		return UJUtilsSys::IsEditor() ? 1:0;

	// Maybe .Cur.Name
	static const FName NAME_ItemCur("V.Item.Cur");
	if (Name == NAME_ItemCur) {
		if (UNLIKELY(!GM->Inventory)) return -1;

		const uint64 I = GM->Inventory->GetSelected().ToUnstableInt();
		return I64ToD(I);
	}

	// maybe .cur.count
	/// parsing

	static const TCHAR* const TItemCount = TEXT("V.Item.Count.");
	if (NameS.StartsWith(TItemCount)) {
		if (UNLIKELY(!GM->Inventory)) return -1;

		const size_t L = UJUtilsMisc::TextLen(TItemCount);
		const FName N(NameS.RightChop(L));
		return GM->Inventory->Count(N);
	}

	static const TCHAR* const TFeatGet = TEXT("V.Feat.Get.");
	if (NameS.StartsWith(TFeatGet)) {
		if (UNLIKELY(!Settings)) return -1;

		static const size_t L = UJUtilsMisc::TextLen(TFeatGet);
		const FString& FeatS = NameS.RightChop(L);
		for (const EFeat F: TEnumRange<EFeat>()) {
			const FString& CurFeatS = UEnum::GetValueAsString(F);
			// UE_LOG(LogLGameMode, Log, TEXT("%hs search feat tgt=%s cur=%s"), __func__, *FeatS, *CurFeatS);
			// const bool Same = CurFeatS.Equals(FeatS, ESearchCase::IgnoreCase);
			// EndsWith is a cheat. i know. otherwise i need to use the full name like EFEAT::D_AUTO.
			const bool Same = CurFeatS.EndsWith(FeatS);
			if (LIKELY(!Same)) continue;

			return Settings->GetFeat(F) ? 1: 0; 
		}
	}

	if (NameS.StartsWith("V.Inter.Cur")) { // this is a hack
		// TODO find better way
		if (UNLIKELY(!GM->Char)) return -1;

		const UCInteractor* const Int = Cast<UCInteractor>(GM->Char->GetComponentByClass(UCInteractor::StaticClass()));
		if (UNLIKELY(!Int)) return -1;

		const UCInteract* const Comp = Int->GetHoverComp();
		if (UNLIKELY(!Comp)) return -1;

		static const FName NCurName("V.Inter.Cur.Name");
		if (Name == NCurName) {
			const AActor* const CmpOwner = Comp->GetOwner(); // this is a different owner than this->GetOwner. Owner is a private var also, will break windoz.
			if (UNLIKELY(!CmpOwner)) return -1;

			const FName OwnerName = Settings->GetObjectLabel(CmpOwner);
			UE_LOG(LogLFeatsMan, Log, TEXT("%hs v.inter.cur.name Name=%s i=%i"), __func__, *OwnerName.ToString(), OwnerName.ToUnstableInt());
			const uint64 I = OwnerName.ToUnstableInt();
			return I64ToD(I);
		}

		static const FName NCurState("V.Inter.Cur.State");
		if (LIKELY(Name == NCurState)) { // likely since it's the last.
			const AInteract* const CmpOwner = Cast<AInteract>(Comp->GetOwner());
			if (UNLIKELY(!CmpOwner)) return -1;

			return CmpOwner->GetState();
		}
	}

	static const TCHAR* const TInterState = TEXT("V.Inter.State.");
	if (NameS.StartsWith(TInterState)) {
		const size_t L = UJUtilsMisc::TextLen(TInterState);
		const FString& ActorName = NameS.RightChop(L);
		UE_LOG(LogLFeatsMan, Log, TEXT("%hs Inter State for=%s"), __func__, *ActorName);
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(this, AInteract::StaticClass(), Actors);
		for (const AActor* const A: Actors) {
			if (UNLIKELY(!A)) continue;

			const FName Label = Settings->GetObjectLabel(A);
			const bool Same = Label.ToString().Equals(ActorName, ESearchCase::IgnoreCase);
			if (LIKELY(!Same)) continue;

			const AInteract* const I = Cast<AInteract>(A);
			return LIKELY(I) ? I->GetState(): -1;
		}

		return -1; // not found
	}

	return -1;
}

void ALFeatsMan::SetVar(const FString& Name, const double Val) {
	const FString& Trimmed = Name.TrimStartAndEnd();
	UE_LOG(LogLFeatsMan, Warning, TEXT("%hs SetVar Name=%s Val=%.5f"),
		__func__, *Name, Val);
	
	if (UNLIKELY(Trimmed.IsEmpty())) return;
	const FName N(Trimmed);

	if (UNLIKELY(!GM || !GM->Flags)) return;
	GM->Flags->Set(N, Val);
}

void ALFeatsMan::SetVarId(const double NameID, const double Val) {
	FName Name;

	// this will work. can be stored. and compared. the problem is that calling to .ToString will always return "None"
	FMemory::Memcpy(&Name, &NameID, sizeof(double));
	UE_LOG(LogLFeatsMan, Warning, TEXT("%hs SetVarId Id=%lf Val=%lf Name=%s"),
		__func__, NameID, Val, *Name.ToString());
	UE_LOG(LogLFeatsMan, Warning, TEXT("%hs same as c3s0=%i"),
		__func__, Name==FName("C3S0"));
	if (LIKELY(GM->Flags)) GM->Flags->Set(Name, Val);
	return;
/*
	// TODO make this work
	struct T {
		uint32 ID;
		uint32 Number;
	};
	T TId;
	FMemory::Memcpy(&TId, &NameID, sizeof(uint64));

	FNameEntryId Id;
	Id.FromUnstableInt(TId.ID);
	
	FName N(Id, Id, TId.Number);//(Id, Id, 0);S
	// FName N2 = FName::CreateFromDisplayId(Id, TId.Number); // neither works
	// // this is reversing what fromunstableid does. of course, it does not work.
	// FMemory::Memcpy(&N, &NameID, sizeof(uint64));
	// notice how abhorrently hacky this is. i love it.
	UE_LOG(LogLFeatsMan, Warning, TEXT("%hs SetVar Id=%lli Val=%lf EId=%i Nid=%i Name=%s"),
		__func__, NameID, Val, TId.ID, TId.Number, *N.ToString());
	if (N == FName("C3S0"))
		UE_LOG(LogLFeatsMan, Warning, TEXT("%hs same as c3s0"),
		__func__, NameID, Val, TId.ID, TId.Number, *N.ToString());

	if (UNLIKELY(N.IsNone())) return;
	if (LIKELY(GM->Flags)) GM->Flags->Set(N, Val); */
}

#pragma endregion
