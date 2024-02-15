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

	Root->SetMobility(EComponentMobility::Static);

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
}

void APuzzle::Done_Implementation(bool IsOk) {
	UE_LOG(LogTemp, Log, TEXT("APuzzle::Done ok=%i o=%s"), IsOk, *GetNameSafe(this));
	if (!IsOk || !IsValid(DoneInter)) return;
	// force unlock
	DoneInter->Locked = false;
	DoneInter->TryTrigger();
}

void APuzzle::BeginPlay() {
	Super::BeginPlay();

	CPuzzle->OnDone.AddUniqueDynamic(this, &APuzzle::Done);
	CPuzzle->OnUpdate.AddUniqueDynamic(this, &APuzzle::Update);
}

void APuzzle::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	if (CPuzzle) {
		CPuzzle->OnDone.RemoveAll(this);
		CPuzzle->OnUpdate.RemoveAll(this);
	}
}

void APuzzle::SetStates(const TArray<int32>& States) {
	if (!CPuzzle) return;
	
	const TArray<AInteract*> Inters = CPuzzle->GetInteracts();
	const int32 Num = States.Num();
	const int32 Num2 = Inters.Num();
	for (int32 i = 0; i<Num && i<Num2; ++i) {
		AInteract* const I = Inters[i];
		if (!IsValid(I)) continue;
		I->SetState(States[i]);
	}

	// reload the ids if needed. important.
	CPuzzle->ResetCurrents();
}

void APuzzle::SetLocks(const TArray<bool>& Locks) {
	if (!CPuzzle) return;

	const TArray<AInteract*> Inters = CPuzzle->GetInteracts();
	const int32 Num = Locks.Num();
	const int32 Num2 = Inters.Num();
	for (int32 i = 0; i<Num && i<Num2; ++i) {
		AInteract* const I = Inters[i];
		if (!IsValid(I)) continue;
		I->Locked = Locks[i];
	}
}

void APuzzle::SetEnableds(bool NewEnabled) {
	if (!CPuzzle) return;

	const TArray<AInteract*> Inters = CPuzzle->GetInteracts();
	const int32 Num = Inters.Num();
	for (int32 i = 0; i<Num; ++i) {
		AInteract* const I = Inters[i];
		if (!IsValid(I)) continue;
		I->SetEnabled(NewEnabled);
	}
}
