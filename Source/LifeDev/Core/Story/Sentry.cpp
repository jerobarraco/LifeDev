#include "Sentry.h"

#include "SentrySubsystem.h"

#include "LifeDev/Core/LGameInstance.h"

DEFINE_LOG_CATEGORY_STATIC(LogSentry, Log, Log);

USentry* USentry::Instance(const UObject* const O) {
	const ULGameInstance* const Instance = ULGameInstance::Instance(O);
	if (UNLIKELY(!IsValid(Instance))) return nullptr;

	return Instance->GetSubsystem<USentry>();
}

void USentry::InstInit() {
	Sub = GEngine->GetEngineSubsystem<USentrySubsystem>();
	UE_CLOG(!IsValid(Sub), LogSentry, Warning, TEXT("%hs SEntry subsystem can't be found"));
}

void USentry::InstDeInit() {
	Sub = nullptr;
}
