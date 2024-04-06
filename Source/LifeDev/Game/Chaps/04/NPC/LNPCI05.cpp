// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LNPCI05.h"

ALNPCI05::ALNPCI05():Super() {
	Texts = { FText::FromString("...") };

	SetEnabled(false); // try to default to false. but the step will fade in and re-enable it :G
	Locked = true;
}
