// Copyright Jerónimo Barraco-Mármol

#include "PaperI00.h"

#include "LifeDev/Game/Sys/Consts/ConstItems.h"

APaperI00::APaperI00():Super() {
	// just for looks
	// RewardFlash = -.2f;
	// RewardItem = LDConsts::Items::Poem3;
	// TriggerDlg = "P03_Look*";

	Super::SetMobility(EComponentMobility::Static);

	// TODO add text get it from the diags
}
