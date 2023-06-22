// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LNPC01.h"

ALNPC01::ALNPC01():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CMesh(TEXT("/Game/LifeDev/Game/Chaps/All/Chars/Char18/Char18.Char18"));
	Mesh->SetStaticMesh(CMesh.Object);
}
