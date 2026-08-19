#include "RewindGameMode.h"

#include "RewindCharacter.h"
#include "RewindWorldStateSubsystem.h"

ARewindGameMode::ARewindGameMode()
{
	DefaultPawnClass = ARewindCharacter::StaticClass();
}

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
