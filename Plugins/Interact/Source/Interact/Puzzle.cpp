// Copyright Jerónimo Barraco-Mármol

#include "Puzzle.h"

#include "Interact.h"
#include "Components/BillboardComponent.h"
#include "Interact/CPuzzle.h"

APuzzle::APuzzle():Super() {
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	CPuzzle = CreateDefaultSubobject<UCPuzzle>(TEXT("CPuzzle"));
	CPuzzle->DisableOnDone = true;

#if WITH_EDITORONLY_DATA // inspired by AInfo
	UBillboardComponent* const SpriteComponent = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("Sprite"));
	if (!IsRunningCommandlet() && (SpriteComponent != nullptr)) {
		// static ConstructorHelpers::FObjectFinderOptional<UTexture2D> SpriteTexture(TEXT("/Engine/EditorResources/S_Actor"));
		// static ConstructorHelpers::FObjectFinder<UTexture2D> CSprText(TEXT("/Engine/EditorResources/S_Actor"));
		static ConstructorHelpers::FObjectFinderOptional<UTexture2D>
			CSprTexture(TEXT("/Engine/EditorResources/S_Actor")); // S_Solver
		static const FName ID_Info = TEXT("Puzzle");
		static const FText ID_Name = NSLOCTEXT("SpriteCategory", "Puzzle", "Puzzle");

		SpriteComponent->SetupAttachment(Root);
		SpriteComponent->Sprite = CSprTexture.Get();
		// SpriteComponent->Sprite = CSprText.Object;
		SpriteComponent->SpriteInfo.Category = ID_Info;
		SpriteComponent->SpriteInfo.DisplayName = ID_Name;
		SpriteComponent->bIsScreenSizeScaled = true;
	}
	// setting spatially loaded to false could break datalayer usage which is critical
#endif // WITH_EDITORONLY_DATA

	Root->SetMobility(EComponentMobility::Static);
}

void APuzzle::Reset_Implementation() {
	if (CPuzzle) CPuzzle->Reset();
}

void APuzzle::Done_Implementation(bool IsOk) {
	UE_LOG(LogTemp, Log, TEXT("APuzzle::Done ok=%i o=%s"), IsOk, *GetNameSafe(this));
	if (!IsOk) return;

	if (IsValid(DoneInter)) {
		DoneInter->Locked = false;// force unlock
		DoneInter->TryTrigger();
	}
	
	if (IsValid(DoneActor)) {
		DoneActor->SetActorHiddenInGame(false);
		AInteract* const Reward = Cast<AInteract>(DoneActor);
		if (IsValid(Reward)) {
			Reward->SetEnabled(true);
		}
	}
}

void APuzzle::BeginPlay() {
	Super::BeginPlay();

	CPuzzle->OnDone.AddUniqueDynamic(this, &APuzzle::Done);
	CPuzzle->OnUpdate.AddUniqueDynamic(this, &APuzzle::Update);
	CPuzzle->OnReset.AddUniqueDynamic(this, &APuzzle::DoReset);
	
	if (IsValid(DoneActor)) {
		DoneActor->SetActorHiddenInGame(true);
		AInteract* const Reward = Cast<AInteract>(DoneActor);
		if (IsValid(Reward)) {
			Reward->SetEnabled(false);
		}
	}
}

void APuzzle::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	if (CPuzzle) {
		CPuzzle->OnDone.RemoveAll(this);
		CPuzzle->OnUpdate.RemoveAll(this);
		CPuzzle->OnReset.RemoveAll(this);
	}
}
