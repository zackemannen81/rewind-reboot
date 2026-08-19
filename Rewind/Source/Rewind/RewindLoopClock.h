#pragma once

#include "CoreMinimal.h"

/** Elapsed loop time. World clocks read this, not engine time or frame count. */
struct FRewindLoopClock
{
	void Reset()
	{
		ElapsedSeconds = 0.0;
		bRunning = false;
	}

	void Start()
	{
		ElapsedSeconds = 0.0;
		bRunning = true;
	}

	void Stop()
	{
		bRunning = false;
	}

	void Advance(double DeltaSeconds)
	{
		if (bRunning && DeltaSeconds > 0.0)
		{
			ElapsedSeconds += DeltaSeconds;
		}
	}

	double GetElapsedSeconds() const { return ElapsedSeconds; }
	bool IsRunning() const { return bRunning; }

private:
	double ElapsedSeconds = 0.0;
	bool bRunning = false;
};
