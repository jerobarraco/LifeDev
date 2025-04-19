#pragma once

#include "SentryDataTypes.h"

#include "Sentry.generated.h"

struct FDiag;
class AStep;
enum class EFeat : uint8;
class USentrySubsystem;

static TMap<FString, FString> EmptyData;
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
	UFUNCTION(BlueprintCallable, BlueprintPure=false,
		meta=(AutoCreateRefTerm="Hint,Cat,Type,Data"))
	void AddHint(const FString& Hint,
		const TMap<FString, FString>& Data,
		const FString& Cat = "Default", const FString& Type ="Default") const;
	UFUNCTION(BlueprintCallable, BlueprintPure=false,
		meta=(AutoCreateRefTerm="Msg, Default"))
	void AddMsg(const FString& Msg, const ESentryLevel& Level = ESentryLevel::Info) const;

protected:
	void InstInit();
	void InstDeInit();

	
	void GameInit();
	void GameDeInit();
	UFUNCTION()
	void FeatUp(const EFeat Feat, const bool Enabled);
	UFUNCTION()
	void Saving(const bool IsSaving);
	UFUNCTION()
	void StepStart(AStep* const Step);
	UFUNCTION()
	void StepStop(AStep* const Step);
	UFUNCTION()
	void DiagAdd(const FName Name, const FDiag& Diag);
	UFUNCTION()
	void DiagDone();

	UPROPERTY(Transient)
	TObjectPtr<USentrySubsystem> Sub = nullptr;

	friend class ALGGameMode;
	friend class ULGameInstance;
};

