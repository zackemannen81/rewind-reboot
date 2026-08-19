#include "RewindGameMode.h"

#include "RewindWorldStateSubsystem.h"

void ARewindGameMode::StartPlay()
{
	if (UWorld* World = GetWorld())
	{
		if (URewindWorldStateSubsystem* WorldState = World->GetSubsystem<URewindWorldStateSubsystem>())
		{
			WorldState->EnsureAuthoredSpace();
		}
	}
	Super::StartPlay();
}

void ARewindGameMode::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);
	if (UWorld* World = GetWorld())
	{
		if (URewindWorldStateSubsystem* WorldState = World->GetSubsystem<URewindWorldStateSubsystem>())
		{
			WorldState->PlacePlayerBody();
		}
	}
}
