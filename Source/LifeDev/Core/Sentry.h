#pragma once
#include "Sentry.generated.h"

class USentrySubsystem;

UCLASS(Blueprintable)
class LIFEDEV_API USentry: public UGameInstanceSubsystem {
	GENERATED_BODY()
public:
	static USentry* Instance(const UObject* const O);;

protected:
	void InstInit();
	void InstDeInit();
	void GameInit();
	void GameDeInit(); //todo

	UPROPERTY(Transient)
	TObjectPtr<USentrySubsystem> Sub = nullptr;

	friend class ALGGameMode;
	friend class ULGameInstance;
};

