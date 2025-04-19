#pragma once
#include "Sentry.generated.h"

enum class EFeat : uint8;
class USentrySubsystem;

UCLASS(Blueprintable)
class LIFEDEV_API USentry: public UGameInstanceSubsystem {
	GENERATED_BODY()
public:
	static USentry* Instance(const UObject* const O);;

	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	void SendComment(const FString& FB) const;

	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	void TagSet(const FString& Tag, const FString& Val) const;
	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	void TagRem(const FString& Tag) const;
	
protected:
	void InstInit();
	void InstDeInit();
	void GameInit();
	void GameDeInit();
	UFUNCTION()
	void FeatUp(const EFeat Feat, const bool Enabled);

	UPROPERTY(Transient)
	TObjectPtr<USentrySubsystem> Sub = nullptr;

	friend class ALGGameMode;
	friend class ULGameInstance;
};

