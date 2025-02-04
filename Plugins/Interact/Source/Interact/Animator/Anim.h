#pragma once

#include "CoreMinimal.h"

#include "Anim.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAnimMatDone);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAnimMatDoneDyn, UMaterialInstanceDynamic* const, Mat, FName, Name);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAnimMatDoneMPC, UMaterialParameterCollectionInstance* const, Mat, FName, Name);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAnimMatDoneData, UPrimitiveComponent* const, Comp, int32, Index);

USTRUCT(Blueprintable, BlueprintType)
struct FAMBase {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Transient)
	FName Name = NAME_None;

	UPROPERTY(BlueprintReadWrite, Transient)
	TObjectPtr<UMaterialParameterCollectionInstance> MPCI = nullptr;

	UPROPERTY(BlueprintReadWrite, Transient, meta=(DeprecatedProperty))
	TObjectPtr<UMaterialInstanceDynamic> Mat = nullptr;
	// it's lazy to put the material here too. but it does simplify the initBase

	// wip polymorfic test
	UPROPERTY(BlueprintReadWrite, Transient)
	TObjectPtr<UObject> Obj = nullptr;
	
	UPROPERTY(BlueprintReadWrite, Transient)
	TObjectPtr<UCurveFloat> Curve = nullptr;
	UPROPERTY(BlueprintReadWrite, Transient)
	float Duration = 1.0;
	
	UPROPERTY(BlueprintReadWrite, Transient)
	float Elapsed = 1.0;

	FORCEINLINE bool IsDone() const {
		return FMath::IsNearlyEqual(Elapsed, Duration)
			|| Elapsed > Duration;
	}

	// needed or android won't package >_<! due to the virtual functions
	// has to be public.
	virtual ~FAMBase() = default;
	// named this way to avoid name collision with IsValid
	virtual bool FIsValid() const;
	// adds dt to the elapsed, returns current progress
	void AddDT(const float DT, float &Prog);
	// returns true on done
	bool Tick(const float DT);
	// set value using Lerp progress. override and call SetValue yourself.
	virtual bool SetLerp(const float Prog) {
		UE_LOG(LogTemp, Warning, TEXT("Empty Base SetLerp"));
		return false;
	}
};


USTRUCT(Blueprintable, BlueprintType)
struct FAMPFloat: public FAMBase {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Transient)
	float From = 0.0;

	UPROPERTY(BlueprintReadWrite, Transient)
	float To = 1.0;
	
	virtual bool SetVal(const float Val = 1.0) const;
	virtual bool SetLerp(const float Prog) override;
};

USTRUCT(Blueprintable, BlueprintType)
struct FAMDFloat: public FAMPFloat {
	GENERATED_BODY()

public:
	virtual bool SetVal(const float Val = 1.0) const override;
	virtual bool FIsValid() const override;
};

USTRUCT(Blueprintable, BlueprintType)
struct FAMPVector: public FAMBase {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Transient)
	FLinearColor From = FLinearColor::Black;

	UPROPERTY(BlueprintReadWrite, Transient)
	FLinearColor To = FLinearColor::White;

	// more expensive but nicer on colors
	UPROPERTY(BlueprintReadWrite, Transient)
	bool UseHSV = false;
	
	virtual bool SetVal(const FLinearColor& Val = FLinearColor::White) const;
	virtual bool SetLerp(const float Prog) override;
};

USTRUCT(Blueprintable, BlueprintType)
struct FAMDVector: public FAMPVector {
	GENERATED_BODY()

public:
	virtual bool SetVal(const FLinearColor& Val = FLinearColor::White) const override;
	virtual bool FIsValid() const override;
};

USTRUCT(Blueprintable, BlueprintType)
struct FAMData: public FAMBase {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Transient)
	TObjectPtr<UPrimitiveComponent> Comp = nullptr;

	UPROPERTY(BlueprintReadWrite, Transient)
	int32 Index = -1;
	// yikes, names only work on material instance Dynamic

	UPROPERTY(BlueprintReadWrite, Transient)
	bool IsScalar = true;
	
	// more expensive but nicer on colors
	UPROPERTY(BlueprintReadWrite, Transient)
	bool UseHSV = false;

	UPROPERTY(BlueprintReadWrite, Transient)
	FLinearColor From = FLinearColor::Black;

	UPROPERTY(BlueprintReadWrite, Transient)
	FLinearColor To = FLinearColor::White;

	bool GetCurrent(FLinearColor& OCurrent) const;
	bool SetVal(const FLinearColor& V = FLinearColor::White) const;
	virtual bool SetLerp(const float Prog) override;
	virtual bool FIsValid() const override { return IsValid(Comp) && Index >=0; }
};

// Subsystem that animates materials parameter collections' parameters.
// And custom primitive data. It's a bit deprecated. The AnimatorMPC and AnimatorData is preferred.
UCLASS(Blueprintable, Category="Interact", Config=Interact, DefaultConfig)
class INTERACT_API UAnim: public UTickableWorldSubsystem {
	GENERATED_BODY()

public:

#pragma region Base
	static UAnim* Instance(const UObject* const O);

	UAnim();
	virtual void Deinitialize() override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Tick(const float DT) override;
	virtual TStatId GetStatId() const override;
	virtual bool IsTickable() const override { return IsFading; }
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
#pragma endregion

#pragma region mpci
	// Fades in or out.
	// By design, it will replace any previous fades with the same name.
	// If it's fading it will continue from where it is, even if the direction changes.
	// Name: name of the parameter
	// To: value to fade to.
	// Duration: <0 uses the default, 0 is instant, >0 uses whatever specified.
	// Curve. easing curve. has to be in the range 0-1 for both axis. Y overshooting is fine.
	UFUNCTION(BlueprintCallable)
	bool MPCFloatFade(const UMaterialParameterCollection* const MPC,
		const FName Name, const float To = 1.0, const float Duration = -1,
		UCurveFloat* const Curve = nullptr);

	// Fades a vector (or color).
	// Name: the name of the parameter to fade
	// To: is the target vector/color
	// Duration: <0 uses the default, 0 is instant, >0 uses whatever specified.
	// UseHSV: uses HSV for lerp. is more expensive, but looks better on colors.
	// Curve. easing curve. has to be in the range 0-1 for both axis. Y overshooting is fine.
	UFUNCTION(BlueprintCallable)
	bool VectorFade(const UMaterialParameterCollection* const MPC,
		const FName Name, const FLinearColor& To = FLinearColor::White,
		const float Duration = -1, const bool UseHSV = false,
		UCurveFloat* const Curve = nullptr);
#pragma endregion

#pragma region dynmat
	// Fades in or out.
	// By design, it will replace any previous fades with the same name.
	// If it's fading it will continue from where it is, even if the direction changes.
	// Name: name of the parameter
	// To: value to fade to.
	// Duration: <0 uses the default, 0 is instant, >0 uses whatever specified.
	// Curve. easing curve. has to be in the range 0-1 for both axis. Y overshooting is fine.
	UFUNCTION(BlueprintCallable)
	bool DynFloatFade(UMaterialInstanceDynamic* const Mat,
		const FName Name, const float To = 1.0, const float Duration = -1,
		UCurveFloat* const Curve = nullptr);

	// Fades a vector (or color).
	// Name: the name of the parameter to fade
	// To: is the target vector/color
	// Duration: <0 uses the default, 0 is instant, >0 uses whatever specified.
	// UseHSV: uses HSV for lerp. is more expensive, but looks better on colors.
	// Curve. easing curve. has to be in the range 0-1 for both axis. Y overshooting is fine.
	UFUNCTION(BlueprintCallable)
	bool DynVectorFade(UMaterialInstanceDynamic* const Mat,
		const FName Name, const FLinearColor& To = FLinearColor::White,
		const float Duration = -1, const bool UseHSV = false,
		UCurveFloat* const Curve = nullptr);
#pragma endregion

	// Fades a custom primitive data.
	// Index: The index of the data. For a vector this is the start index.
	// IsScalar: whether this is a scalar or a color.
	//		When a scalar is used. only the R component of the color is used.
	//		When not a scalar, Index (R), Index+1 (G), +2 (B), and +3(A) will always be used.
	// To: is the target vector/color. For a scalar use the R field.
	// Duration: <0 uses the default, 0 is instant, >0 uses whatever specified.
	// UseHSV: uses HSV for lerp. Only used for vectors.
	//		It's more expensive, but looks better on colors.
	//		Might not look good on abstract vectors (e.g. used for position).
	// Curve. easing curve. has to be in the range 0-1 for both axis. "Y" overshooting is fine.
	// Warning:
	//		Triggering the same parameter twice will try to stop the previous as long as the component and index are the same.
	//		This is untested though.
	UFUNCTION(BlueprintCallable)
	bool DataFade(UPrimitiveComponent* const Component,
		const int32 Index, const bool IsScalar = true,
		const FLinearColor& To = FLinearColor::White,
		const float Duration = -1, const bool UseHSV = false,
		UCurveFloat* const Curve = nullptr
	);

	// returns true while fading.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool GetIsFading() const { return IsFading; }

	// returns true if a param with that name is fading
	UFUNCTION(BlueprintCallable)
	bool GetIsFadingMPC(const UMaterialParameterCollectionInstance* const MPCI, const FName Name) const;
	// returns true if a param with that name is fading
	UFUNCTION(BlueprintCallable)
	bool GetIsFadingDyn(const UMaterialInstanceDynamic* const Mat, const FName Name) const;
	// returns true if a param with that name is fading
	UFUNCTION(BlueprintCallable)
	bool GetIsFadingData(const UPrimitiveComponent* const Comp, const int32 Index) const;
	 
	// default fade duration. can be changed. and can be specified on the .ini config files.
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp, Config)
	float DurationDefault = 1.f;

	// whether this subsystem will be created.
	// when false, it will save some cycles, but might make the app crash.
	// should be changed in the config file
	UPROPERTY(BlueprintReadWrite, Config, Category=SetUp)
	bool ShouldBeCreated = true;
	
	// when ALL the items have faded
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Transient)
	FAnimMatDone OnDone;

	// when a specific mpc param is done
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Transient)
	FAnimMatDoneMPC OnItemDoneMPC;
	// when a specific dynamic material param is done
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Transient)
	FAnimMatDoneDyn OnItemDoneDyn;
	// when a specific custom primitive data param is done
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Transient)
	FAnimMatDoneData OnItemDoneData;

protected:
	bool ParamInitBasic(FAMBase& OParam, const FName Name,
		UCurveFloat* const Curve = nullptr,
		const float Duration = -1) const;

	bool ParamInitMPC(const UMaterialParameterCollection* const MPC,
		const FName Name, FAMBase& OParam,
		UCurveFloat* const Curve = nullptr,
		const float Duration = -1.0) const;

	bool ParamInitDyn(UMaterialInstanceDynamic* const Mat,
		const FName Name, FAMBase& OParam,
		UCurveFloat* const Curve = nullptr,
		const float Duration = -1.0) const;

	template<typename Item>
	bool ItemTick(const float DT, TArray<Item>& IOArr,
		void(UAnim::* Done)(const Item&));
	template<typename Item>
	void ItemsEmpty(TArray<Item>& IOArr,
		void(UAnim::* Done)(const Item&));

	void ItemDoneDynF(const FAMDFloat& It) {
		OnItemDoneDyn.Broadcast(It.Mat, It.Name);
	}
	void ItemDoneDynV(const FAMDVector& It) {
		OnItemDoneDyn.Broadcast(It.Mat, It.Name);
	}
	void ItemDoneMPCF(const FAMPFloat& Item) {
		OnItemDoneMPC.Broadcast(Item.MPCI, Item.Name);
	}
	void ItemDoneMPCV(const FAMPVector& Item) {
		OnItemDoneMPC.Broadcast(Item.MPCI, Item.Name);
	}
	void ItemDoneData(const FAMData& Item) {
		OnItemDoneData.Broadcast(Item.Comp, Item.Index);
	}

	bool IsFading = false;

	UPROPERTY(Transient)
	TArray<FAMPFloat> MPCFloatParams;
	UPROPERTY(Transient)
	TArray<FAMPVector> MPCVectorParams;
	UPROPERTY(Transient)
	TArray<FAMDFloat> DynFloatParams;
	UPROPERTY(Transient)
	TArray<FAMDVector> DynVectorParams;
	UPROPERTY(Transient)
	TArray<FAMData> DataParams;
};
