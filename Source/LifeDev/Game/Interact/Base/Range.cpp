// Copyright Jerónimo Barraco-Mármol

#include "Range.h"

#include "Components/SphereComponent.h"
#include "Materials/MaterialParameterCollection.h"

#include "Interact/Animator/Anim.h"
#include "Interact/Animator/CAnimatorMix.h"

#include "JUtils/Actors/CQuickMesh.h"


#include "LifeDev/Game/Interact/LInteract.h"
// this requires to create an object channel "Range"
// and edit the collision channel Interact to "overlap" range
// then add another collision profile for OverlapInteract that overlaps Interacts and has type Range.

static const FName HintMPCName = FName("Hint");

ARange::ARange():Super() {
	// super important or it will NOT work
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	Super::SetActorTickEnabled(false); // ensure we don't animate on start

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	Collider->SetupAttachment(Root);
	Collider->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	Collider->SetCollisionProfileName("Hint");
	Collider->CanCharacterStepUpOn = ECB_No;
	Collider->InitSphereRadius(52); // 50 is ok but with 60 i make sure it's bigger than the sphere
	
	Mesh = CreateDefaultSubobject<UCQuickMesh>(TEXT("Mesh"));
	Mesh->SetupAttachment(Collider);
	// Mesh->SetRelativeScale3D(FVector(UE_SMALL_NUMBER));
	Mesh->SetQuickCollisionEnabled(false);
	Mesh->bReceiveMobileCSMShadows = false;
	Mesh->SetAffectDistanceFieldLighting(false);//needed since the material uses distance field
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		ObjMesh(TEXT("/Engine/EditorMeshes/AssetViewer/Sphere_inversenormals"));
	if (LIKELY(ObjMesh.Succeeded()))
		Mesh->SetStaticMesh(ObjMesh.Object);

	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		ObjMat(TEXT("/Game/LifeDev/Game/Inters/Cards/Card00Outline_MI.Card00Outline_MI"));
	if (LIKELY(ObjMat.Succeeded()))
		Mesh->SetMaterial(0, ObjMat.Object);

	Anim = CreateDefaultSubobject<UCAnimatorMix>(TEXT("Anim"));
	Anim->UseSweep = true;
	Anim->IsAdditive = false;
	Anim->Duration = 2;
	Anim->TStart.SetScale3D(FVector(1)); // 1 would avoid flashing the player, but doesn't touch items that are close. (makes no sense)
	Anim->TEnd.SetScale3D(FVector(DefScaleMax)); // note that the serialized value is not available here. so this is just a fallback.
	Anim->TRoot = Collider; // using the collider since sweep only happens for this component
	Anim->SetComponentTickInterval(0); // full fps this moves very fast

	Anim->MatFName = TEXT("Opacity");
	Anim->MatFStart = 1;
	Anim->MatFEnd = 0;

	static ConstructorHelpers::FObjectFinder<UMaterialParameterCollection>
		CMPC(TEXT("/Game/LifeDev/Game/Flashback/Flashback_MPC.Flashback_MPC"));
	MPC = CMPC.Object;

	SetActorHiddenInGame(true);
}

void ARange::OverlapBegin(UPrimitiveComponent* const Cmp, AActor* const OtherActor,
	UPrimitiveComponent* const OtherComp, const int32 OtherBodyIndex,
	const bool bFromSweep, const FHitResult& SweepResult) {

	ALInteract* const Inter = Cast<ALInteract>(OtherActor);
	if (!Inter) return;

	UE_LOG(LogTemp, Verbose, TEXT("ARange::%hs hit o=%s"), __func__, *Inter->Label.ToString());
	Inter->ShowHint();
}

void ARange::BeginPlay() {
	Super::BeginPlay();

	if (UNLIKELY(!Anim | !Collider)) return;

	Anim->Mat = Mesh->CreateDynamicMaterialInstance(0);
	Anim->CodeCurve.Clear();
	Anim->Curve = nullptr;
	Anim->OnEnd.AddUniqueDynamic(this, &ARange::AnimEnd);

	// Anim->OnUpdate.AddUniqueDynamic(this, &ARange::AnimUpd); // nopes
	Collider->OnComponentBeginOverlap.AddUniqueDynamic(this, &ARange::OverlapBegin);

	// UCodeCurveLib* const Lib = UCodeCurveLib::Instance();
	// Anim->CodeCurve.BindDynamic(Lib, &UCodeCurveLib::InSin);
}

void ARange::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	if (LIKELY(Anim)) Anim->OnEnd.RemoveAll(this);
	if (LIKELY(Collider)) Collider->OnComponentBeginOverlap.RemoveAll(this);

	const UWorld* const World = GetWorld();
	if (World) {
		FTimerManager& Timer = World->GetTimerManager();
		Timer.ClearAllTimersForObject(this); // this does not fix it
		Timer.ClearTimer(HRange); // this might. TODO retest.
		HRange.Invalidate();
	}
	Super::EndPlay(EndPlayReason);
}

void ARange::AnimEnd() {
	UE_LOG(LogTemp, Log, TEXT("ARange::AnimEnd"));
	// disable. so that it won't trigger hint on interacts that just become non-hidden/visible
	SetActorHiddenInGame(true);
	Collider->Deactivate();
	Collider->SetGenerateOverlapEvents(false); // actually this is the one that fixes it. the rest are nice to haves.
}

void ARange::Trigger() {
	SetActorHiddenInGame(false);
	// force the scale so that we don't have false positives
	Collider->SetWorldScale3D(Anim->TStart.GetScale3D());
	Collider->Activate(true); // doesn't really fix it. but it's nice anyway. i guess. until it breaks something.
	Collider->SetGenerateOverlapEvents(true); // this is the important fix. 
	// while this is the logical spot of the anim (at the end) it won't set the scale until next tick.
	Anim->Activate(true); // force the animation to restart so that it triggers again.

	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;
	
	UAnim* const AnimMat = UAnim::Instance(this);
	if (LIKELY(AnimMat)) {
		const FAParams P {.Name = HintMPCName, .Duration = .25};
		AnimMat->MPCFloatFade(MPC, P, 1);
	}

	FTimerDelegate D;
	constexpr float OutTime = .5;
	D.BindLambda([AnimMat, OutTime, this] () {
		// avoid crashes if someone finished the game during a range >_<
		// even though i clear the timer on EndPlay, that does NOT fix it!
		if (UNLIKELY(!AnimMat | !IsValid(AnimMat) | !IsValid(this))) return;
		
		const FAParams POut {.Name = HintMPCName, .Duration = OutTime};
		AnimMat->MPCFloatFade(MPC, POut, 0);
	});
	const AInteract* const Int = GetMutableDefault<AInteract>(); // changing the hinttime on the settings breaks this, :(((
	const float Rate = LIKELY(Int) ?
		FMath::Max(.01, Int->HintTime - OutTime) :  // Just before the Interact hides the hint. 0 won't trigger :(
		Anim->Duration; // or just the duration in case interact fails.

	FTimerManager& Timer = World->GetTimerManager();
	Timer.ClearTimer(HRange);
	Timer.SetTimer(HRange, D, Rate, false);
}

void ARange::SetScaleMax(const float Scale) const {
	if (UNLIKELY(!Anim)) return;

	Anim->TEnd.SetScale3D(FVector(Scale));
}
