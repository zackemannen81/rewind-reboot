#include "RewindLoopBreakSignature.h"

#include "RewindFirstRun.h"
#include "RewindLog.h"
#include "RewindLoopSubsystem.h"
#include "RewindMessageIds.h"
#include "Components/PostProcessComponent.h"
#include "Engine/Engine.h"
#include "Engine/World.h"

ARewindLoopBreakSignature::ARewindLoopBreakSignature()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	PostProcess = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcess"));
	PostProcess->SetupAttachment(Root);
	PostProcess->bUnbound = true;
	PostProcess->bEnabled = true;
	PostProcess->BlendWeight = 0.f;
	PostProcess->Priority = 10.f;

	FPostProcessSettings& Settings = PostProcess->Settings;
	Settings.bOverride_VignetteIntensity = true;
	Settings.VignetteIntensity = 1.0f;
	Settings.bOverride_SceneFringeIntensity = true;
	Settings.SceneFringeIntensity = 4.0f;
	Settings.bOverride_ColorSaturation = true;
	Settings.ColorSaturation = FVector4(0.15f, 0.15f, 0.15f, 1.0f);
	Settings.bOverride_FilmGrainIntensity = true;
	Settings.FilmGrainIntensity = 1.0f;
}

void ARewindLoopBreakSignature::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	(void)DeltaSeconds;

	DisplayedIntensity = 0.0;
	double Elapsed = -1.0;
	if (UWorld* World = GetWorld())
	{
		if (const URewindLoopSubsystem* Loop = World->GetSubsystem<URewindLoopSubsystem>())
		{
			DisplayedIntensity = Loop->GetLoopBreakIntensity();
			Elapsed = Loop->GetElapsedLoopTime();
		}
	}

	if (PostProcess)
	{
		PostProcess->BlendWeight = static_cast<float>(DisplayedIntensity);
		PostProcess->bEnabled = DisplayedIntensity > 0.0;
	}

	if (DisplayedIntensity > 0.0)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				9002,
				0.2f,
				FColor::White,
				FString::Printf(TEXT("STATIC  interference=%.2f  t=%.2f"),
					DisplayedIntensity, Elapsed));
		}
		if (!bLoggedVisible)
		{
			RewindFirstRun::ShowOnce(this, RewindMessageIds::LoopBreak);
			RewindLog::Event(this, FString::Printf(
				TEXT("LOOP BREAK  interference=%.2f (elapsed loop time)"),
				DisplayedIntensity));
			bLoggedVisible = true;
		}
	}
	else
	{
		if (bLoggedVisible)
		{
			RewindFirstRun::ShowOnce(this, RewindMessageIds::ApartmentReturned);
		}
		bLoggedVisible = false;
	}
}
