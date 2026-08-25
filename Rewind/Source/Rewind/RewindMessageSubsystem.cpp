#include "RewindMessageSubsystem.h"

#include "RewindMessageOverlay.h"
#include "Engine/GameViewportClient.h"
#include "Engine/World.h"

URewindMessageSubsystem* URewindMessageSubsystem::Get(const UObject* WorldContext)
{
	if (!WorldContext)
	{
		return nullptr;
	}
	const UWorld* World = WorldContext->GetWorld();
	return World ? World->GetSubsystem<URewindMessageSubsystem>() : nullptr;
}

bool URewindMessageSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	const UWorld* World = Cast<UWorld>(Outer);
	return World && World->IsGameWorld();
}

void URewindMessageSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void URewindMessageSubsystem::Deinitialize()
{
	DestroyOverlay();
	Director.Clear();
	Super::Deinitialize();
}

void URewindMessageSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	EnsureOverlay();
	SyncOverlay();
}

void URewindMessageSubsystem::Tick(float DeltaTime)
{
	Director.Tick(DeltaTime);
	SyncOverlay();
}

TStatId URewindMessageSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(URewindMessageSubsystem, STATGROUP_Tickables);
}

void URewindMessageSubsystem::Show(FName Id)
{
	Director.Show(Id);
	EnsureOverlay();
	SyncOverlay();
}

void URewindMessageSubsystem::Show(FName Id, const FString& Arg0)
{
	Director.Show(Id, Arg0);
	EnsureOverlay();
	SyncOverlay();
}

void URewindMessageSubsystem::ShowTimed(FName Id, float DurationSeconds)
{
	Director.ShowTimed(Id, DurationSeconds);
	EnsureOverlay();
	SyncOverlay();
}

void URewindMessageSubsystem::ShowTimed(FName Id, float DurationSeconds, const FString& Arg0)
{
	Director.ShowTimed(Id, DurationSeconds, Arg0);
	EnsureOverlay();
	SyncOverlay();
}

void URewindMessageSubsystem::ShowSequence(const TArray<FName>& Ids)
{
	Director.ShowSequence(Ids);
	EnsureOverlay();
	SyncOverlay();
}

void URewindMessageSubsystem::Clear()
{
	Director.Clear();
	SyncOverlay();
}

void URewindMessageSubsystem::EnsureOverlay()
{
	if (Overlay.IsValid())
	{
		return;
	}

	UWorld* World = GetWorld();
	UGameViewportClient* Viewport = World ? World->GetGameViewport() : nullptr;
	if (!Viewport)
	{
		return;
	}

	SAssignNew(Overlay, SRewindMessageOverlay);
	Viewport->AddViewportWidgetContent(Overlay.ToSharedRef(), 50);
}

void URewindMessageSubsystem::DestroyOverlay()
{
	if (!Overlay.IsValid())
	{
		return;
	}

	UWorld* World = GetWorld();
	if (UGameViewportClient* Viewport = World ? World->GetGameViewport() : nullptr)
	{
		Viewport->RemoveViewportWidgetContent(Overlay.ToSharedRef());
	}
	Overlay.Reset();
}

void URewindMessageSubsystem::SyncOverlay()
{
	if (Overlay.IsValid())
	{
		Overlay->SetLine(Director.GetActiveText());
	}
}
