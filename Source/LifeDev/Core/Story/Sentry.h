#pragma once


#include "Sentry.generated.h"
UCLASS(Blueprintable)
class LIFEDEV_API USentry: public UGameInstanceSubsystem {
public:

protected:
	void InstInit(); // todo
	void InstDeInit() {}; // todo
	void GameInit() {};//todo
	void GameDeInit() {}; //todo
	
	friend class ULGGameMode;
	friend class ULGameInstance;
};
