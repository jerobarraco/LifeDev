#include "CLDust.h"

#include "NiagaraSystem.h"

UCLDust::UCLDust() {
	bAutoManageAttachment = true;
	SetAutoActivate(true); // TODO use the feats to toggle this. // todo test the feature flag

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>
		DefaultMapping(TEXT("/Game/LifeDev/Game/Env/Ambience/Dust"));
	SetAsset(DefaultMapping.Object);
	// Mapping = DefaultMapping.Object;
}
