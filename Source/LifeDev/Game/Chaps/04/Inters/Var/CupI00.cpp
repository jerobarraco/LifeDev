#include "CupI00.h"

#include "CQuickMesh.h"

ACupI00::ACupI00() {
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		ObjMat(TEXT("/Game/LifeDev/Game/Var/Mats/Voxel/Palettes/Palette01_DMI"));
	if(LIKELY(ObjMat.Succeeded())) Mesh->SetMaterial(0, ObjMat.Object);
}
