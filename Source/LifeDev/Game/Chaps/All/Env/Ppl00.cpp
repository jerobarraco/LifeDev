// Copyright (C) 2023-2025 Jeronimo Barraco-Marmol

#include "Ppl00.h"

#include "Components/CapsuleComponent.h"

#include "Eval.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorFade.h"
#include "CQuickMesh.h"

#include "CPplSig.h"

APpl00::APpl00() {
	UseFade = true; // read begin play
	UseAutoActivate = true;
	Sig = CreateDefaultSubobject<UCPplSig>(TEXT("Sig"));

	static ConstructorHelpers::FObjectFinder<UForceFeedbackEffect>
		CFFe(TEXT("/Game/LifeDev/Game/Env/Ppl/Ppl_FFE"));
	RumbleLook = CFFe.Object;

	// TODO on sfx class set the loading stuff

	// this sound is intentionally stereo since it plays with the panning
	// and i think it's suitable
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSFX(TEXT("/Game/LifeDev/Game/Env/Ppl/UIGlitch_26"));
	SFXLook = CSFX.Object;

	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		CHeadMat(TEXT("/Game/LifeDev/Game/Env/Ppl/PplHead_MI"));
	AnimFade->MatBase = CHeadMat.Object;
	Mesh->SetEvaluateWorldPositionOffset(true);
	Head->SetEvaluateWorldPositionOffset(true);
	// todo there's a tiny hitch the first time the first ppl is set to not hidden.
	// unfortunately putting this into beginplay does not fixes it

	// Sig->CompsHide.AddUnique(Root);

	Collider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collider"));
	Collider->SetupAttachment(Interact);
	Collider->SetGenerateOverlapEvents(false);
	Collider->bUseAttachParentBound = true;
	Collider->CanCharacterStepUpOn = ECB_No;
	// Collider->SetRelativeLocation(FVector(0,5,0));
	// Collider->SetCapsuleRadius(100);
	Collider->SetCapsuleSize(150, 150);
	// Collider->bComputeBoundsOnceForGame = true;
	// Collider->bComputedBoundsOnceForGame = false;
	// SetBoxExtent(FVector(50,50,110));
	Collider->bComputeFastLocalBounds = true;

	SetActorHiddenInGame(true); // depends on the box
}

void APpl00::BeginPlay() {
	Super::BeginPlay();

	UseFade = false; // disable fade here, so that the (SetActive) significance doesn't use it.
	Sig->SetSignificance(ESigValue::Off);
	// but so we can use it later.
	Sig->OnChanged.AddUniqueDynamic(this, &APpl00::SigChanged);
	Sig->SetActive(true, true);
	UseFade = true; // restore for the sigchanged
}

void APpl00::SetActorHiddenInGame(const bool NewHidden) {
	Super::SetActorHiddenInGame(NewHidden);
	Collider->SetCollisionProfileName(NewHidden ? "NoCollision" : "InvisibleWall");
}

void APpl00::Look_Implementation() {
	Super::Look_Implementation();
	SetActive(false);
}

void APpl00::SigChanged(const ESigValue Significance, const ESigValue SignificanceOld) {
	UE_LOG(LogTemp, Log, TEXT("%hs o=%s v=%s"),
		__func__, *Label.ToString(), *UEnum::GetValueAsString(Significance));
	
	// only happens when the player is far away. if it's high (close) discard
	if (Significance > ESigValue::Med) return;

	const UEval* const Eval = UEval::Instance(this);
	double Res = 0;
	const bool Ok = LIKELY(Eval) && Eval->Eval(ShowCondition, Res, true);
	if (!Ok | (Res <= 0)) {
		UE_LOG(LogTemp, Log, TEXT("%hs o=%s Condition unsuccessful: '%s'."),
			__func__, *Label.ToString(), *ShowCondition);
		return;
	}

	// once it's shown, deactivate.
	Sig->Deactivate();
	// UseFade = true;
	SetActorHiddenInGame(false);
}
