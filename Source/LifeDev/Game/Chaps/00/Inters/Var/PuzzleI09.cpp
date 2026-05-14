// Copyright Jerónimo Barraco-Mármol

#include "PuzzleI09.h"

#include "Interact/CPuzzle.h"
#include "JUtils/Misc/JMiscConsts.h"
#include "Story/Step.h"
#include "Story/Story.h"

APuzzleI09::APuzzleI09():Super() {
	CPuzzle->Type = EPuzzleType::SEQUENCE;
	CPuzzle->Solution = {0,0,0};
	IsOneShot = true;
	// UseActiveOnce = true; // i think this might make this disabled when it's been saved
	ResetOnFail = true;
	// idea: change the solution depending on the chapter.
	// will also require to reset when progressing.
	CPuzzle->SetPieces({
		SoftOP(AInteract, "/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.LSwitch03_UAID_D8BBC116E501CB5A02_1700292265"),
		SoftOP(AInteract, "/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.LSwitch03_UAID_D8BBC116E501805A02_1588908064"),
		SoftOP(AInteract, "/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.LSwitch03_UAID_D8BBC116E501815A02_1457917247"),
		SoftOP(AInteract, "/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.LSwitch03_UAID_D8BBC116E501815A02_1460358248"),
		SoftOP(AInteract, "/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.LSwitch03_UAID_D8BBC116E501815A02_1470043249"),
		SoftOP(AInteract, "/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.LSwitch03_UAID_D8BBC116E501815A02_1478320250"),
		SoftOP(AInteract, "/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.LSwitch03_UAID_D8BBC116E501815A02_1482305251"),
		SoftOP(AInteract, "/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.LSwitch03_UAID_D8BBC116E501815A02_1484441252"),
		SoftOP(AInteract, "/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.LSwitch03_UAID_D8BBC116E501815A02_1489810253"),
		SoftOP(AInteract, "/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.LSwitch03_UAID_D8BBC116E501815A02_1491909254"),
		SoftOP(AInteract, "/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.LSwitch03_UAID_D8BBC116E501CB5A02_1700290264"),
		SoftOP(AInteract, "/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.LSwitch03_UAID_D8BBC116E501CB5A02_1700285263")});
}

void APuzzleI09::BeginPlay() {
	Super::BeginPlay();
	if (UNLIKELY(!Story)) return;
	Story->OnStart.AddUniqueDynamic(this, &APuzzleI09::StartStep);
	
	/// apply these here since the cpuzzle pieces might not be loaded in the constructor.
	
	// SetAutoActives(false); // this one breaks it // todo call this on postload or somth
	SetActives(false); // doesn't really work
	StartStep(Story->GetStep(Story->GetCurrent())); // necessary. the step is already started on beginplay. since the step loads the dl, and this is on a dl.
}

// attempt to make this interaction chapter agnostic
// this is kind of a test. TODO move this class to Game/CHaps/All/Inters/Var/Phone if i decide to keep it like this.
// TODo enable with the story step instead of this
// todo it's better to have multiple phones than having this

void APuzzleI09::StartStep(AStep* const Step) {
	if (UNLIKELY(!Step)) return;

	// only important if i want to trigger on different chapters.
	if (Step->Label == "C0S0") {
		Reset();
		SetActives(true);
		CPuzzle->Solution = {9,1,1};
	}
}
