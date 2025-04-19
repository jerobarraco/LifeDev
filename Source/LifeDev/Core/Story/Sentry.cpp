#include "Sentry.h"

#include "SentrySubsystem.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogSentry, Log, Log);

USentry* USentry::Instance(const UObject* const O) {
	if (UNLIKELY(!IsValid(O))) return nullptr;
	const UGameInstance* const Instance = UGameplayStatics::GetGameInstance(O);
	if (UNLIKELY(!IsValid(Instance))) return nullptr;

	return Instance->GetSubsystem<USentry>();
}

void USentry::InstInit() {
	Sub = GEngine->GetEngineSubsystem<USentrySubsystem>();
	UE_CLOG(!IsValid(Sub), LogSentry, Warning, TEXT("%hs SEntry subsystem can't be found"));
}
