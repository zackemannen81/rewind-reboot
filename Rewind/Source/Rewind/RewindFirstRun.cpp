#include "RewindFirstRun.h"

#include "RewindMessageCatalog.h"
#include "RewindMessageQueue.h"
#include "RewindMessageSubsystem.h"
#include "RewindSessionSubsystem.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"

FName RewindFirstRun::SeenFact(FName MessageId)
{
	return FName(*FString::Printf(TEXT("seen.%s"), *MessageId.ToString()));
}

bool RewindFirstRun::ShowOnce(
	TSet<FName>& Knowledge,
	FRewindMessageDirector& Director,
	FName MessageId)
{
	const FName Fact = SeenFact(MessageId);
	if (Knowledge.Contains(Fact))
	{
		return false;
	}
	if (!Director.Show(MessageId))
	{
		return false;
	}
	Knowledge.Add(Fact);
	return true;
}

bool RewindFirstRun::ShowOnce(const UObject* WorldContext, FName MessageId)
{
	if (!WorldContext || !FRewindMessageCatalog::Find(MessageId))
	{
		return false;
	}

	const UWorld* World = WorldContext->GetWorld();
	const UGameInstance* GameInstance = World ? World->GetGameInstance() : nullptr;
	URewindSessionSubsystem* Session =
		GameInstance ? GameInstance->GetSubsystem<URewindSessionSubsystem>() : nullptr;
	URewindMessageSubsystem* Messages = URewindMessageSubsystem::Get(WorldContext);
	if (!Session || !Messages)
	{
		return false;
	}

	const FName Fact = SeenFact(MessageId);
	if (Session->HasKnowledge(Fact))
	{
		return false;
	}

	Session->ObtainKnowledge(Fact);
	Messages->Show(MessageId);
	return true;
}

bool RewindFirstRun::IsStairRegion(FName RegionName)
{
	const FString Name = RegionName.ToString();
	return Name.Contains(TEXT("Stair"))
		&& !Name.Contains(TEXT("Upper"))
		&& !Name.Contains(TEXT("Entrance"));
}
