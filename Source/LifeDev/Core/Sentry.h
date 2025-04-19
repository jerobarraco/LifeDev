#pragma once
#include "Sentry.generated.h"

class USentrySubsystem;

UCLASS(Blueprintable)
class LIFEDEV_API USentry: public UGameInstanceSubsystem {
	GENERATED_BODY()
public:
	static USentry* Instance(const UObject* const O);;

	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	void SendComment(const FString& FB) const;

	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	void TagSet(const FName& Tag, const FString& Val) const;
	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	void TagRem(const FName& Tag) const;
	
protected:
	void InstInit();
	void InstDeInit();
	void GameInit();
	void GameDeInit();

	UPROPERTY(Transient)
	TObjectPtr<USentrySubsystem> Sub = nullptr;

	friend class ALGGameMode;
	friend class ULGameInstance;
};

