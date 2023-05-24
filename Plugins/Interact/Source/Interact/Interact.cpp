// Copyright Jerónimo Barraco-Mármol

#include "Interact.h"

#include "CInteract.h"

AInteract::AInteract():Super() {
	// super important or it will NOT work
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	SetActorTickEnabled(false); // ensure we don't animate on start
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));

	IRoot = CreateDefaultSubobject<USceneComponent>(TEXT("IRoot"));
	IRoot->SetupAttachment(RootComponent);
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(IRoot);
	Mesh->PrimaryComponentTick.bStartWithTickEnabled = false;
	Mesh->SetComponentTickEnabled(false);
	Mesh->SetGenerateOverlapEvents(false);
	Mesh->SetCollisionProfileName("NoCollision");
	Mesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	Mesh->SetCanEverAffectNavigation(false);

	Interact = CreateDefaultSubobject<UCInteract>(TEXT("Interact"));
	Interact->SetupAttachment(Mesh);
	Interact->SetComponentTickEnabled(false);
	Interact->HoverMesh = Mesh;
}

bool AInteract::TryUseItem_Implementation(const FName& Name) {
	UE_LOG(LogTemp, Log, TEXT("Ainteract.TryUseItem=%s"), *Name.ToString());
	return false;
}

void AInteract::BeginPlay() {
	Super::BeginPlay();
	SetText();
	Interact->OnTrigger.AddUniqueDynamic(this, &AInteract::Trigger);
	Interact->OnHover.AddUniqueDynamic(this, &AInteract::Hover);
}

void AInteract::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	Interact->OnTrigger.RemoveAll(this);
	Interact->OnHover.RemoveAll(this);
	Interact->DeInit();
}

void AInteract::SetText_Implementation() {}

void AInteract::SetInteractAutoBounds() {
	// Mesh->GetLocalBounds();
	// TODO how do i get the mesh size?
	Interact->Bounds = Mesh->GetPlacementExtent();
}

void AInteract::Trigger_Implementation() {
	UE_LOG(LogTemp, Log, TEXT("Actor Triggered"));
	SetText();
}

void AInteract::Hover_Implementation(bool IsOn) {}


/* TODO fix this

LogOutputDevice: Error: === Handled ensure: ===
LogOutputDevice: Error: Ensure condition failed: false  [File:./Runtime/Engine/Private/Components/SceneComponent.cpp] [Line: 1998] 
LogOutputDevice: Error: Template Mismatch during attachment. Attaching instanced component to template component. Parent 'Interact' (Owner 'Default__B_NPC00_C') Self 'PostProcess' (Owner 'B_NPC00_C_0').
LogOutputDevice: Error: Stack: 
LogOutputDevice: Error: [Callstack] 0x00007f046998d1bf libUnrealEditor-Engine.so!USceneComponent::AttachToComponent(USceneComponent*, FAttachmentTransformRules const&, FName) [/home/nande/work/UE5.2/Engine/Source/./Runtime/Engine/Private/Components/SceneComponent.cpp:1998]
LogOutputDevice: Error: [Callstack] 0x00007f0469918754 libUnrealEditor-Engine.so!USceneComponent::OnRegister() [/home/nande/work/UE5.2/Engine/Source/./Runtime/Engine/Private/Components/SceneComponent.cpp:678]
LogOutputDevice: Error: [Callstack] 0x00007f046b0fde99 libUnrealEditor-Engine.so!UPostProcessComponent::OnRegister() [/home/nande/work/UE5.2/Engine/Source/./Runtime/Engine/Private/UnrealClient.cpp:1724]
LogOutputDevice: Error: [Callstack] 0x00007f046980692b libUnrealEditor-Engine.so!UActorComponent::ExecuteRegisterEvents(FRegisterComponentContext*) [/home/nande/work/UE5.2/Engine/Source/./Runtime/Engine/Private/Components/ActorComponent.cpp:1679]
LogOutputDevice: Error: [Callstack] 0x00007f0469813b24 libUnrealEditor-Engine.so!UActorComponent::RegisterComponentWithWorld(UWorld*, FRegisterComponentContext*) [/home/nande/work/UE5.2/Engine/Source/./Runtime/Engine/Private/Components/ActorComponent.cpp:1349]
LogOutputDevice: Error: [Callstack] 0x00007f046920d5c1 libUnrealEditor-Engine.so!AActor::IncrementalRegisterComponents(int, FRegisterComponentContext*) [/home/nande/work/UE5.2/Engine/Source/./Runtime/Engine/Private/Actor.cpp:5378]
LogOutputDevice: Error: [Callstack] 0x00007f046920cb38 libUnrealEditor-Engine.so!AActor::RegisterAllComponents() [/home/nande/work/UE5.2/Engine/Source/./Runtime/Engine/Private/Actor.cpp:5277]
LogOutputDevice: Error: [Callstack] 0x00007f04691f41a3 libUnrealEditor-Engine.so!AActor::PostSpawnInitialize(UE::Math::TTransform<double> const&, AActor*, APawn*, bool, bool, bool, ESpawnActorScaleMethod) [/home/nande/work/UE5.2/Engine/Source/./Runtime/Engine/Private/Actor.cpp:3713]
LogOutputDevice: Error: [Callstack] 0x00007f046a0b9ce7 libUnrealEditor-Engine.so!UWorld::SpawnActor(UClass*, UE::Math::TTransform<double> const*, FActorSpawnParameters const&) [/home/nande/work/UE5.2/Engine/Source/./Runtime/Engine/Private/LevelActor.cpp:706]
LogOutputDevice: Error: [Callstack] 0x00007f046a0ba58b libUnrealEditor-Engine.so!UWorld::SpawnActor(UClass*, UE::Math::TVector<double> const*, UE::Math::TRotator<double> const*, FActorSpawnParameters const&) [/home/nande/work/UE5.2/Engine/Source/./Runtime/Engine/Private/LevelActor.cpp:424]
LogOutputDevice: Error: [Callstack] 0x00007f045eff3986 libUnrealEditor-Kismet.so!FBlueprintEditor::UpdatePreviewActor(UBlueprint*, bool) [/home/nande/work/UE5.2/Engine/Source/./Editor/Kismet/Private/BlueprintEditor.cpp:10098]
LogOutputDevice: Error: [Callstack] 0x00007f045eff34d7 libUnrealEditor-Kismet.so!FBlueprintEditor::Tick(float) [/home/nande/work/UE5.2/Engine/Source/./Editor/Kismet/Private/BlueprintEditor.cpp:9103]
LogOutputDevice: Error: [Callstack] 0x00007f0463ce82b1 libUnrealEditor-UnrealEd.so!FTickableEditorObject::TickObjects(float) [/home/nande/work/UE5.2/Engine/Source/Editor/UnrealEd/Public/TickableEditorObject.h:49]
LogOutputDevice: Error: [Callstack] 0x00007f0463caae92 libUnrealEditor-UnrealEd.so!UEditorEngine::Tick(float, bool) [/home/nande/work/UE5.2/Engine/Source/./Editor/UnrealEd/Private/EditorEngine.cpp:1660]
LogOutputDevice: Error: [Callstack] 0x00007f046487a172 libUnrealEditor-UnrealEd.so!UUnrealEdEngine::Tick(float, bool) [/home/nande/work/UE5.2/Engine/Source/./Editor/UnrealEd/Private/UnrealEdEngine.cpp:516]
LogOutputDevice: Error: [Callstack] 0x000000000023d476 UnrealEditor-Linux-DebugGame!FEngineLoop::Tick() [/home/nande/work/UE5.2/Engine/Source/./Runtime/Launch/Private/LaunchEngineLoop.cpp:5806]
LogOutputDevice: Error: [Callstack] 0x000000000024a24a UnrealEditor-Linux-DebugGame!GuardedMain(char16_t const*) [/home/nande/work/UE5.2/Engine/Source/./Runtime/Launch/Private/Launch.cpp:190]
LogOutputDevice: Error: [Callstack] 0x00007f04625cf9ba libUnrealEditor-UnixCommonStartup.so!CommonUnixMain(int, char**, int (*)(char16_t const*), void (*)()) [/home/nande/work/UE5.2/Engine/Source/Runtime/Unix/UnixCommonStartup/Private/UnixCommonStartup.cpp:269]
LogOutputDevice: Error: [Callstack] 0x00007f04622e0d90 libc.so.6!UnknownFunction(0x29d8f)
LogOutputDevice: Error: [Callstack] 0x00007f04622e0e40 libc.so.6!__libc_start_main(+0x7f)
LogOutputDevice: Error: [Callstack] 0x0000000000236789 UnrealEditor-Linux-DebugGame!_start()
*/