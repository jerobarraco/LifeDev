// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once

#include "Subsystems/WorldSubsystem.h"
#include "UObject/ObjectPtr.h"

#include "Anim.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAnimDone);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAnimDynDone, UMaterialInstanceDynamic* const, Mat, const FName, Name);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAnimMPCDone, UMaterialParameterCollectionInstance* const, Mat, const FName, Name);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAnimSndDone, UAudioComponent* const, Cmp, const FName, Name);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAnimDataDone, UPrimitiveComponent* const, Comp, const int32, Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAnimCompDone, USceneComponent* const, Comp, const FName, Name);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAnimGenDone, UObject* const, Obj, const FName, Name);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FAnimGenUpd, UObject* const, Obj, const FName, Name, const float, Alpha);

class UCurveFloat;

// common stuff to be passed as parameter to all function
USTRUCT(Blueprintable, BlueprintType)
struct FAParams {
	GENERATED_BODY()

public:
	// name, necessary to distinguish. without it the effect won't start.
	// Except for Data (the ones with Index), and CompTrans.
	UPROPERTY(BlueprintReadWrite, Transient)
	FName Name = NAME_None;
	UPROPERTY(BlueprintReadWrite, Transient)
	TObjectPtr<UCurveFloat> Curve = nullptr;
	// the duration. -1 for using the default in the anim subsystem or config.
	UPROPERTY(BlueprintReadWrite, Transient)
	float Duration = -1;
	UPROPERTY(BlueprintReadWrite, Transient)
	bool Reversed = false;
	UPROPERTY(BlueprintReadWrite, Transient)
	bool Loop = false;
	UPROPERTY(BlueprintReadWrite, Transient)
	bool Bounce = false;
	UPROPERTY(BlueprintReadWrite, Transient)
	bool UseDilation = true;
};

USTRUCT(Blueprintable, BlueprintType)
struct FABase {
	GENERATED_BODY()

public:
	// polymorfic source object
	UPROPERTY(BlueprintReadWrite, Transient)
	TObjectPtr<UObject> Obj = nullptr;
	
	UPROPERTY(BlueprintReadWrite, Transient)
	FAParams Pars;

	// how much it has elapsed already
	UPROPERTY(BlueprintReadWrite, Transient)
	float Elapsed = 1.0;

	FORCEINLINE bool IsDone() const {
		return FMath::IsNearlyEqual(Elapsed, Pars.Duration)
			| (Elapsed > Pars.Duration);
	}

	// needed or android won't package >_<! due to the virtual functions
	// has to be public.
	virtual ~FABase() = default;
	// named this way to avoid name collision with IsValid
	virtual bool FIsValid() const;
	// adds dt to the elapsed, returns current progress
	float AddDT(float DT);
	// returns true on done
	bool Tick(const float DT);
	// set value using Lerp progress. override and call SetValue yourself.
	virtual bool SetLerp(const float Prog) {
		UE_LOG(LogTemp, Warning, TEXT("Empty Base SetLerp"));
		return false;
	}
	virtual bool IsSame(const FABase& Other) const {
		return (Pars.Name == Other.Pars.Name) & (Obj == Other.Obj);
	}
	virtual bool IsSame(const UObject* const OtherObj, const FName OtherName) const {
		return (Pars.Name == OtherName) & (Obj == OtherObj);
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
struct FAData: public FAPVector {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Transient)
	int32 Index = -1;

	UPROPERTY(BlueprintReadWrite, Transient)
	bool IsScalar = true;

	bool GetCurrent(FLinearColor& OCurrent) const;
	virtual bool SetVal(const FLinearColor& V = FLinearColor::White) const override;
	virtual bool LoadFrom() override;
};

USTRUCT(Blueprintable, BlueprintType)
struct FACTrans: public FABase {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Transient)
	FTransform From = FTransform::Identity;

	UPROPERTY(BlueprintReadWrite, Transient)
	FTransform To = FTransform::Identity;

	// world or relative
	UPROPERTY(BlueprintReadWrite, Transient)
	bool IsWorld = true;
	UPROPERTY(BlueprintReadWrite, Transient)
	bool IsAdditive = false;
	UPROPERTY(BlueprintReadWrite, Transient)
	bool UseSweep = false;

	virtual bool SetVal(const FTransform& Val) const;
	virtual bool SetLerp(const float Prog) override;
	virtual bool LoadFrom() override;
};

USTRUCT(Blueprintable, BlueprintType)
struct FAGen: public FABase {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FAnimGenUpd OnUpdate; // CAN'T be transient, or it will "forget" the values.

	bool To:1 = true; // just to be able to compile
	virtual bool SetVal(const bool Val=false) const { return true; }; // just to compile
	virtual bool SetLerp(const float Prog) override;
};

USTRUCT(Blueprintable, BlueprintType)
struct FATime: public FAPFloat {
	GENERATED_BODY()

public:
	virtual bool SetVal(const float Val = 1.0) const;
	virtual bool LoadFrom() override;
};

// Subsystem that animates stuff in a more easy way. this is for one-off fire and forget effects.
// it offers much less control than the "CAnimator" components.
// CAnimator objects are preferred.
// One of the downsides of this class is that it runs on every tick, there's no way to optimize each animation.
// Whereas the CAnimator works really well with the Significance and other manual tweaks.
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

	// tries to stop all instances of this animation.
	// Index is only used for custom primitive data.
	// Obj is the owner object (the mpc, component, prim data, etc.)
	UFUNCTION(BlueprintCallable, meta=(AutoCreateRefTerm="Name"))
	void Stop(UObject* const Obj, const FName& Name, const int32 Index=-1);
#pragma endregion

#pragma region mpci
	// Fades in or out.
	// By design, it will replace any previous fades with the same name.
	// If it's fading it will continue from where it is, even if the direction changes.
	// Name: name of the parameter
	// To: value to fade to.
	// Duration: <0 uses the default, 0 is instant, >0 uses whatever specified.
	// Curve. easing curve. has to be in the range 0-1 for both axis. Y overshooting is fine.
	UFUNCTION(BlueprintCallable, meta=(AutoCreateRefTerm="Params"))
	bool MPCFloatFade(const UMaterialParameterCollection* const MPC,
		const FAParams& Params, const float To = 1.0);

	// Fades a vector (or color).
	// Name: the name of the parameter to fade
	// To: is the target vector/color
	// Duration: <0 uses the default, 0 is instant, >0 uses whatever specified.
	// UseHSV: uses HSV for lerp. is more expensive, but looks better on colors.
	// Curve. easing curve. has to be in the range 0-1 for both axis. Y overshooting is fine.
	UFUNCTION(BlueprintCallable, meta=(AutoCreateRefTerm="Params,To"))
	bool MPCVectorFade(const UMaterialParameterCollection* const MPC, const FAParams& Params,
		const FLinearColor& To = FLinearColor::White, const bool UseHSV = false);
#pragma endregion

#pragma region dynmat
	// Fades in or out.
	// By design, it will replace any previous fades with the same name.
	// If it's fading it will continue from where it is, even if the direction changes.
	// Name: name of the parameter
	// To: value to fade to.
	// Duration: <0 uses the default, 0 is instant, >0 uses whatever specified.
	// Curve. easing curve. has to be in the range 0-1 for both axis. Y overshooting is fine.
	UFUNCTION(BlueprintCallable, meta=(AutoCreateRefTerm="Params"))
	bool DynFloatFade(UMaterialInstanceDynamic* const Mat, const FAParams& Params,
		const float To = 1.0);

	// Fades a vector (or color).
	// Name: the name of the parameter to fade
	// To: is the target vector/color
	// Duration: <0 uses the default, 0 is instant, >0 uses whatever specified.
	// UseHSV: uses HSV for lerp. is more expensive, but looks better on colors.
	// Curve. easing curve. has to be in the range 0-1 for both axis. Y overshooting is fine.
	UFUNCTION(BlueprintCallable, meta=(AutoCreateRefTerm="Params,To"))
	bool DynVectorFade(UMaterialInstanceDynamic* const Mat, const FAParams& Params,
		const FLinearColor& To = FLinearColor::White, const bool UseHSV = false);
#pragma endregion

#pragma region sound
	// Fades in or out.
	// By design, it will replace any previous fades with the same name.
	// If it's fading it will continue from where it is, even if the direction changes.
	// Name: name of the parameter
	// To: value to fade to.
	// Duration: <0 uses the default, 0 is instant, >0 uses whatever specified.
	// Curve. easing curve. has to be in the range 0-1 for both axis. Y overshooting is fine.
	UFUNCTION(BlueprintCallable, meta=(AutoCreateRefTerm="Params"))
	bool SndFloatFade(UAudioComponent* const Comp,
		const FAParams& Params, const float To = 1.0);
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
	UFUNCTION(BlueprintCallable, meta=(AutoCreateRefTerm="Params,To"))
	bool DataFade(UPrimitiveComponent* const Comp, const FAParams& Params,
		const int32 Index, bool IsScalar = true,
		const FLinearColor& To = FLinearColor::White, bool UseHSV = false);
#pragma endregion

#pragma region comp
	UFUNCTION(BlueprintCallable, meta=(AutoCreateRefTerm="Params,To"))
	bool CompTransFade(USceneComponent* Comp, const FAParams& Params,
		const FTransform& To, const bool IsWorld = false, const bool IsAdditive = false,
		const bool UseSweep = false);
#pragma endregion
#pragma region gen
	UFUNCTION(BlueprintCallable, meta=(AutoCreateRefTerm="Params"))
	bool TimeFade(UObject* const Owner, const FAParams& Params, const float To=1.);
	UFUNCTION(BlueprintCallable, meta=(AutoCreateRefTerm="Params"))
	bool GenFade(UObject* const Owner, const FAParams& Params, const FAnimGenUpd& OnUpd);
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
	UFUNCTION(BlueprintCallable)
	bool GetIsFadingData(const UPrimitiveComponent* const Comp, const int32 Index) const;
	UFUNCTION(BlueprintCallable)
	bool GetIsFadingSound(const UAudioComponent* const Comp, const FName Name) const;
	UFUNCTION(BlueprintCallable)
	bool GetIsFadingComp(const USceneComponent* const Comp) const;
#pragma endregion

	// default fade duration. can be changed. and can be specified on the .ini config files.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp, Config)
	float DurationDefault = 1.f;

#pragma region delegates
	// when ALL the items have faded
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Transient)
	FAnimDone OnDone;
	// when a specific mpc param is done
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Transient)
	FAnimMPCDone OnItemMPCDone;
	// when a specific dynamic material param is done
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Transient)
	FAnimDynDone OnItemDynDone;
	// when a specific custom primitive data param is done
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Transient)
	FAnimDataDone OnItemDataDone;
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Transient)
	FAnimSndDone OnItemSndDone;
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Transient)
	FAnimCompDone OnItemCompDone;
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Transient)
	FAnimGenDone OnItemGenDone;
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Transient)
	FAnimGenDone OnItemTimeDone;
#pragma endregion

protected:
	// whether this subsystem will be created.
	// when false, it will save some cycles, but might make the app crash if you try to use it without checking the pointer.
	// (checking the pointer is always recommended)
	// should be changed in the config file
	UPROPERTY(BlueprintReadWrite, Config, Category=SetUp)
	bool ShouldCreate = true;

#pragma region Items
	bool ItemInit(FABase& IOItem) const;
	template<typename Type>
	bool ItemSetup(Type& IOItem, TArray<Type>& IOItems, void(UAnim::* Done)(const Type&));
	template<typename Type>
	void ItemsRem(const Type& Item, TArray<Type>& IOArr);
	// ensure te set To and Duration before calling.
	template<typename Type>
	bool ItemsSetNow(const Type& Item,
		void(UAnim::* Done)(const Type&));
	template<typename Type>
	bool ItemTick(const float DT, TArray<Type>& IOArr, void(UAnim::* Done)(const Type&));
	template<typename Type>
	void ItemsEmpty(TArray<Type>& IOArr, void(UAnim::* Done)(const Type&));
	template<typename Type>
	FORCEINLINE bool ItemIsIn(const UObject* const Obj, const FName Name,
		const TArray<Type>& IArr) const;
#pragma endregion

#pragma region Done
	// can't make const due to the declaration of the pointer
	void ItemDoneDynF(const FADFloat& It);
	void ItemDoneDynV(const FADVector& It);
	void ItemDoneMPCF(const FAPFloat& Item) ;
	void ItemDoneMPCV(const FAPVector& Item);
	void ItemDoneData(const FAData& Item);
	void ItemDoneSndF(const FASFloat& Item);
	void ItemDoneComp(const FACTrans& Item);
	void ItemDoneGen(const FAGen& Item);
	void ItemDoneTime(const FATime& Item);
#pragma endregion

#pragma region Vars
	bool IsFading = false;

	UPROPERTY(Transient)
	TArray<FAPFloat> ItemsMPCF;
	UPROPERTY(Transient)
	TArray<FAPVector> ItemsMPCV;
	UPROPERTY(Transient)
	TArray<FADFloat> ItemsDynF;
	UPROPERTY(Transient)
	TArray<FADVector> ItemsDynV;
	UPROPERTY(Transient)
	TArray<FAData> ItemsData;
	UPROPERTY(Transient)
	TArray<FASFloat> ItemsSndF;
	UPROPERTY(Transient)
	TArray<FACTrans> ItemsCompT;
	UPROPERTY(Transient)
	TArray<FAGen> ItemsGen;
	UPROPERTY(Transient)
	TArray<FATime> ItemsTime;
#pragma endregion
};
