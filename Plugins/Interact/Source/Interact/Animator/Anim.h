#pragma once

#include "CoreMinimal.h"

#include "Anim.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAnimDone);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAnimDoneDyn, UMaterialInstanceDynamic* const, Mat, const FName, Name);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAnimDoneMPC, UMaterialParameterCollectionInstance* const, Mat, const FName, Name);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAnimDoneSnd, UAudioComponent* const, Cmp, const FName, Name);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAnimDoneData, UPrimitiveComponent* const, Comp, const int32, Index);

USTRUCT(Blueprintable, BlueprintType)
struct FABase {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Transient)
	FName Name = NAME_None;

	// polymorfic source object
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
	virtual ~FABase() = default;
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
	virtual bool IsSame(const FABase& Other) const {
		return Name == Other.Name && Obj == Other.Obj;
	}
	virtual bool IsSame(const UObject* const OtherObj, const FName OtherName) const {
		return Name == OtherName && Obj == OtherObj;
	}
	// loads the "From" value
	virtual bool LoadFrom() {
		UE_LOG(LogTemp, Warning, TEXT("Empty Base LoadFrom"));
		return false;
	}
};

// parameter float
USTRUCT(Blueprintable, BlueprintType)
struct FAPFloat: public FABase {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Transient)
	float From = 0.0;

	UPROPERTY(BlueprintReadWrite, Transient)
	float To = 1.0;

	virtual bool SetVal(const float Val = 1.0) const;
	virtual bool SetLerp(const float Prog) override;
	virtual bool LoadFrom() override;
};

// dynamic float
USTRUCT(Blueprintable, BlueprintType)
struct FADFloat: public FAPFloat {
	GENERATED_BODY()

public:
	virtual bool SetVal(const float Val = 1.0) const override;
	virtual bool LoadFrom() override;
};

// sound float
USTRUCT(Blueprintable, BlueprintType)
struct FASFloat: public FAPFloat {
	GENERATED_BODY()

public:
	virtual bool SetVal(const float Val = 1.0) const override;
	virtual bool LoadFrom() override;
};

USTRUCT(Blueprintable, BlueprintType)
struct FAPVector: public FABase {
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
	virtual bool LoadFrom() override;
};

USTRUCT(Blueprintable, BlueprintType)
struct FADVector: public FAPVector {
	GENERATED_BODY()

public:
	virtual bool SetVal(const FLinearColor& Val = FLinearColor::White) const override;
	virtual bool LoadFrom() override;
};

USTRUCT(Blueprintable, BlueprintType)
struct FAData: public FABase {
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
	virtual bool LoadFrom() override;
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

#pragma region sound
	// Fades in or out.
	// By design, it will replace any previous fades with the same name.
	// If it's fading it will continue from where it is, even if the direction changes.
	// Name: name of the parameter
	// To: value to fade to.
	// Duration: <0 uses the default, 0 is instant, >0 uses whatever specified.
	// Curve. easing curve. has to be in the range 0-1 for both axis. Y overshooting is fine.
	UFUNCTION(BlueprintCallable)
	bool SndFloatFade(UAudioComponent* const Cmp,
		const FName Name, const float To = 1.0, const float Duration = -1,
		UCurveFloat* const Curve = nullptr);
#pragma endregion
#pragma region data
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
	bool DataFade(UPrimitiveComponent* const Cmp,
		const int32 Index, const bool IsScalar = true,
		const FLinearColor& To = FLinearColor::White,
		const float Duration = -1, const bool UseHSV = false,
		UCurveFloat* const Curve = nullptr
	);
#pragma endregion

#pragma region isfading
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
	bool GetIsFadingData(const UPrimitiveComponent* const Cmp, const int32 Index) const;
	UFUNCTION(BlueprintCallable)
	bool GetIsFadingSound(const UAudioComponent* const Cmp, const FName Name) const;
#pragma endregion

	// default fade duration. can be changed. and can be specified on the .ini config files.
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp, Config)
	float DurationDefault = 1.f;

	// whether this subsystem will be created.
	// when false, it will save some cycles, but might make the app crash.
	// should be changed in the config file
	UPROPERTY(BlueprintReadWrite, Config, Category=SetUp)
	bool ShouldBeCreated = true;

#pragma region delegates
	// when ALL the items have faded
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Transient)
	FAnimDone OnDone;
	
	// when a specific mpc param is done
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Transient)
	FAnimDoneMPC OnItemDoneMPC;
	// when a specific dynamic material param is done
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Transient)
	FAnimDoneDyn OnItemDoneDyn;
	// when a specific custom primitive data param is done
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Transient)
	FAnimDoneData OnItemDoneData;
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Transient)
	FAnimDoneSnd OnItemDoneSnd;
#pragma endregion

protected:
	bool ItemInitBasic(FABase& OParam, UObject* const Obj, const FName Name,
		UCurveFloat* const Curve = nullptr,
		const float Duration = -1) const;

	bool ItemInitMPC(const UMaterialParameterCollection* const MPC,
		const FName Name, FABase& OParam,
		UCurveFloat* const Curve = nullptr,
		const float Duration = -1.0) const;

	bool ItemInitDyn(UMaterialInstanceDynamic* const Mat,
		const FName Name, FABase& OParam,
		UCurveFloat* const Curve = nullptr,
		const float Duration = -1.0) const;

	template<typename Item>
	bool ItemTick(const float DT, TArray<Item>& IOArr,
		void(UAnim::* Done)(const Item&));
	template<typename Item>
	void ItemsEmpty(TArray<Item>& IOArr,
		void(UAnim::* Done)(const Item&));
	template<typename Item>
	void ItemsRemoveSame(const Item& Param, TArray<Item>& IOArr);
	// ensure te set To and Duration first
	template<typename Item>
	bool ItemsSetNow(const Item& Param,
		void(UAnim::* Done)(const Item&));
	template<typename Item>
	bool ItemSetup(Item& OParam, UObject* const Obj, const FName Name,
		UCurveFloat* const Curve, const float Duration, TArray<Item>& IOItems,
		void(UAnim::* Done)(const Item&)
		);
	// can't make const due to the declaration of the pointer
#pragma region done
	void ItemDoneDynF(const FADFloat& It);
	void ItemDoneDynV(const FADVector& It);
	void ItemDoneMPCF(const FAPFloat& Item) ;
	void ItemDoneMPCV(const FAPVector& Item);
	void ItemDoneData(const FAData& Item);
	void ItemDoneSndF(const FASFloat& Item);
#pragma endregion
	bool IsFading = false;

	UPROPERTY(Transient)
	TArray<FAPFloat> MPCFloatParams;
	UPROPERTY(Transient)
	TArray<FAPVector> MPCVectorParams;
	UPROPERTY(Transient)
	TArray<FADFloat> DynFloatParams;
	UPROPERTY(Transient)
	TArray<FADVector> DynVectorParams;
	UPROPERTY(Transient)
	TArray<FAData> DataParams;
	UPROPERTY(Transient)
	TArray<FASFloat> SndFloatParams;
};
