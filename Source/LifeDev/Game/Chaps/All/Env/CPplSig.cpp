// Copyright (C) 2023-2025 Jeronimo Barraco-Marmol


#include "CPplSig.h"
// note, this is using the significance system in a weird way. so read this.
// it leverages the sig system for polling this component async.
// it becomes significant once you pass a couple of conditions
// It starts insignificant
UCPplSig::UCPplSig() {
	SetAutoActivate(true);
	IsOffIfHidden = false; // we are actually
	TestOcclusion = true;
}
