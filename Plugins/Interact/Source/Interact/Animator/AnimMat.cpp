#include "AnimMat.h"

#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"

DEFINE_LOG_CATEGORY_STATIC(LogAnimMat, Log, Log);

// https://dev.epicgames.com/documentation/en-us/unreal-engine/storing-custom-data-in-unreal-engine-materials-per-primitive

void FAMBase::AddDT(const float DT, float& Prog) {
	// clamp to perfect duration, to avoid overshooting.
	Elapsed = FMath::Min(Elapsed + DT,Duration);
	if (Duration == 0) return; // not nearly zero needed. is just for the division below.

	const float RProg = Elapsed / Duration;
	Prog = IsValid(Curve) ? Curve->GetFloatValue(RProg) : RProg;
}

bool FAMBase::Tick(const float DT) {
	if (!FIsValid()) return true;

	float Prog;
	AddDT(DT, Prog);
	SetLerp(Prog);
	return IsDone();
}

bool FAMBase::FIsValid() const { return IsValid(MPCI); } // avoid including the type on header

bool FAMFloat::SetVal(const float Val) const {
	UE_LOG(LogAnimMat, Verbose, TEXT("%hs Name=%s Val=%.4f"),
		__func__, *Name.ToString(), Val);
	if (!FIsValid()) return false;

	return MPCI->SetScalarParameterValue(Name, Val);
}

bool FAMFloat::SetLerp(const float Prog) {
	const float Val = FMath::LerpStable(From, To, Prog);
	return SetVal(Val);
}

bool FMPFVector::SetVal(const FLinearColor& Val) const {
	UE_LOG(LogAnimMat, Verbose, TEXT("%hs Name=%s Val=%s"),
			__func__, *Name.ToString(), *Val.ToString());
	if (!FIsValid()) return false;

	return MPCI->SetVectorParameterValue(Name, Val);
}

bool FMPFVector::SetLerp(const float Prog) {
	const FLinearColor Val = UseHSV ?
		FLinearColor::LerpUsingHSV(From, To, Prog) :
		FMath::LerpStable(From, To, Prog);
	return SetVal(Val);
}

const int32 FMPFData::GetDynamicIndex() const {
	if (!IsValid(Comp)) {
		UE_LOG(LogTemp, Warning, TEXT("%hs Invalid component"), __func__);
		return INDEX_NONE;
	}

	return IsScalar ?
		Comp->GetCustomPrimitiveDataIndexForScalarParameter(Name):
		Comp->GetCustomPrimitiveDataIndexForScalarParameter(Name);
}

bool FMPFData::GetCurrent(FLinearColor& OCurrent) const {
	OCurrent = FLinearColor::Black; // initialize to a sane value

	if (!FIsValid()) {
		UE_LOG(LogAnimMat, Warning, TEXT("%hs Invalid component or index. Stop"),
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

	UE_LOG(LogAnimMat, Log, TEXT("%hs Name=%s Current=%s Index=%i Scalar=%i"),
		__func__, *GetNameSafe(Comp), *OCurrent.ToString(), Index, IsScalar);

	return true;
}

bool FMPFData::SetVal(const FLinearColor& V) const {
	UE_LOG(LogAnimMat, Verbose, TEXT("%hs Name=%s Val=%s Index=%i Scalar=%i"),
			__func__, *GetNameSafe(Comp), *V.ToString(), Index, IsScalar);

	if (!FIsValid()) {
		UE_LOG(LogAnimMat, Log, TEXT("%hs Invalid component or index. Skip"),
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

bool FMPFData::SetLerp(const float Prog) {
	const FLinearColor Val = UseHSV ?
		FLinearColor::LerpUsingHSV(From, To, Prog) :
		FMath::LerpStable(From, To, Prog);
	return SetVal(Val);
}

UAnimMat::UAnimMat():Super() {}

UAnimMat* UAnimMat::Instance(UObject* O) {
	if (!IsValid(O)) return nullptr;

	const UWorld* const W = O->GetWorld();
	if (!W) return nullptr;

	UAnimMat* const AnimMat = W->GetSubsystem<UAnimMat>();
	return IsValid(AnimMat) ? AnimMat : nullptr;
}

bool UAnimMat::ParamInitBasic(FAMBase& OParam, const FName Name, UCurveFloat* const Curve,
	const float Duration) const {
	UE_LOG(LogAnimMat, Log, TEXT("%hs name=%s, duration=%.3f"),
		__func__, *Name.ToString(), Duration);

	OParam.Name = Name;
	OParam.Curve = IsValid(Curve) ? Curve : nullptr;
	OParam.Elapsed = 0.0; // reset in case it was running
	OParam.Duration = Duration < 0 ? DurationDefault : Duration;

	// at end to allow for data params
	if (OParam.Name.IsNone()) {
		UE_LOG(LogAnimMat, Warning,
			TEXT("%hs Name can't be none (Unless it's a custom primitive data fade). Stop."),
			__func__);
		return false;
	}

	return true;
}

bool UAnimMat::ParamInitMPC(const UMaterialParameterCollection* const MPC, const FName Name,
	FAMBase& OParam, UCurveFloat* const Curve, const float Duration) const {
	UE_LOG(LogAnimMat, Log, TEXT("%hs name=%s, duration=%.3f"),
		__func__, *Name.ToString(), Duration);

	// these are done at the beginning so that even after returning it's partially valid
	if (!ParamInitBasic(OParam, Name, Curve, Duration)) return false;
	
	const UWorld* const World = GetWorld();
	if (!World) return false;
	
	if (!IsValid(MPC)) {
		UE_LOG(LogAnimMat, Warning, TEXT("%hs Invalid mpc. Stop."),
			__func__);
		return false;
	}

	OParam.MPCI = World->GetParameterCollectionInstance(MPC);
	if (!IsValid(OParam.MPCI)) {
		UE_LOG(LogAnimMat, Warning, TEXT("%hs Can't get MPC Instance. Stop."),
			__func__);
		return false;
	}

	return true;
}

bool UAnimMat::FloatFade(const UMaterialParameterCollection* const MPC, const FName Name,
	const float To, const float Duration, UCurveFloat* const Curve) {
	UE_LOG(LogAnimMat, Log, TEXT("%hs name=%s, to=%.3f, duration=%.3f"),
		__func__, *Name.ToString(), To, Duration);

	FAMFloat Param;
	Param.To = To;
	if (!ParamInitMPC(MPC, Name, Param, Curve, Duration)) {
		UE_LOG(LogAnimMat, Warning, TEXT("%hs Failed to init param. Stop."),
			__func__);
		return false;
	};
	
	if (FMath::IsNearlyZero(Duration)) {
		if (FloatParams.Contains(Name)) FloatParams.Remove(Name); // ensure we remove it.

		const bool Ok = Param.SetVal(To);
		OnItemDone.Broadcast(Name, INDEX_NONE); // notify AFTER change.
		return Ok;
	}
	
	if (!Param.MPCI->GetScalarParameterValue(Name, Param.From)) {
		UE_LOG(LogAnimMat, Warning, TEXT("%hs Can't get the current value."),
			__func__);
		// we do it anyway.
	}

	FloatParams.Add(Name, MoveTemp(Param));
	IsFading = true;
	return true;
}

bool UAnimMat::VectorFade(const UMaterialParameterCollection* const MPC, const FName Name,
	const FLinearColor& To, const float Duration, bool UseHSV,
	UCurveFloat* const Curve) {
	UE_LOG(LogAnimMat, Log, TEXT("%hs name=%s, to=%s, duration=%.3f, usehsv=%i"),
		__func__, *Name.ToString(), *To.ToString(), Duration, UseHSV);

	FMPFVector Param;
	Param.UseHSV = UseHSV;
	Param.To = To;
	if (!ParamInitMPC(MPC, Name, Param, Curve, Duration)) {
		UE_LOG(LogAnimMat, Warning, TEXT("%hs Failed to init param. Stop."),
			__func__);
		return false;
	}

	if (FMath::IsNearlyZero(Duration)) {
		if (VectorParams.Contains(Name)) VectorParams.Remove(Name); // ensure we remove it
		
		const bool Ok = Param.SetVal(To);
		OnItemDone.Broadcast(Name, INDEX_NONE); // notify AFTER change.
		return Ok;
	}

	if (!Param.MPCI->GetVectorParameterValue(Name, Param.From)) {
		UE_LOG(LogAnimMat, Warning, TEXT("%hs Can't get the current value."),
			__func__);
		// we do it anyway.
	}

	VectorParams.Add(Name, MoveTemp(Param));
	IsFading = true;
	return true;
}

bool UAnimMat::DataFade(UPrimitiveComponent* const Component, const int32 Index, const bool IsScalar,
	const FLinearColor& To, const float Duration, const bool UseHSV, UCurveFloat* const Curve) {

	UE_LOG(LogAnimMat, Log, TEXT("%hs comp=%s, index=%i, scalar=%i, to=%s, duration=%.3f, hsv=%i"),
		__func__, *GetNameSafe(Component), Index, IsScalar, *To.ToString(), Duration, UseHSV);

	FMPFData Param;
	Param.Index = Index;
	Param.UseHSV = UseHSV;
	Param.To = To;
	Param.Comp = Component;
	Param.IsScalar = IsScalar;
	static const FName PrimDataName("PrimData");
	ParamInitBasic(Param, PrimDataName, Curve, Duration);// ignore the name issue (return)

	if (!IsValid(Component)) {
		UE_LOG(LogAnimMat, Warning, TEXT("%hs Component is not valid. Stop."),
			__func__);
		return false;
	}

	if (FMath::IsNearlyZero(Duration)) {
		// if (DataParams.Contains(Name)) DataParams.Remove(Name); // ensure we remove it
		// TODO be able to remove a data param. i need a way to map it. by component and index. (hash?)
		
		const bool Ok = Param.SetVal(Param.To);
		OnItemDone.Broadcast(Param.Name, Param.Index); // notify AFTER change.
		return Ok;
	}

	Param.GetCurrent(Param.From); // ignore return, we'll do it anyway.

	DataParams.Add(MoveTemp(Param));
	IsFading = true;
	return true;
}

void UAnimMat::Tick(const float DeltaTime) {
	Super::Tick(DeltaTime);
	UE_LOG(LogAnimMat, Verbose, TEXT("%hs"), __func__);

	const bool ContFloat = ParamTick(DeltaTime, FloatParams); //FloatTick(DeltaTime);
	const bool ContVec = ParamTick(DeltaTime, VectorParams);
	const bool ContTick = DataTick(DeltaTime);
	// done this way to avoid lazy evaluation to skip vec
	const bool Continue = ContFloat || ContVec || ContTick;
	if (Continue) return;

	UE_LOG(LogAnimMat, Log, TEXT("%hs Done"), __func__);
	IsFading = false;
	OnDone.Broadcast();
}

bool UAnimMat::DataTick(float DT) {
	TArray<int32> ToRemove;
	bool Cont = false;
	// traversing in reverse to remove on the spot
	for (int32 i= DataParams.Num()-1; i>=0; --i) {
		FMPFData& Par = DataParams[i];
		const bool IsDone = Par.Tick(DT);
		
		// only at end, to ensure the val is set.
		if (IsDone) {
			// important to clone the values, since this var is by ref, once remove is called the data is bogus.
			const FName N = Par.Name;
			const int32 I = Par.Index;
			DataParams.RemoveAt(i);
			OnItemDone.Broadcast(N, I); // Notify AFTER remove.
			// this is kind of dangerous. since someone could as side effect decide to fade another (or same) data again
			// but since we are looping backwards using classic style loop (proof that is not obsolete) then we are fine
			// since new elements would be added at the end of the array, which would be the current index.
		} else Cont = true;
	}

	return Cont;
}

template <typename Item>
bool UAnimMat::ParamTick(const float DT, TMap<FName, Item>& IOArr) {
	TArray<FName> ToRemove;
	bool Cont = false;
	// iterate using KV because it's a bit faster.
	for (TTuple<FName, Item> &KV: IOArr) {
		Item &Par = KV.Value;
		FName Name = KV.Key;
		
		const bool IsDone = Par.Tick(DT);
		
		// only at end, to ensure the val is set.
		if (IsDone) ToRemove.Add(Name);
		else Cont = true;
	}

	// removed separately to not affect the previous for iteration.
	// This also notifies all at once after they are set, which is good.
	for (const FName N: ToRemove) RemoveItem(N, IOArr);

	return Cont;
}

template<typename Item>
void UAnimMat::RemoveItem(const FName N, TMap<FName, Item> &IOArr) {
	if (IOArr.Remove(N) <= 0) return; // avoid notifying if we didn't remove anything.
	OnItemDone.Broadcast(N, INDEX_NONE); // notify AFTER change.
}

template <typename Item>
void UAnimMat::EmptyItems(TMap<FName, Item>& IOArr) {
	TArray<FName> Keys;
	IOArr.GetKeys(Keys);
	IOArr.Empty(); // empty before notifying in case, someone adds one as side effect. We already have the keys.
	for (const FName N: Keys) OnItemDone.Broadcast(N, INDEX_NONE);
}

void UAnimMat::Deinitialize() {
	IsFading = false;
	EmptyItems(FloatParams);
	EmptyItems(VectorParams);

	for (const FMPFData& D: DataParams) {
		OnItemDone.Broadcast(D.Name, D.Index);
	}
	DataParams.Empty();
	Super::Deinitialize();
}

void UAnimMat::Initialize(FSubsystemCollectionBase& Collection) {
	Super::Initialize(Collection);
}

bool UAnimMat::ShouldCreateSubsystem(UObject* Outer) const {
	if (!FSlateApplication::IsInitialized()) return false; // this requires the Slate dependency on Bulid.cs

	UE_LOG(LogTemp, Log, TEXT("AnimMat::ShoulbBeCreated is=%i. the world subsystem will not be created."),
		ShouldBeCreated);

	if (!ShouldBeCreated) {
		UE_LOG(LogTemp, Log, TEXT("AnimMat::ShouldBeCreated is false,"
			"the world subsystem will not be created. Can be changed on the config file Interact.ini"));
		return false;
	}

	return Super::ShouldCreateSubsystem(Outer);
}

bool UAnimMat::DoesSupportWorldType(const EWorldType::Type WorldType) const {
	// The world subsystem shouldn't be used in the editor
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

// without this it will crash. yes. it will crash. https://forums.unrealengine.com/t/how-can-i-tick-a-tickableworldsubsystem/489697/3
// https://benui.ca/unreal/tickable-object/
TStatId UAnimMat::GetStatId() const {
	// another way RETURN_QUICK_DECLARE_CYCLE_STAT( FMyTickableThing, STATGROUP_Tickables );
	return GetStatID();
}

