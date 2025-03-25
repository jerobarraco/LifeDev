// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#include "Anim.h"

#include "Components/AudioComponent.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"

DEFINE_LOG_CATEGORY_STATIC(LogAnim, Log, Log);

// https://dev.epicgames.com/documentation/en-us/unreal-engine/storing-custom-data-in-unreal-engine-materials-per-primitive

#pragma region structs
float FABase::AddDT(const float DT) {
	// clamp to perfect duration, to avoid overshooting.
	Elapsed = FMath::Min(Elapsed + DT,Duration);
	if (Duration == 0) return 0; // don't need nearly zero. it's just for the division below.

	const float RProg = Elapsed / Duration;
	return IsValid(Curve) ? Curve->GetFloatValue(RProg) : RProg;
}

bool FABase::Tick(const float DT) {
	// no need to do this. while it would be an optimization. it's not required.
	// if (UNLIKELY(!FIsValid())) return true;

	const float Prog = AddDT(DT);
	SetLerp(Prog);
	return IsDone();
}

// define here to avoid including the type on header
bool FABase::FIsValid() const { return !Name.IsNone() && IsValid(Obj); }

#pragma region setval
bool FAPFloat::SetVal(const float Val) const {
	UE_LOG(LogAnim, Verbose, TEXT("%hs Name=%s Val=%.4f"),
		__func__, *Name.ToString(), Val);
	if (UNLIKELY(!Name.IsNone())) return false;

	UMaterialParameterCollectionInstance* const MM =
		Cast<UMaterialParameterCollectionInstance>(Obj);
	if (UNLIKELY(!MM)) return false;

	return MM->SetScalarParameterValue(Name, Val);
}

bool FADFloat::SetVal(const float Val) const {
	UE_LOG(LogAnim, Verbose, TEXT("%hs Name=%s Val=%.4f"),
		__func__, *Name.ToString(), Val);
	if (UNLIKELY(Name.IsNone())) return false;

	UMaterialInstanceDynamic* const MM = Cast<UMaterialInstanceDynamic>(Obj);
	if (UNLIKELY(!MM)) return false;

	MM->SetScalarParameterValue(Name, Val);
	return true;
}

bool FASFloat::SetVal(const float Val) const {
	UE_LOG(LogAnim, Verbose, TEXT("%hs Name=%s Val=%.4f"),
		__func__, *Name.ToString(), Val);
	if (UNLIKELY(Name.IsNone())) return false;

	UAudioComponent* const MM = Cast<UAudioComponent>(Obj);
	if (UNLIKELY(!MM || !MM->IsPlaying())) return false; // isplaying is critical to avoid crash. yes crash.

	MM->SetFloatParameter(Name, Val);
	return true;
}

bool FADVector::SetVal(const FLinearColor& Val) const {
	UE_LOG(LogAnim, Verbose, TEXT("%hs Name=%s Val=%s"),
			__func__, *Name.ToString(), *Val.ToString());
	if (UNLIKELY(Name.IsNone())) return false;

	UMaterialInstanceDynamic* const MM = Cast<UMaterialInstanceDynamic>(Obj);
	if (UNLIKELY(!MM)) return false;
	MM->SetVectorParameterValue(Name, Val);
	return true;
}

bool FAPVector::SetVal(const FLinearColor& Val) const {
	UE_LOG(LogAnim, Verbose, TEXT("%hs Name=%s Val=%s"),
			__func__, *Name.ToString(), *Val.ToString());
	if (UNLIKELY(Name.IsNone())) return false;

	UMaterialParameterCollectionInstance* const MM =
		Cast<UMaterialParameterCollectionInstance>(Obj);
	if (UNLIKELY(!MM)) return false;

	return MM->SetVectorParameterValue(Name, Val);
}

bool FAData::SetVal(const FLinearColor& V) const {
	UE_LOG(LogAnim, Verbose, TEXT("%hs Name=%s Val=%s Index=%i Scalar=%i"),
		__func__, *GetNameSafe(Obj), *V.ToString(), Index, IsScalar);
	if (UNLIKELY(Name.IsNone())) return false;

	UPrimitiveComponent* const Comp =
		Cast<UPrimitiveComponent>(Obj);
	if (UNLIKELY(!Comp)) return false;
	
	if (IsScalar)
		Comp->SetCustomPrimitiveDataFloat(Index, V.R);
	else
		Comp->SetCustomPrimitiveDataVector4(Index, V);

	// ONLY works on dynamic material which is what we are trying to avoid
	// Comp->SetScalarParameterForCustomPrimitiveData(Name, V.R);
	// Comp->SetVectorParameterForCustomPrimitiveData(Name, V);

	return true;
}

bool FACTrans::SetVal(const FTransform& Val) const {
	UE_LOG(LogAnim, Verbose, TEXT("%hs Name=%s Val=%s"),
			__func__, *Name.ToString(), *Val.ToString());
	if (UNLIKELY(Name.IsNone())) return false;

	USceneComponent* const Comp =
		Cast<USceneComponent>(Obj);
	if (UNLIKELY(!Comp)) return false;

	if (IsWorld)
		Comp->SetWorldTransform(Val, UseSweep);
	else
		Comp->SetRelativeTransform(Val, UseSweep);
	return true;
}

#pragma endregion
#pragma region LoadFrom
bool FAPFloat::LoadFrom() {
	const UMaterialParameterCollectionInstance* const MPCI =
		Cast<UMaterialParameterCollectionInstance>(Obj);
	return MPCI && MPCI->GetScalarParameterValue(Name, From);
}

bool FAPVector::LoadFrom() {
	const UMaterialParameterCollectionInstance* const MPCI =
		Cast<UMaterialParameterCollectionInstance>(Obj);
	return MPCI && MPCI->GetVectorParameterValue(Name, From);
}

bool FADFloat::LoadFrom() {
	const UMaterialInstanceDynamic* const Mat =
		Cast<UMaterialInstanceDynamic>(Obj);
	return Mat && Mat->GetScalarParameterValue(Name, From); 
}

bool FADVector::LoadFrom() {
	const UMaterialInstanceDynamic* const Mat =
		Cast<UMaterialInstanceDynamic>(Obj);
	return Mat && Mat->GetVectorParameterValue(Name, From); 
}

bool FASFloat::LoadFrom() {
	UAudioComponent* const Comp = Cast<UAudioComponent>(Obj);
	if (UNLIKELY(!Comp)) return false;

	const TArray<FAudioParameter>& Params = Comp->GetInstanceParameters(); // notice is valid at the top
	for (const FAudioParameter& P : Params) {
		if (LIKELY(P.ParamName != Name)) continue;
		From = P.FloatParam;
		return true;
	}

	return false;
}

bool FAData::LoadFrom() {
	return GetCurrent(From);
}

bool FACTrans::LoadFrom() {
	const USceneComponent* const Comp = Cast<USceneComponent>(Obj);
	if (UNLIKELY(!Comp)) return false;

	From = IsWorld ? Comp->GetComponentTransform() : From = Comp->GetRelativeTransform();
	return true;
}
#pragma endregion

#pragma region SetLerp
bool FAPFloat::SetLerp(const float Prog) {
	const float Val = FMath::LerpStable(From, To, Prog);
	return SetVal(Val);
}

bool FAPVector::SetLerp(const float Prog) {
	const FLinearColor& Val = UseHSV ?
		FLinearColor::LerpUsingHSV(From, To, Prog) :
		FMath::LerpStable(From, To, Prog);
	return SetVal(Val);
}
//
// bool FAData::SetLerp(const float Prog) {
// 	const FLinearColor Val = UseHSV ?
// 		FLinearColor::LerpUsingHSV(From, To, Prog) :
// 		FMath::LerpStable(From, To, Prog);
// 	return SetVal(Val);
// }

bool FACTrans::SetLerp(const float Prog) {
	USceneComponent* const Comp = Cast<USceneComponent>(Obj);
	if (UNLIKELY(!Comp)) return false;
	
	FTransform TNew = From;
	if (IsAdditive) {
		From.BlendFromIdentityAndAccumulate(
			TNew, To, (const ScalarRegister) Prog);
	} else {
		// Thanks, Tim! this actually works very well!
		TNew.BlendWith(To, Prog);
	}

	if (IsWorld)
		Comp->SetWorldTransform(TNew, UseSweep);
	else 
		Comp->SetRelativeTransform(TNew, UseSweep);
	return true;
}

bool FAGen::SetLerp(const float Prog) {
	if (UNLIKELY(!OnUpdate.IsBound())) return false;
	OnUpdate.Execute(Obj, Name, Prog);
	return true;
}
#pragma endregion

bool FAData::GetCurrent(FLinearColor& OCurrent) const {
	OCurrent = FLinearColor::Black; // initialize to a sane value

	if (UNLIKELY(Name.IsNone())) return false;

	const UPrimitiveComponent* const Comp =
		Cast<UPrimitiveComponent>(Obj);
	if (UNLIKELY(!Comp)) return false;

	const FCustomPrimitiveData& Prim = Comp->GetCustomPrimitiveData();
	const int32 Num = Prim.Data.Num();
	int32 It = Index;

	// lame way to get the data, but the data itself it's quite lame.
	// manual unpacking :(
	
	if (It<Num) {
		OCurrent.R = Prim.Data[It];
		++It;
	}
	// done for scalars
	if (IsScalar) return true;

	// i could have used only one if, but i want to unpack as many as i can.
	// init as black, so it's probably ok, (though Alpha is probably 1)
	if (It<Num) {
		OCurrent.G = Prim.Data[It];
		++It;
	}
	if (It<Num) {
		OCurrent.B = Prim.Data[It];
		++It;
	}
	if (It<Num) OCurrent.A = Prim.Data[It];

	UE_LOG(LogAnim, Log, TEXT("%hs Name=%s Current=%s Index=%i Scalar=%i"),
		__func__, *GetNameSafe(Comp), *OCurrent.ToString(), Index, IsScalar);

	return true;
}
#pragma endregion

UAnim::UAnim():Super() {}

UAnim* UAnim::Instance(const UObject*const  O) {
	if (UNLIKELY(!IsValid(O))) return nullptr;

	const UWorld* const W = O->GetWorld();
	if (UNLIKELY(!W)) return nullptr;

	UAnim* const AnimMat = W->GetSubsystem<UAnim>();
	return LIKELY(IsValid(AnimMat)) ? AnimMat : nullptr;
}

bool UAnim::ItemInitBasic(FABase& OItem, UObject* const Obj, const FName Name,
UCurveFloat* const Curve, const float Duration) const {
	UE_LOG(LogAnim, Log, TEXT("%hs name=%s, duration=%.3f"),
		__func__, *Name.ToString(), Duration);

	OItem.Obj = Obj;
	OItem.Name = Name;
	OItem.Curve = IsValid(Curve) ? Curve : nullptr;
	OItem.Elapsed = 0.0; // reset in case it was running
	OItem.Duration = Duration < 0 ? DurationDefault : Duration;

	if (UNLIKELY(!IsValid(Obj))) {
		UE_LOG(LogAnim, Warning, TEXT("%hs Root object is invalid. Name=%s. Stop."),
			__func__, *Name.ToString());
		return false;
	}

	// at end to allow for data params
	if (UNLIKELY(OItem.Name.IsNone())) {
		UE_LOG(LogAnim, Warning, TEXT("%hs Name can't be none. Stop."),
			__func__);
		return false;
	}

	return true;
}

#pragma region dones
void UAnim::ItemDoneDynF(const FADFloat& It) {
	OnItemDynDone.Broadcast(Cast<UMaterialInstanceDynamic>(It.Obj), It.Name);
}

void UAnim::ItemDoneDynV(const FADVector& It) {
	OnItemDynDone.Broadcast(Cast<UMaterialInstanceDynamic>(It.Obj), It.Name);
}

void UAnim::ItemDoneMPCF(const FAPFloat& Item) {
	OnItemMPCDone.Broadcast(Cast<UMaterialParameterCollectionInstance>(Item.Obj), Item.Name);
}

void UAnim::ItemDoneMPCV(const FAPVector& Item) {
	OnItemMPCDone.Broadcast(Cast<UMaterialParameterCollectionInstance>(Item.Obj), Item.Name);
}

void UAnim::ItemDoneData(const FAData& Item) {
	OnItemDataDone.Broadcast(Cast<UPrimitiveComponent>(Item.Obj), Item.Index);
}

void UAnim::ItemDoneSndF(const FASFloat& Item) {
	OnItemSndDone.Broadcast(Cast<UAudioComponent>(Item.Obj), Item.Name);
}

void UAnim::ItemDoneComp(const FACTrans& Item) {
	OnItemCompDone.Broadcast(Cast<USceneComponent>(Item.Obj), Item.Name);
}

void UAnim::ItemDoneGen(const FAGen& Item) {
	OnItemGenDone.Broadcast(Item.Obj, Item.Name);
}
#pragma endregion

template <typename Item>
bool UAnim::ItemSetup(Item& OItem, UObject* const Obj, const FName Name,
UCurveFloat* const Curve, const float Duration, TArray<Item>& IOItems,
void(UAnim::* Done)(const Item&) ) {
	if (UNLIKELY(!ItemInitBasic(OItem, Obj, Name, Curve, Duration))) {
		UE_LOG(LogAnim, Warning, TEXT("%hs Failed to init param. Stop."),
			__func__);
		return false;
	}

	ItemsRemoveSame(OItem, IOItems);
	if (ItemsSetNow(OItem, Done)) return true;

	const bool Got = OItem.LoadFrom();
	UE_CLOG(UNLIKELY(!Got), LogAnim, Warning, TEXT("%hs Can't get the current value."),
		__func__); // we do it anyway.

	IOItems.Add(OItem);
	IsFading = true;
	return true;
}

bool UAnim::MPCFloatFade(const UMaterialParameterCollection* const MPC, const FName Name,
const float To, const float Duration, UCurveFloat* const Curve) {
	UE_LOG(LogAnim, Log, TEXT("%hs name=%s, to=%.3f, duration=%.3f"),
		__func__, *Name.ToString(), To, Duration);
	
	if (UNLIKELY(!IsValid(MPC))) return false;
	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return false;
	
	FAPFloat Param;
	Param.To = To;
	UObject* const Obj = World->GetParameterCollectionInstance(MPC);
	return ItemSetup(Param, Obj, Name, Curve, Duration, ItemsMPCF, &UAnim::ItemDoneMPCF);
}

bool UAnim::MPCVectorFade(const UMaterialParameterCollection* const MPC, const FName Name,
const FLinearColor& To, const float Duration, const bool UseHSV,
	UCurveFloat* const Curve) {
	UE_LOG(LogAnim, Log, TEXT("%hs name=%s, to=%s, duration=%.3f, usehsv=%i"),
		__func__, *Name.ToString(), *To.ToString(), Duration, UseHSV);

	FAPVector Param;
	Param.UseHSV = UseHSV;
	Param.To = To;

	if (UNLIKELY(!IsValid(MPC))) return false;
	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return false;
	
	UObject* const Obj = World->GetParameterCollectionInstance(MPC);
	return ItemSetup(Param, Obj, Name, Curve, Duration, ItemsMPCV, &UAnim::ItemDoneMPCV);
}

bool UAnim::DynFloatFade(UMaterialInstanceDynamic* const Mat, const FName Name, const float To,
const float Duration, UCurveFloat* const Curve) {
	UE_LOG(LogAnim, Log, TEXT("%hs name=%s, to=%.3f, duration=%.3f"),
		__func__, *Name.ToString(), To, Duration);

	FADFloat Param;
	Param.To = To;
	return ItemSetup(Param, Mat, Name, Curve, Duration, ItemsDynF, &UAnim::ItemDoneDynF);
}

bool UAnim::DynVectorFade(UMaterialInstanceDynamic* const Mat, const FName Name, const FLinearColor& To,
const float Duration, const bool UseHSV, UCurveFloat* const Curve) {
	UE_LOG(LogAnim, Log, TEXT("%hs name=%s, to=%s, duration=%.3f, usehsv=%i"),
		__func__, *Name.ToString(), *To.ToString(), Duration, UseHSV);

	FADVector Param;
	Param.UseHSV = UseHSV;
	Param.To = To;
	return ItemSetup(Param, Mat, Name, Curve, Duration, ItemsDynV, &UAnim::ItemDoneDynV);
}

bool UAnim::SndFloatFade(UAudioComponent* const Comp, const FName Name, const float To, const float Duration,
UCurveFloat* const Curve) {
	UE_LOG(LogAnim, Log, TEXT("%hs name=%s, to=%.3f, duration=%.3f"),
		__func__, *Name.ToString(), To, Duration);

	FASFloat Param;
	Param.To = To;
	return ItemSetup(Param, Comp, Name, Curve, Duration, ItemsSndF, &UAnim::ItemDoneSndF);
}

bool UAnim::DataFade(UPrimitiveComponent* const Comp, const int32 Index, const bool IsScalar,
const FLinearColor& To, const float Duration, const bool UseHSV, UCurveFloat* const Curve) {

	UE_LOG(LogAnim, Log, TEXT("%hs comp=%s, index=%i, scalar=%i, to=%s, duration=%.3f, hsv=%i"),
		__func__, *GetNameSafe(Comp), Index, IsScalar, *To.ToString(), Duration, UseHSV);

	FAData Item;
	Item.Index = Index; // still need the index for getCurrent and SetVal
	Item.UseHSV = UseHSV;
	Item.To = To;
	Item.IsScalar = IsScalar;
	// necessary for polymorphic behavior
	const FName Name = FName(FString::Printf(TEXT("%i"), Index));
	return ItemSetup(Item, Comp, Name, Curve, Duration, ItemsData, &UAnim::ItemDoneData);
}

bool UAnim::CompTransFade(USceneComponent* const Comp, const FTransform& To, const float Duration, const bool IsWorld,
const bool IsAdditive, const bool UseSweep, UCurveFloat* const Curve) {
	UE_LOG(LogAnim, Log, TEXT("%hs comp=%s, to=%s, duration=%.3f"),
		__func__, *GetNameSafe(Comp), *To.ToString(), Duration);

	FACTrans Item;
	Item.To = To;
	Item.UseSweep = UseSweep;
	Item.IsAdditive = IsAdditive;
	Item.IsWorld = IsWorld;
	// necessary for polymorphic behavior
	static const FName Name = NAME_Transform;
	return ItemSetup(Item, Comp, Name, Curve, Duration, ItemsCompT, &UAnim::ItemDoneComp);
}

bool UAnim::GenFade(const FName& Name, UObject* Owner, const FAnimGenUpd& OnUpd,
const float Duration, UCurveFloat* const Curve) {
	UE_LOG(LogAnim, Log, TEXT("%hs name=%s duration=%.3f"), __func__, *Name.ToString(), Duration);
	FAGen Item;
	Item.OnUpdate = OnUpd;
	const bool Ok = ItemSetup(Item, Owner, Name, Curve, Duration, ItemsGen, &UAnim::ItemDoneGen);
	return Ok;
}

void UAnim::Tick(const float DT) {
	Super::Tick(DT);
	UE_LOG(LogAnim, Verbose, TEXT("%hs"), __func__);

	const bool ContMPCFloat = ItemTick(DT, ItemsMPCF, &UAnim::ItemDoneMPCF);
	const bool ContMPCVec = ItemTick(DT, ItemsMPCV, &UAnim::ItemDoneMPCV);
	const bool ContData = ItemTick(DT, ItemsData, &UAnim::ItemDoneData);
	const bool ContDynFloat = ItemTick(DT, ItemsDynF, &UAnim::ItemDoneDynF);
	const bool ContDynVector = ItemTick(DT, ItemsDynV, &UAnim::ItemDoneDynV);
	const bool ContSndFloat = ItemTick(DT, ItemsSndF, &UAnim::ItemDoneSndF);
	const bool ContComp = ItemTick(DT, ItemsCompT, &UAnim::ItemDoneComp);
	const bool ContGen = ItemTick(DT, ItemsGen, &UAnim::ItemDoneGen);
	// done this way to avoid short-circuit to skip vec (though if the compiler is trying to be smart...)
	const bool Continue = ContMPCFloat || ContMPCVec || ContData
		|| ContDynFloat || ContDynVector || ContSndFloat || ContComp || ContGen;

	if (LIKELY(Continue)) return;

	IsFading = false;
	OnDone.Broadcast();
	UE_LOG(LogAnim, Verbose, TEXT("%hs Tick Done"), __func__);
}

template <typename Type>
bool UAnim::ItemTick(const float DT, TArray<Type>& IOArr, void(UAnim::* Done)(const Type&)) {
	UE_LOG(LogAnim, Verbose, TEXT("%hs"), __func__);
	TArray<int32> ToRemove;
	bool Cont = false;
	// traversing in reverse to remove on the spot
	for (int32 i = IOArr.Num()-1; i>=0; --i) {
		Type& Par = IOArr[i];
		const bool IsDone = Par.Tick(DT);
		
		if (UNLIKELY(!IsDone)) {
			Cont = true;
			continue; // continue instead of return because we need to process all items
		}

		// important to clone the values, since this var is by ref, once remove is called the data is bogus.
		Type Old = Par;
		IOArr.RemoveAtSwap(i);
		// only at end, to ensure the val is set.
		if (LIKELY(Done))
			(this->*Done)(Old);
		// this is kind of dangerous. since someone could as side effect decide to fade another (or same) data again
		// but since we are looping backwards using classic style loop (proof that is not obsolete) then we are fine
		// since new elements would be added at the end of the array, which would be the current index.
	}

	return Cont;
}

template<typename Type>
void UAnim::ItemsEmpty(TArray<Type>& IOArr, void(UAnim::* Done)(const Type&)) {
	TArray<Type> Copy = IOArr;
	IOArr.Empty(); // empty before notifying.
	if (UNLIKELY(!Done)) return;

	for (const Type& D: Copy)
		(this->*Done)(D);
}

template<typename Type>
void UAnim::ItemsRemoveSame(const Type& Item, TArray<Type>& IOArr) {
	// ensure we remove it the ones colliding. allow to remove more than 1.
	for (int32 i = IOArr.Num()-1; i>=0; --i) {
		const Type& O = IOArr[i];
		if (LIKELY(!Item.IsSame(O))) continue;
		IOArr.RemoveAtSwap(i);
	}
}

template <typename Type>
bool UAnim::ItemsSetNow(const Type& Item, void(UAnim::* Done)(const Type&)) {
	if (UNLIKELY(!FMath::IsNearlyZero(Item.Duration))) return false;

	Item.SetVal(Item.To);
	(this->*Done)(Item);
	return true;
}

template <typename Item>
bool UAnim::ItemIsIn(const UObject* const Obj, const FName Name, const TArray<Item>& IArr) const {
	for (const Item& I: IArr)
		if (I.IsSame(Obj, Name)) return true;
	return false;
}

bool UAnim::GetIsFadingMPC(
const UMaterialParameterCollectionInstance* const MPCI, const FName Name) const {
	if (UNLIKELY(!IsValid(MPCI))) return false;
	if (ItemIsIn(MPCI, Name, ItemsMPCF)) return true;
	if (ItemIsIn(MPCI, Name, ItemsMPCV)) return true;
	return false;
}

bool UAnim::GetIsFadingDyn(const UMaterialInstanceDynamic* const Mat, const FName Name) const {
	if (UNLIKELY(!IsValid(Mat))) return false;
	if (ItemIsIn(Mat, Name, ItemsDynF)) return true;
	if (ItemIsIn(Mat, Name, ItemsDynV)) return true;
	return false;
}

bool UAnim::GetIsFadingData(const UPrimitiveComponent* const Comp, const int32 Index) const {
	if (UNLIKELY(!IsValid(Comp))) return false;

	const FName Name(FString::Printf(TEXT("%i"), Index));
	if (ItemIsIn(Comp, Name, ItemsData)) return true;
	return false;
}

bool UAnim::GetIsFadingSound(const UAudioComponent* const Comp, const FName Name) const {
	if (UNLIKELY(!IsValid(Comp))) return false;
	if (ItemIsIn(Comp, Name, ItemsSndF)) return true;
	return false;
}

bool UAnim::GetIsFadingComp(const USceneComponent* const Comp) const {
	if (UNLIKELY(!IsValid(Comp))) return false;
	static const FName Name = NAME_Transform;
	if (ItemIsIn(Comp, Name, ItemsCompT)) return true;
	return false;
}

void UAnim::Deinitialize() {
	ItemsEmpty(ItemsMPCF, &UAnim::ItemDoneMPCF);
	ItemsEmpty(ItemsMPCV, &UAnim::ItemDoneMPCV);
	ItemsEmpty(ItemsData, &UAnim::ItemDoneData);
	ItemsEmpty(ItemsDynF, &UAnim::ItemDoneDynF);
	ItemsEmpty(ItemsDynV, &UAnim::ItemDoneDynV);
	ItemsEmpty(ItemsSndF, &UAnim::ItemDoneSndF);
	ItemsEmpty(ItemsCompT, &UAnim::ItemDoneComp);
	IsFading = false;
	Super::Deinitialize();
}

void UAnim::Initialize(FSubsystemCollectionBase& Collection) {
	Super::Initialize(Collection);
}

bool UAnim::ShouldCreateSubsystem(UObject* const Outer) const {
	if (!FSlateApplication::IsInitialized()) return false; // this requires the Slate dependency on Bulid.cs

	UE_LOG(LogAnim, Log, TEXT("%hs is=%i."),
		__func__, ShouldBeCreated);

	if (!ShouldBeCreated) {
		UE_LOG(LogAnim, Log, TEXT("%hs is false. The world subsystem will not be created."
			"Can be changed on the config file Interact.ini"), __func__);
		return false;
	}

	return Super::ShouldCreateSubsystem(Outer);
}

bool UAnim::DoesSupportWorldType(const EWorldType::Type WorldType) const {
	// The world subsystem shouldn't be used in the editor. from enhanced input system
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

// without this it will crash. yes. it will crash. https://forums.unrealengine.com/t/how-can-i-tick-a-tickableworldsubsystem/489697/3
// https://benui.ca/unreal/tickable-object/
TStatId UAnim::GetStatId() const {
	// another way RETURN_QUICK_DECLARE_CYCLE_STAT( FMyTickableThing, STATGROUP_Tickables );
	return GetStatID();
}

// thought on using operator== for removing. which looks more "chic".
// but the code is much complex, quite probably slower, and forces me to have the "type" in the struct.
// and do nasty checks. besides "==" is confusing in case you expect that it would also check if the target value is the same, which it wont.
// once again the classic 20 years old for loop is still the best option. remember that some things just get better with time.
// bool FMPFBase::operator==(const FMPFBase& Other) const {
// return Type == Other.Type && Name == Other.Name && MPCI == Other.MPCI;
// }
// needed for overloading operator== and casting.
// FMPFBase is NOT redundant, is needed
// UPROPERTY(BlueprintReadOnly, Transient)
// UScriptStruct* Type = FMPFBase::StaticStruct();
// FMPFData():Super() {
// HAS to be inside here. define in the struct body and it won't work.
// Type = FMPFData::StaticStruct();
// }

// bool FMPFData::operator==(const FMPFBase& Other) const {
// Cast<> doesn't work on structs. we need to use C cast. but that can crash.
// https://forums.unrealengine.com/t/is-there-an-equivalent-of-casting-for-structs-value-types/62984
// if (Type != Other.Type) {
// UE_LOG(LogTemp, Warning, TEXT(" Structs of different classes"));
// return false; // crash aversion
// }
// const FMPFData* const D = (FMPFData*)(&Other);
// comparing all the params (including mpc) in case we are comparing bogus data.
// return FMPFBase::operator==(Other) && Index == D->Index && Comp == D->Comp;
// }

/*
// do not use. returns the index from the name but only works on dynamic materials which this system is not for.
// int32 GetDynamicIndex() const;

const int32 FAMData::GetDynamicIndex() const {
	if (!IsValid(Comp)) {
		UE_LOG(LogTemp, Warning, TEXT("%hs Invalid component"), __func__);
		return INDEX_NONE;
	}

	return IsScalar ?
		Comp->GetCustomPrimitiveDataIndexForScalarParameter(Name):
		Comp->GetCustomPrimitiveDataIndexForScalarParameter(Name);
}
*/
