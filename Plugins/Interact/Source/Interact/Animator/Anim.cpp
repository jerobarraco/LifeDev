#include "Anim.h"

#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"

DEFINE_LOG_CATEGORY_STATIC(LogAnim, Log, Log);

// https://dev.epicgames.com/documentation/en-us/unreal-engine/storing-custom-data-in-unreal-engine-materials-per-primitive

void FABase::AddDT(const float DT, float& Prog) {
	// clamp to perfect duration, to avoid overshooting.
	Elapsed = FMath::Min(Elapsed + DT,Duration);
	if (Duration == 0) return; // not nearly zero needed. is just for the division below.

	const float RProg = Elapsed / Duration;
	Prog = IsValid(Curve) ? Curve->GetFloatValue(RProg) : RProg;
}

bool FABase::Tick(const float DT) {
	if (UNLIKELY(!FIsValid())) return true;

	float Prog;
	AddDT(DT, Prog);
	SetLerp(Prog);
	return IsDone();
}

// define here to avoid including the type on header
bool FABase::FIsValid() const { return !Name.IsNone() && IsValid(Obj); }
// bool FADFloat::FIsValid() const { return !Name.IsNone() && IsValid(Obj); }
// bool FASFloat::FIsValid() const {}
// bool FADVector::FIsValid() const { return !Name.IsNone() && IsValid(Obj); }

bool FAPFloat::SetVal(const float Val) const {
	UE_LOG(LogAnim, Verbose, TEXT("%hs Name=%s Val=%.4f"),
		__func__, *Name.ToString(), Val);
	if (UNLIKELY(!FIsValid())) return false;

	return MPCI->SetScalarParameterValue(Name, Val);
}

bool FAPFloat::SetLerp(const float Prog) {
	const float Val = FMath::LerpStable(From, To, Prog);
	return SetVal(Val);
}

bool FADFloat::SetVal(const float Val) const {
	UE_LOG(LogAnim, Verbose, TEXT("%hs Name=%s Val=%.4f"),
		__func__, *Name.ToString(), Val);
	if (UNLIKELY(!FIsValid())) return false;

	UMaterialInstanceDynamic* const MM = Cast<UMaterialInstanceDynamic>(Obj);
	if (UNLIKELY(!MM)) return false;
	MM->SetScalarParameterValue(Name, Val);
	return true;
}

bool FADVector::SetVal(const FLinearColor& Val) const {
	UE_LOG(LogAnim, Verbose, TEXT("%hs Name=%s Val=%s"),
			__func__, *Name.ToString(), *Val.ToString());
	if (UNLIKELY(!FIsValid())) return false;

	UMaterialInstanceDynamic* const MM = Cast<UMaterialInstanceDynamic>(Obj);
	if (UNLIKELY(!MM)) return false;
	MM->SetVectorParameterValue(Name, Val);
	// Mat->SetVectorParameterValue(Name, Val);
	return true;
}

bool FAPVector::SetVal(const FLinearColor& Val) const {
	UE_LOG(LogAnim, Verbose, TEXT("%hs Name=%s Val=%s"),
			__func__, *Name.ToString(), *Val.ToString());
	if (UNLIKELY(!FIsValid())) return false;

	return MPCI->SetVectorParameterValue(Name, Val);
}

bool FAPVector::SetLerp(const float Prog) {
	const FLinearColor& Val = UseHSV ?
		FLinearColor::LerpUsingHSV(From, To, Prog) :
		FMath::LerpStable(From, To, Prog);
	return SetVal(Val);
}

bool FAData::GetCurrent(FLinearColor& OCurrent) const {
	OCurrent = FLinearColor::Black; // initialize to a sane value

	if (!FIsValid()) {
		UE_LOG(LogAnim, Warning, TEXT("%hs Invalid component or index. Stop"),
			__func__);
		return false;
	}

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

bool FAData::SetVal(const FLinearColor& V) const {
	UE_LOG(LogAnim, Verbose, TEXT("%hs Name=%s Val=%s Index=%i Scalar=%i"),
		__func__, *GetNameSafe(Comp), *V.ToString(), Index, IsScalar);

	if (UNLIKELY(!FIsValid())) {
		UE_LOG(LogAnim, Log, TEXT("%hs Invalid component or index. Skip"),
		__func__);
		return false;
	}
	
	if (IsScalar)
		Comp->SetCustomPrimitiveDataFloat(Index, V.R);
	else
		Comp->SetCustomPrimitiveDataVector4(Index, V);

	// ONLY works on dynamic material which is what we are trying to avoid
	// Comp->SetScalarParameterForCustomPrimitiveData(Name, V.R);
	// Comp->SetVectorParameterForCustomPrimitiveData(Name, V);

	return true;
}

bool FAData::SetLerp(const float Prog) {
	const FLinearColor Val = UseHSV ?
		FLinearColor::LerpUsingHSV(From, To, Prog) :
		FMath::LerpStable(From, To, Prog);
	return SetVal(Val);
}

UAnim::UAnim():Super() {}

UAnim* UAnim::Instance(const UObject*const  O) {
	if (UNLIKELY(!IsValid(O))) return nullptr;

	const UWorld* const W = O->GetWorld();
	if (UNLIKELY(!W)) return nullptr;

	UAnim* const AnimMat = W->GetSubsystem<UAnim>();
	return LIKELY(IsValid(AnimMat)) ? AnimMat : nullptr;
}

bool UAnim::ParamInitBasic(FABase& OParam, const FName Name, UCurveFloat* const Curve,
	const float Duration) const {
	UE_LOG(LogAnim, Log, TEXT("%hs name=%s, duration=%.3f"),
		__func__, *Name.ToString(), Duration);

	OParam.Name = Name;
	OParam.Curve = IsValid(Curve) ? Curve : nullptr;
	OParam.Elapsed = 0.0; // reset in case it was running
	OParam.Duration = Duration < 0 ? DurationDefault : Duration;

	// at end to allow for data params
	if (UNLIKELY(OParam.Name.IsNone())) {
		UE_LOG(LogAnim, Warning,
			TEXT("%hs Name can't be none (Unless it's a custom primitive data fade). Stop."),
			__func__);
		return false;
	}

	return true;
}

bool UAnim::ParamInitMPC(const UMaterialParameterCollection* const MPC, const FName Name,
	FABase& OParam, UCurveFloat* const Curve, const float Duration) const {
	UE_LOG(LogAnim, Log, TEXT("%hs name=%s, duration=%.3f"),
		__func__, *Name.ToString(), Duration);

	// these are done at the beginning so that even after returning it's partially valid
	if (UNLIKELY(!ParamInitBasic(OParam, Name, Curve, Duration))) return false;
	
	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return false;
	
	if (UNLIKELY(!IsValid(MPC))) {
		UE_LOG(LogAnim, Warning, TEXT("%hs Invalid mpc. Stop."),
			__func__);
		return false;
	}

	OParam.MPCI = World->GetParameterCollectionInstance(MPC);
	OParam.Obj = OParam.MPCI;
	if (UNLIKELY(!IsValid(OParam.MPCI))) {
		UE_LOG(LogAnim, Warning, TEXT("%hs Can't get MPC Instance. Stop."),
			__func__);
		return false;
	}

	return true;
}

bool UAnim::ParamInitDyn(UMaterialInstanceDynamic* const Mat, const FName Name, FABase& OParam,
	UCurveFloat* const Curve, const float Duration) const {
	UE_LOG(LogAnim, Log, TEXT("%hs name=%s, duration=%.3f"),
		__func__, *Name.ToString(), Duration);

	// these are done at the beginning so that even after returning it's partially valid
	if (UNLIKELY(!ParamInitBasic(OParam, Name, Curve, Duration))) return false;

	if (UNLIKELY(!IsValid(Mat))) {
		UE_LOG(LogAnim, Warning, TEXT("%hs Invalid mat. Stop."),
			__func__);
		return false;
	}
	OParam.Obj = Mat;
	OParam.Mat = Mat;
	return true;
}

bool UAnim::MPCFloatFade(const UMaterialParameterCollection* const MPC, const FName Name,
	const float To, const float Duration, UCurveFloat* const Curve) {
	UE_LOG(LogAnim, Log, TEXT("%hs name=%s, to=%.3f, duration=%.3f"),
		__func__, *Name.ToString(), To, Duration);

	FAPFloat Param;
	if (UNLIKELY(!ParamInitMPC(MPC, Name, Param, Curve, Duration))) {
		UE_LOG(LogAnim, Warning, TEXT("%hs Failed to init param. Stop."),
			__func__);
		return false;
	}
	
	Param.To = To;
	// ensure we remove it the ones colliding. allow to remove more than 1.
	for (int32 i = MPCFloatParams.Num()-1; i>=0; --i) {
		const FAPFloat& O = MPCFloatParams[i];
		if (LIKELY(Param.MPCI != O.MPCI || Param.Name != O.Name)) continue;
		MPCFloatParams.RemoveAtSwap(i);
	}

	if (FMath::IsNearlyZero(Param.Duration)) {
		const bool Ok = Param.SetVal(To);
		ItemDoneMPCF(Param);
		return Ok;
	}

	const bool Got = !Param.MPCI->GetScalarParameterValue(Name, Param.From);
	UE_CLOG(UNLIKELY(Got), LogAnim, Warning, TEXT("%hs Can't get the current value."),
		__func__); // we do it anyway.

	MPCFloatParams.Add(MoveTemp(Param));
	IsFading = true;
	return true;
}

bool UAnim::VectorFade(const UMaterialParameterCollection* const MPC, const FName Name,
const FLinearColor& To, const float Duration, const bool UseHSV,
	UCurveFloat* const Curve) {
	UE_LOG(LogAnim, Log, TEXT("%hs name=%s, to=%s, duration=%.3f, usehsv=%i"),
		__func__, *Name.ToString(), *To.ToString(), Duration, UseHSV);

	FAPVector Param;
	if (UNLIKELY(!ParamInitMPC(MPC, Name, Param, Curve, Duration))) {
		UE_LOG(LogAnim, Warning, TEXT("%hs Failed to init param. Stop."),
			__func__);
		return false;
	}

	Param.UseHSV = UseHSV;
	Param.To = To;
	for (int32 i = MPCVectorParams.Num()-1; i>=0; --i) {
		const FAPVector& O = MPCVectorParams[i];
		if (LIKELY(Param.MPCI != O.MPCI || Param.Name != O.Name)) continue;
		MPCVectorParams.RemoveAtSwap(i);
	}
	
	if (FMath::IsNearlyZero(Param.Duration)) {
		const bool Ok = Param.SetVal(To);
		ItemDoneMPCV(Param); // notify AFTER change.
		return Ok;
	}

	// we do it anyway.
	const bool Got = Param.MPCI->GetVectorParameterValue(Name, Param.From);
	UE_CLOG(UNLIKELY(!Got), LogAnim, Warning, TEXT("%hs Can't get the current value."),
		__func__);

	MPCVectorParams.Add(MoveTemp(Param));
	IsFading = true;
	return true;
}

bool UAnim::DynFloatFade(UMaterialInstanceDynamic* const Mat, const FName Name, const float To,
const float Duration, UCurveFloat* const Curve) {
	UE_LOG(LogAnim, Log, TEXT("%hs name=%s, to=%.3f, duration=%.3f"),
		__func__, *Name.ToString(), To, Duration);

	FADFloat Param;
	if (UNLIKELY(!ParamInitDyn(Mat, Name, Param, Curve, Duration))) {
		UE_LOG(LogAnim, Warning, TEXT("%hs Failed to init param. Stop."),
			__func__);
		return false;
	}
	
	Param.To = To;
	// ensure we remove it the ones colliding. allow to remove more than 1.
	for (int32 i = DynFloatParams.Num()-1; i>=0; --i) {
		const FADFloat& O = DynFloatParams[i];
		if (LIKELY(Param.Mat != O.Mat || Param.Name != O.Name)) continue;
		DynFloatParams.RemoveAtSwap(i);
	}

	if (FMath::IsNearlyZero(Param.Duration)) {
		const bool Ok = Param.SetVal(To);
		ItemDoneDynF(Param);
		return Ok;
	}
	
	const bool Got = Param.Mat->GetScalarParameterValue(Name, Param.From);
	UE_CLOG(UNLIKELY(!Got), LogAnim, Warning, TEXT("%hs Can't get the current value."),
		__func__); // we do it anyway.

	DynFloatParams.Add(MoveTemp(Param));
	IsFading = true;
	return true;
}

bool UAnim::DynVectorFade(UMaterialInstanceDynamic* const Mat, const FName Name, const FLinearColor& To,
const float Duration, const bool UseHSV, UCurveFloat* const Curve) {
	UE_LOG(LogAnim, Log, TEXT("%hs name=%s, to=%s, duration=%.3f, usehsv=%i"),
		__func__, *Name.ToString(), *To.ToString(), Duration, UseHSV);

	FADVector Param;
	if (UNLIKELY(!ParamInitDyn(Mat, Name, Param, Curve, Duration))) {
		UE_LOG(LogAnim, Warning, TEXT("%hs Failed to init param. Stop."),
			__func__);
		return false;
	}

	Param.UseHSV = UseHSV;
	Param.To = To;

	for (int32 i = DynVectorParams.Num()-1; i>=0; --i) {
		const FAPVector& O = DynVectorParams[i];
		if (LIKELY(Param.Mat != O.Mat || Param.Name != O.Name)) continue;
		DynVectorParams.RemoveAtSwap(i);
	}
	
	if (FMath::IsNearlyZero(Param.Duration)) {
		const bool Ok = Param.SetVal(To);
		ItemDoneDynV(Param); // notify AFTER change.
		return Ok;
	}

	const bool Got = Param.Mat->GetVectorParameterValue(Name, Param.From); // get the initial value.
	UE_CLOG(UNLIKELY(!Got), LogAnim, Warning, TEXT("%hs Can't get the current value."),
		__func__); // we do it anyway if not got
	
	DynVectorParams.Add(MoveTemp(Param));
	IsFading = true;
	return true;
}

bool UAnim::DataFade(UPrimitiveComponent* const Component, const int32 Index, const bool IsScalar,
	const FLinearColor& To, const float Duration, const bool UseHSV, UCurveFloat* const Curve) {

	UE_LOG(LogAnim, Log, TEXT("%hs comp=%s, index=%i, scalar=%i, to=%s, duration=%.3f, hsv=%i"),
		__func__, *GetNameSafe(Component), Index, IsScalar, *To.ToString(), Duration, UseHSV);

	FAData Param;
	Param.Index = Index;
	Param.UseHSV = UseHSV;
	Param.To = To;
	Param.Comp = Component;
	Param.IsScalar = IsScalar;
	static const FName PrimDataName("PrimData");
	ParamInitBasic(Param, PrimDataName, Curve, Duration);// ignore the name issue (return)

	if (UNLIKELY(!IsValid(Component))) {
		UE_LOG(LogAnim, Warning, TEXT("%hs Component is not valid. Stop."),
			__func__);
		return false;
	}

	// Removing using a less performant linear search.
	// Maybe in the future i use a map or smth, but not worthy atm.
	for (int32 i = DataParams.Num()-1; i>=0; i--) {
		const FAData& D = DataParams[i];
		if (D.Comp != Param.Comp || D.Index!=Param.Index) continue;
		DataParams.RemoveAtSwap(i);
	}

	UE_LOG(LogTemp, Log, TEXT(" Param Fade count =%i"), DataParams.Num());
	if (FMath::IsNearlyZero(Param.Duration)) {
		const bool Ok = Param.SetVal(Param.To);
		ItemDoneData(Param);
		return Ok;
	}

	const bool Got = Param.GetCurrent(Param.From); // ignore return, we'll do it anyway.
	UE_CLOG(UNLIKELY(!Got), LogAnim, Warning, TEXT("%hs Can't get the current value."),
		__func__); // we do it anyway if not got

	DataParams.Add(MoveTemp(Param));
	IsFading = true;
	return true;
}

void UAnim::Tick(const float DT) {
	Super::Tick(DT);
	UE_LOG(LogAnim, Verbose, TEXT("%hs"), __func__);

	const bool ContMPCFloat = ItemTick(DT, MPCFloatParams, &UAnim::ItemDoneMPCF);
	const bool ContMPCVec = ItemTick(DT, MPCVectorParams, &UAnim::ItemDoneMPCV);
	const bool ContData = ItemTick(DT, DataParams, &UAnim::ItemDoneData);
	const bool ContDynFloat = ItemTick(DT, DynFloatParams, &UAnim::ItemDoneDynF);
	const bool ContDynVector = ItemTick(DT, DynVectorParams, &UAnim::ItemDoneDynV);
	// done this way to avoid short-circuit to skip vec (though if the compiler is trying to be smart...)
	const bool Continue = ContMPCFloat || ContMPCVec || ContData || ContDynFloat || ContDynVector;

	if (LIKELY(Continue)) return;

	IsFading = false;
	OnDone.Broadcast();
}

template <typename Item>
bool UAnim::ItemTick(const float DT, TArray<Item>& IOArr,
void(UAnim::* Done)(const Item&)) {
	TArray<int32> ToRemove;
	bool Cont = false;
	// traversing in reverse to remove on the spot
	for (int32 i = IOArr.Num()-1; i>=0; --i) {
		Item& Par = IOArr[i];
		const bool IsDone = Par.Tick(DT);
		
		if (UNLIKELY(!IsDone)) {
			Cont = true;
			continue; // continue instead of return because we need to process all items
		}

		// important to clone the values, since this var is by ref, once remove is called the data is bogus.
		Item ParOld = Par;
		IOArr.RemoveAtSwap(i);
		// only at end, to ensure the val is set.
		if (LIKELY(Done))
			(this->*Done)(ParOld);
		// this is kind of dangerous. since someone could as side effect decide to fade another (or same) data again
		// but since we are looping backwards using classic style loop (proof that is not obsolete) then we are fine
		// since new elements would be added at the end of the array, which would be the current index.
	}

	return Cont;
}

template<typename Item>
void UAnim::ItemsEmpty(TArray<Item>& IOArr, void(UAnim::* Done)(const Item&)) {
	TArray<Item> Copy = IOArr;
	DataParams.Empty(); // empty before notifying.
	if (UNLIKELY(!Done)) return;
	
	for (const Item& D: Copy)
		(this->*Done)(D);
}


bool UAnim::GetIsFadingMPC(
	const UMaterialParameterCollectionInstance* const MPCI, const FName Name) const {
	if (UNLIKELY(!IsValid(MPCI))) return false;
	
	for (const FAPFloat& P: MPCFloatParams)
		if (P.Name == Name && MPCI == P.MPCI) return true;
	for (const FAPVector& P: MPCVectorParams)
		if (P.Name == Name && MPCI == P.MPCI) return true;

	return false;
}

bool UAnim::GetIsFadingDyn(const UMaterialInstanceDynamic* const Mat, const FName Name) const {
	if (UNLIKELY(!IsValid(Mat))) return false;
	
	for (const FADFloat& P: DynFloatParams)
		if (P.Name == Name && Mat == P.Mat) return true;
	for (const FADVector& P: DynVectorParams)
		if (P.Name == Name && Mat == P.Mat) return true;

	return false;
}

bool UAnim::GetIsFadingData(const UPrimitiveComponent* const Comp, const int32 Index) const {
	if (UNLIKELY(!IsValid(Comp))) return false;

	for (const FAData& P: DataParams) {
		if (P.Index == Index && (Comp == P.Comp)) return true;
	}
	
	return false;
}

void UAnim::Deinitialize() {
	ItemsEmpty(MPCFloatParams, &UAnim::ItemDoneMPCF);
	ItemsEmpty(MPCVectorParams, &UAnim::ItemDoneMPCV);
	ItemsEmpty(DataParams, &UAnim::ItemDoneData);
	ItemsEmpty(DynFloatParams, &UAnim::ItemDoneDynF);
	ItemsEmpty(DynVectorParams, &UAnim::ItemDoneDynV);
	IsFading = false;
	Super::Deinitialize();
}

void UAnim::Initialize(FSubsystemCollectionBase& Collection) {
	Super::Initialize(Collection);
}

bool UAnim::ShouldCreateSubsystem(UObject* Outer) const {
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

/*
 
// TODO make this to use a ItemDone as ptr
template<typename Item>
void UAnimMat::EmptyItems(TArray<Item>& IOArr) {
	TArray<Item> Copy = IOArr;
	DataParams.Empty(); // empty before notifying.
	for (const Item& D: Copy) {
		ItemDone(D);
	}
}

void UAnimMat::EmptyItemsData(TArray<FAMData>& IOArr) {
	TArray<FAMData> Copy = IOArr;
	DataParams.Empty(); // empty before notifying.
	for (const FAMData& D: Copy) {
		ItemDoneData(D);
	}
}
template <typename Item>
bool UAnimMat::ParamTick(const float DT, TArray<Item>& IOArr) {
	TArray<int32> ToRemove;
	bool Cont = false;
	// traversing in reverse to remove on the spot
	for (int32 i = IOArr.Num()-1; i>=0; --i) {
		Item& Par = IOArr[i];
		const bool IsDone = Par.Tick(DT);
		
		// only at end, to ensure the val is set.
		if (UNLIKELY(!IsDone)) {
			Cont = true;
			continue;
		}

		// important to clone the values, since this var is by ref, once remove is called the data is bogus.
		Item ParOld = Par;
		IOArr.RemoveAtSwap(i);
		ItemDone(ParOld);
		// this is kind of dangerous. since someone could as side effect decide to fade another (or same) data again
		// but since we are looping backwards using classic style loop (proof that is not obsolete) then we are fine
		// since new elements would be added at the end of the array, which would be the current index.
	}

	return Cont;
}
bool UAnimMat::DataTick(float DT) {
	TArray<int32> ToRemove;
	bool Cont = false;
	// traversing in reverse to remove on the spot
	for (int32 i= DataParams.Num()-1; i>=0; --i) {
		FAMData& Par = DataParams[i];
		const bool IsDone = Par.Tick(DT);
		
		// only at end, to ensure the val is set.
		if (UNLIKELY(!IsDone)) {
			Cont = true;
			continue;
		}

		// important to clone the values, since this var is by ref, once remove is called the data is bogus.
		FAMData ParOld = Par;
		DataParams.RemoveAtSwap(i);
		ItemDoneData(ParOld);
		// this is kind of dangerous. since someone could as side effect decide to fade another (or same) data again
		// but since we are looping backwards using classic style loop (proof that is not obsolete) then we are fine
		// since new elements would be added at the end of the array, which would be the current index.
	}

	return Cont;
}
 */
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