#include "Sentry.h"

#include "SentrySubsystem.h"

#include "Diags/Diags.h"
#include "Interact/CInteract.h"
#include "Interact/CInteractor.h"
#include "Interact/Interact.h"
#include "JUtils/Misc/JUtilsSys.h"
#include "Story/Step.h"
#include "Story/Story.h"

#include "LifeDev/Core/LGameInstance.h"
#include "LifeDev/Game/Char/LChar.h"
#include "LifeDev/Game/Sys/LGGameMode.h"
#include "Settings/LSettings.h"

DEFINE_LOG_CATEGORY_STATIC(LogSentry, Log, Log);

USentry* USentry::Instance(const UObject* const O) {
	const ULGameInstance* const Instance = ULGameInstance::Instance(O);
	if (UNLIKELY(!IsValid(Instance))) return nullptr;

	return Instance->GetSubsystem<USentry>();
}

void USentry::AddComment(const FString& Comment) const {
	if (UNLIKELY(!IsValid(Sub))) return;

	USentryId* const Id = Sub->CaptureMessage("FEEDBACK!"); // yes, the docs says it needs to be like this.
	if (UNLIKELY(!Id)) {
		UE_LOG(LogSentry, Error, TEXT("%hs Could not capture message. Id is null."), __func__);
		return;
	}

	Sub->CaptureUserFeedbackWithParams(Id, "", Comment, UJUtilsSys::GetUserName());
}

void USentry::TagSet(const FString& Tag, const FString& Val) const {
	if (UNLIKELY(!IsValid(Sub))) return;
	Sub->SetTag(Tag, Val);
}

void USentry::TagRem(const FString& Tag) const {
	if (UNLIKELY(!IsValid(Sub))) return;
	Sub->RemoveTag(Tag);
}

void USentry::AddHint(const FString& Hint, const TMap<FString, FString>& Data, const FString& Cat, const FString& Type) const {
	if (UNLIKELY(!IsValid(Sub))) return;
	Sub->AddBreadcrumbWithParams(Hint, Cat, Type, Data);
}

void USentry::AddMsg(const FString& Msg, const ESentryLevel& Level) const {
	if (UNLIKELY(!IsValid(Sub))) return;
	Sub->CaptureMessage(Msg, Level);
}

void USentry::InstInit() {
	Sub = GEngine->GetEngineSubsystem<USentrySubsystem>();
	UE_CLOG(!IsValid(Sub), LogSentry, Warning, TEXT("%hs Sentry subsystem can't be found"),
		__func__);
}

void USentry::InstDeInit() {
	Sub = nullptr;
}

void USentry::GameInit() {
	AddHint("GameInit", {});

	ULSettings* const Settings = ULSettings::Instance(this);
	if (LIKELY(Settings)) {
		Settings->OnFeatUpdate.AddUniqueDynamic(this, &USentry::FeatUp);
		Settings->OnSaving.AddUniqueDynamic(this, &USentry::Saving);
	}

	UStory* const Story = UStory::Instance(this);
	if (LIKELY(Story)) {
		Story->OnStart.AddUniqueDynamic(this, &USentry::StepStart);
		Story->OnStop.AddUniqueDynamic(this, &USentry::StepStop);
	}

	UDiags* const Diags = UDiags::Instance(this);
	if (LIKELY(Diags)) { // TODO careful this could slow the game
		Diags->OnAdd.AddUniqueDynamic(this, &USentry::DiagAdd);
		Diags->OnDone.AddUniqueDynamic(this, &USentry::DiagDone);
	}

	ALGGameMode* const GM = ALGGameMode::Instance(this);
	if (LIKELY(GM && GM->Char)) {
		UCInteractor* const Inter = Cast<UCInteractor>(GM->Char->GetComponentByClass(UCInteractor::StaticClass()));
		if (LIKELY(Inter)) Inter->OnTrigger.AddUniqueDynamic(this, &USentry::InterTrigger);
	}
}

void USentry::GameDeInit() {
	AddHint("GameDeInit", {});
	
	ULSettings* const Settings = ULSettings::Instance(this);
	if (LIKELY(Settings)) {
		Settings->OnFeatUpdate.RemoveAll(this);
		Settings->OnSaving.RemoveAll(this);
	}

	UStory* const Story = UStory::Instance(this);
	if (LIKELY(Story)) {
		Story->OnStart.RemoveAll(this);
		Story->OnStop.RemoveAll(this);
	}

	UDiags* const Diags = UDiags::Instance(this);
	if (LIKELY(Diags)) {
		Diags->OnAdd.RemoveAll(this);
		Diags->OnDone.RemoveAll(this);
	}
	
	if (LIKELY(Sub)) Sub->ClearBreadcrumbs();
	
	ALGGameMode* const GM = ALGGameMode::Instance(this);
	if (LIKELY(GM && GM->Char)) {
		UCInteractor* const Inter = Cast<UCInteractor>(GM->Char->GetComponentByClass(UCInteractor::StaticClass()));
		if (LIKELY(Inter)) Inter->OnTrigger.RemoveAll(this);
	}
}

void USentry::FeatUp(const EFeat Feat, const bool Enabled) {
	const FString& Tag = UEnum::GetValueAsString(Feat);
	if (Enabled)
		TagSet(Tag, "On");
	else
		TagRem(Tag);
}

void USentry::Saving(const bool IsSaving) {
	const FString& Tag("Saving");
	const FString& On(IsSaving?TEXT("1"):TEXT("0"));
	AddHint(Tag, {{TEXT("Saving"), On}});
}

static const FString TagNameStep("Story::Step");
void USentry::StepStart(AStep* const Step) {
	const FString& N = LIKELY(IsValid(Step)) ? Step->Name.ToString() : TEXT("");
	AddHint(TagNameStep, {{"Name", N}});
	TagSet(TagNameStep, N);
}

void USentry::StepStop(AStep* const Step) {
	if (LIKELY(IsValid(Step)))
		AddHint(TagNameStep, {{"Name", Step->Name.ToString()}});
	TagRem(TagNameStep);
}

void USentry::DiagAdd(const FName& Name, const FDiag& Diag) {
	static const FString Hint("Diag::Add");
	AddHint(Hint, {{"Name",Name.ToString()}});
}

void USentry::DiagDone() {
	static const FString Hint("Diag::Done");
	AddHint(Hint, {});
}

void USentry::InterTrigger(const UCInteract* const Comp) {
	static const FString Hint("Inter::Trigger");
	const AInteract* Inter = Cast<AInteract>(Comp->GetOwner());
	const FName& N = Inter ? Inter->Label : GetFNameSafe(Comp);
	AddHint(Hint, {{"Name", N.ToString() }} );
}
