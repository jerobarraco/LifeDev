#pragma once

#include "SentryDataTypes.h"

#include "Sentry.generated.h"

enum class EFeat : uint8;
struct FDiag;
class UCInteract;
class AStep;
class USentrySubsystem;

UCLASS(Blueprintable)
class LIFEDEV_API USentry: public UGameInstanceSubsystem {
	GENERATED_BODY()
public:
	static USentry* Instance(const UObject* const O);

	UFUNCTION(BlueprintCallable, BlueprintPure=false, meta=(WorldContext="O", AutoCreateRefTerm="Msg,Level"))
	static void SAddMsg(const UObject* const O, const FString& Msg, const ESentryLevel& Level = ESentryLevel::Info);
	// triggers a feedback. (this creates an instance)
	UFUNCTION(BlueprintCallable, BlueprintPure=false, meta=(AutoCreateRefTerm="Comment"))
	void AddComment(const FString& Comment) const;
	// triggers a message. (this creates an instance)
	UFUNCTION(BlueprintCallable, BlueprintPure=false,meta=(AutoCreateRefTerm="Msg,Level"))
	void AddMsg(const FString& Msg, const ESentryLevel& Level = ESentryLevel::Info) const;
	// a tag is a state. does not trigger an instance. useful to track things when tracking a bug.
	// also avoid using unless necessary.
	UFUNCTION(BlueprintCallable, BlueprintPure=false, meta=(AutoCreateRefTerm="Tag,Val"))
	void TagSet(const FString& Tag, const FString& Val) const;
	UFUNCTION(BlueprintCallable, BlueprintPure=false, meta=(AutoCreateRefTerm="Tag"))
	void TagRem(const FString& Tag) const;
	// aka breadcrumb, something that happened between instances. does not create an instance.
	// this is mostly necessary when you know what you want to debug. for a SPECIFIC bug you're tracking.
	// don't add unnecessarily, specially since the types are VERY heavy.
	UFUNCTION(BlueprintCallable, BlueprintPure=false,meta=(AutoCreateRefTerm="Hint,Cat,Type,Data"))
	void AddHint(const FString& Hint, const TMap<FString, FString>& Data,
		const FString& Cat = "Default", const FString& Type ="Default") const;

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
	void DiagAdd(const FName& Name, const FDiag& Diag);
	UFUNCTION()
	void DiagDone();
	UFUNCTION()
	void InterTrigger(const UCInteract* const Comp);

	UPROPERTY(Transient)
	TObjectPtr<USentrySubsystem> Sub = nullptr;

	friend class ALGGameMode;
	friend class ULGameInstance;
};

