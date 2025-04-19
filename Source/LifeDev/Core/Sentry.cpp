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

void USentry::SendComment(const FString& FB) {
	if (UNLIKELY(!IsValid(Sub))) return;
	static FString ID("Feedback");

	Sub->CaptureUserFeedbackWithParams(ID, "", FB, UJUtilsSys::GetUserName());
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
	// TODO get subsystems and bind
}
void USentry::GameDeInit() {
	ULSettings* const Settings = ULSettings::Instance(this);
	if (LIKELY(Settings)) Settings->OnFeatUpdate.RemoveAll(this);
}
