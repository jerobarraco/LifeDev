// Copyright (C) 2023-2025 Jeronimo Barraco-Marmol

#include "Ppl00.h"

#include "Components/BoxComponent.h"

#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorFade.h"
#include "CQuickMesh.h"

#include "CPplSig.h"

APpl00::APpl00() {
	UseFade = true; // read begin play
	Sig = CreateDefaultSubobject<UCPplSig>(TEXT("Sig"));

	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		CHeadMat(TEXT("/Game/LifeDev/Game/Env/Ppl/PplHead_MI.PplHead_MI"));
	AnimFade->MatBase = CHeadMat.Object;
	Mesh->SetEvaluateWorldPositionOffset(true);
	Head->SetEvaluateWorldPositionOffset(true);
	// todo there's a tiny hitch the first time the first ppl is set to not hidden.
	// unfortunately putting this into beginplay does not fixes it

	// Sig->CompsHide.AddUnique(Root);

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(Interact);
	Box->SetGenerateOverlapEvents(false);
	Box->CanCharacterStepUpOn = ECB_No;
	Box->SetRelativeLocation(FVector(0,5,0));
	Box->SetBoxExtent(FVector(50,50,110));

	SetActorHiddenInGame(true); // depends on the box
}

void APpl00::BeginPlay() {
	Super::BeginPlay();

	UseFade = false; // disable fade here, so that the significance doesn't use it.
	Sig->SetSignificance(ESigValue::Off);
	// but so we can use it later.
	Sig->OnChanged.AddUniqueDynamic(this, &APpl00::SigChanged);
	Sig->SetActive(true, true);
	UseFade = true; // restore for the sigchanged
}

void APpl00::SetActorHiddenInGame(const bool NewHidden) {
	Super::SetActorHiddenInGame(NewHidden);
	Box->SetCollisionProfileName(NewHidden ? "NoCollision" :"BlockAllDynamic" );
}

void APpl00::SigChanged(const ESigValue Significance, const ESigValue SignificanceOld) {
	UE_LOG(LogTemp, Log, TEXT("%hs o=%s"), __func__, *GetNameSafe(GetOwner()));
	// once it's shown, deactivate.
	if (Significance == ESigValue::High) {
		Sig->Deactivate();
		// UseFade = true;
		SetActorHiddenInGame(false);
	}
}
