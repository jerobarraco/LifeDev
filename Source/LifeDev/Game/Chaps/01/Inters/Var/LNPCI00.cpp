#include "LNPCI00.h"

ALNPCI00::ALNPCI00() {
	UseFade = true;
	IsLocked = true;
	Texts = { NSLOCTEXT("NPC01", "State0", "He seems dangerous ...") };
	SetPoseSit();
}
