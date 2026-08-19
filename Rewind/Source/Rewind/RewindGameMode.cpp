#include "RewindGameMode.h"

#include "RewindWorldStateSubsystem.h"

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
