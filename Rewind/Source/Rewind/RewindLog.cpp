#include "RewindLog.h"

#include "RewindLoopSubsystem.h"
#include "Engine/World.h"

DEFINE_LOG_CATEGORY(LogRewind);

namespace RewindLog
{
	double ElapsedSeconds(const UObject* WorldContext)
	{
		if (!WorldContext)
		{
			return -1.0;
		}
		if (UWorld* World = WorldContext->GetWorld())
		{
			if (const URewindLoopSubsystem* Loop = World->GetSubsystem<URewindLoopSubsystem>())
			{
				return Loop->GetElapsedLoopTime();
			}
		}
		return -1.0;
	}

	void Event(const UObject* WorldContext, const FString& Message)
	{
		UE_LOG(LogRewind, Display, TEXT("t=%.2f  %s"), ElapsedSeconds(WorldContext), *Message);
	}

	void Baseline(const FString& Message)
	{
		UE_LOG(LogRewind, Display, TEXT("[apply]  %s"), *Message);
	}
}
