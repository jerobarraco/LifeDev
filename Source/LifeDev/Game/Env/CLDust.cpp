#include "CLDust.h"

UCLDust::UCLDust() {
	bAutoManageAttachment = true;
	SetAutoActivate(false);

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>
		DefaultMapping(TEXT("/Game/LifeDev/Game/Env/Ambience/Dust"));
	SetAsset(DefaultMapping.Object);
	// Mapping = DefaultMapping.Object;
}
