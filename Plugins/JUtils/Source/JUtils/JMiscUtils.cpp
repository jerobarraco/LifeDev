#include "JMiscUtils.h"
#include "CoreGlobals.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"

#ifdef WITH_EDITOR
	#include "Editor.h"
	#include "EditorScriptingHelpers.h"
#else
	#include "Engine/Engine.h"
#endif

// UJMiscUtils* UJMiscUtils::Get() {
	// return nullptr;
// }

UWorld* UJMiscUtils::GetEdWorld()
{
#if WITH_EDITOR
	if (!GEditor) return nullptr;
	if (EditorScriptingHelpers::CheckIfInEditorAndPIE()) { // TODO
		FWorldContext* const PieContext = GEditor->GetPIEWorldContext(0);
		return PieContext ? PieContext->World() : nullptr;
	}
	return GEditor->GetEditorWorldContext(false).World(); 
#else
	return nullptr;
#endif
}

bool UJMiscUtils::IsWithEditor()
{
#if WITH_EDITOR
	return true;
#else
	return false;
#endif
}

UWorld* UJMiscUtils::JGetWorld(UWorld* World) {
	if (IsValid(World)) return World;

	// World = GetWorld();
	// if (IsValid(World)) return World;

	// TODO this is not working as expected.
	#ifdef WITH_EDITOR
		World = GetEdWorld();
		
		if (!World) {
			World = GEngine->GetWorld();	
		}
		return World;
    #else
    	return GEngine->GetWorld();
    #endif
}

void UJMiscUtils::ToggleMapping(UInputMappingContext* Ctx, int32 Prio, bool Enable, UWorld* World) {
	// TODO this is failing.
	World = JGetWorld(World);
	if (!IsValid(World)) return;
	APlayerController* Controller = World->GetFirstPlayerController();
	if (!IsValid(Controller)) return;
	
	UEnhancedInputLocalPlayerSubsystem* const Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
			Controller->GetLocalPlayer()
	);
	if (!IsValid(Subsystem)) return;

	if (Enable) {
		Subsystem->AddMappingContext(Ctx, Prio);
	} else {
		Subsystem->RemoveMappingContext(Ctx);
	}
}
