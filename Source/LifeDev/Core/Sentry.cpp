#include "Sentry.h"

#include "SentrySubsystem.h"
#include "JUtils/Misc/JUtilsSys.h"

#include "LifeDev/Core/LGameInstance.h"
#include "Settings/LSettings.h"

DEFINE_LOG_CATEGORY_STATIC(LogSentry, Log, Log);

USentry* USentry::Instance(const UObject* const O) {
	const ULGameInstance* const Instance = ULGameInstance::Instance(O);
	if (UNLIKELY(!IsValid(Instance))) return nullptr;

	return Instance->GetSubsystem<USentry>();
}

void USentry::SendComment(const FString& FB) const {
	if (UNLIKELY(!IsValid(Sub))) return;
	const FString& Id = Sub->CaptureMessage("FEEDBACK!"); // yes, the docs says it needs to be like this.
	Sub->CaptureUserFeedbackWithParams(Id, "", FB, UJUtilsSys::GetUserName());
}

void USentry::TagSet(const FString& Tag, const FString& Val) const {
	if (UNLIKELY(!IsValid(Sub))) return;
	Sub->SetTag(Tag, Val);
}

void USentry::TagRem(const FString& Tag) const {
	if (UNLIKELY(!IsValid(Sub))) return;
	Sub->RemoveTag(Tag);
}

void USentry::InstInit() {
	Sub = GEngine->GetEngineSubsystem<USentrySubsystem>();
	UE_CLOG(!IsValid(Sub), LogSentry, Warning, TEXT("%hs Sentry subsystem can't be found"),
		__func__);
}

void USentry::InstDeInit() {
	Sub = nullptr;
}

void USentry::GameInit() {
	ULSettings* const Settings = ULSettings::Instance(this);
	if (LIKELY(Settings))
		Settings->OnFeatUpdate.AddUniqueDynamic(this, &USentry::FeatUp);
}


void USentry::GameDeInit() {
	ULSettings* const Settings = ULSettings::Instance(this);
	if (LIKELY(Settings)) Settings->OnFeatUpdate.RemoveAll(this);
}

void USentry::FeatUp(const EFeat Feat, const bool Enabled) {
	static const FName TagFeat("Feat");
	const FString& Tag = UEnum::GetValueAsString(Feat);
	if (Enabled) {
		TagSet(Tag, "On");
	}
	else
		TagRem(Tag);
}
