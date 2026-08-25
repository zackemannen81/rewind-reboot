#include "RewindWorldStateSubsystem.h"

#include "RewindLog.h"
#include "RewindAuthoredCourtyard.h"
#include "RewindCameraRig.h"
#include "RewindCausalCheckpoint.h"
#include "RewindFourCBlockout.h"
#include "RewindFuse.h"
#include "RewindFuseSocket.h"
#include "RewindIds.h"
#include "RewindLoopBreakSignature.h"
#include "RewindProofLayout.h"
#include "RewindLoopParticipant.h"
#include "RewindSessionSubsystem.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "GameFramework/WorldSettings.h"
#include "Engine/GameInstance.h"

namespace
{
	const FName SkipProofLayoutTag(TEXT("Rewind.SkipProofLayout"));
}

bool URewindWorldStateSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	const UWorld* World = Cast<UWorld>(Outer);
	return World && World->IsGameWorld();
}

void URewindWorldStateSubsystem::EnsureAuthoredSpace()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	EnsureLoopBreakSignature();

	// Standalone authored-space and geometry blockouts own their contents. The
	// proof layout is still the default, but an explicit map tag prevents it
	// from being generated on top of those authored actors during PIE.
	if (const AWorldSettings* WorldSettings = World->GetWorldSettings();
		WorldSettings && WorldSettings->ActorHasTag(SkipProofLayoutTag))
	{
		EnsureAuthoredCourtyard();
		return;
	}

	ARewindProofLayout* Layout = nullptr;
	for (TActorIterator<ARewindProofLayout> It(World); It; ++It)
	{
		Layout = *It;
		break;
	}
	if (!Layout)
	{
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Layout = World->SpawnActor<ARewindProofLayout>(FVector::ZeroVector, FRotator::ZeroRotator, Params);
	}
	if (Layout)
	{
		Layout->EnsureContents();
	}

	EnsureCausalCheckpoints();
}

void URewindWorldStateSubsystem::EnsureAuthoredCourtyard()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	ARewindAuthoredCourtyard* Courtyard = nullptr;
	for (TActorIterator<ARewindAuthoredCourtyard> It(World); It; ++It)
	{
		Courtyard = *It;
		break;
	}
	if (!Courtyard)
	{
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Courtyard = World->SpawnActor<ARewindAuthoredCourtyard>(
			FVector::ZeroVector, FRotator::ZeroRotator, Params);
	}
	if (Courtyard)
	{
		Courtyard->EnsureContents();
	}
}

void URewindWorldStateSubsystem::EnsureCausalCheckpoints()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	ARewindCausalCheckpoint* Checkpoint = nullptr;
	for (TActorIterator<ARewindCausalCheckpoint> It(World); It; ++It)
	{
		if (It->GetCheckpointId() == RewindIds::CheckpointGroundFuseGate)
		{
			Checkpoint = *It;
			break;
		}
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (!Checkpoint)
	{
		// Hall floor ends and courtyard floor begins at X = 4110. Crossing that
		// seam is the GroundFuseGate no-return threshold.
		Checkpoint = World->SpawnActor<ARewindCausalCheckpoint>(
			FVector(4110.f, -300.f, 150.f), FRotator::ZeroRotator, Params);
		if (Checkpoint)
		{
			RewindLog::Baseline(TEXT("Checkpoint: GroundFuseGate at hall-to-courtyard seam"));
		}
	}
	if (Checkpoint)
	{
		Checkpoint->Configure(
			RewindIds::CheckpointGroundFuseGate,
			ERewindCheckpointPredicate::GroundFuseGate,
			FVector(80.f, 300.f, 180.f));
		Checkpoint->SetActorLocationAndRotation(
			FVector(4110.f, -300.f, 150.f), FRotator::ZeroRotator, false, nullptr,
			ETeleportType::TeleportPhysics);
	}
}

void URewindWorldStateSubsystem::EnsureLoopBreakSignature()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	for (TActorIterator<ARewindLoopBreakSignature> It(World); It; ++It)
	{
		return;
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	World->SpawnActor<ARewindLoopBreakSignature>(FVector::ZeroVector, FRotator::ZeroRotator, Params);
}

void URewindWorldStateSubsystem::ApplyLoopStart()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	EnsureAuthoredSpace();

	TArray<IRewindLoopParticipant*> WorldParts;
	for (FActorIterator It(World); It; ++It)
	{
		AActor* Actor = *It;
		if (IRewindLoopParticipant* Participant = Cast<IRewindLoopParticipant>(Actor))
		{
			if (!Participant->IsPlayerBody())
			{
				WorldParts.Add(Participant);
			}
		}
	}

	for (IRewindLoopParticipant* Participant : WorldParts)
	{
		Participant->RestoreFromBaseline();
	}

	FName ActiveAnchor = NAME_None;
	if (const UGameInstance* GameInstance = World->GetGameInstance())
	{
		if (const URewindSessionSubsystem* Session = GameInstance->GetSubsystem<URewindSessionSubsystem>())
		{
			ActiveAnchor = Session->GetActiveAnchor();
		}
	}

	if (!ActiveAnchor.IsNone())
	{
		for (IRewindLoopParticipant* Participant : WorldParts)
		{
			Participant->ApplyAnchorOverride(ActiveAnchor);
		}
	}

	PlacePlayerBody();
}

void URewindWorldStateSubsystem::PlacePlayerBody()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	const FTransform Start = GetLoopStartPose();
	APlayerController* Controller = World->GetFirstPlayerController();
	APawn* Pawn = Controller ? Controller->GetPawn() : nullptr;
	if (!Pawn)
	{
		return;
	}

	Pawn->SetActorTransform(Start, false, nullptr, ETeleportType::ResetPhysics);

	// Step 4 of the apply order. Logged so FL-01's "the player is in Apartment
	// 4C at t = 0" is readable from the log rather than needing a live query.
	const FVector P = Start.GetLocation();
	RewindLog::Baseline(FString::Printf(
		TEXT("Player: placed at loop-start pose (%.0f, %.0f, %.0f)"), P.X, P.Y, P.Z));

	// The camera takes its region's pose immediately at loop start, per
	// `camera-and-movement.md`. This is world state notifying presentation; it
	// never reads anything back, so the camera stays outside the model.
	for (TActorIterator<ARewindCameraRig> It(World); It; ++It)
	{
		It->SnapToPlayer();
	}
}

bool URewindWorldStateSubsystem::PlacePlayerForVerification(FName Pose)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return false;
	}

	APlayerController* Controller = World->GetFirstPlayerController();
	APawn* Pawn = Controller ? Controller->GetPawn() : nullptr;
	if (!Pawn)
	{
		RewindLog::Event(this, TEXT("PlacePlayerForVerification: no pawn"));
		return false;
	}

	FVector Location = FVector::ZeroVector;
	FRotator Rotation = FRotator::ZeroRotator;
	if (Pose == FName(TEXT("threshold")))
	{
		ARewindCausalCheckpoint* Checkpoint = nullptr;
		for (TActorIterator<ARewindCausalCheckpoint> It(World); It; ++It)
		{
			if (It->GetCheckpointId() == RewindIds::CheckpointGroundFuseGate)
			{
				Checkpoint = *It;
				break;
			}
		}
		if (!Checkpoint)
		{
			RewindLog::Event(this, TEXT("PlacePlayerForVerification: no GroundFuseGate"));
			return false;
		}
		Location = Checkpoint->GetActorLocation();
		Location.Y -= 160.0;
		Location.Z = 96.0;
		Rotation = FRotator(0.0, 90.0, 0.0);
	}
	else if (Pose == FName(TEXT("fuse")))
	{
		ARewindFuse* Fuse = ARewindFuse::Find(World);
		if (!Fuse)
		{
			RewindLog::Event(this, TEXT("PlacePlayerForVerification: no fuse"));
			return false;
		}
		Location = Fuse->GetActorLocation();
		// Stand 155 cm toward +Y so the 160 cm interact sphere reaches the
		// fuse and not the building socket 10 cm beside it.
		Location.Y += 155.0;
		Location.Z = 1296.0;
		Rotation = FRotator(0.0, 180.0, 0.0);
	}
	else if (Pose == FName(TEXT("courtyard_socket")))
	{
		ARewindFuseSocket* Socket = ARewindFuseSocket::Find(World, ERewindFuseSocket::Courtyard);
		if (!Socket)
		{
			RewindLog::Event(this, TEXT("PlacePlayerForVerification: no courtyard socket"));
			return false;
		}
		Location = Socket->GetActorLocation();
		Location.Z = 96.0;
		Rotation = FRotator(0.0, 90.0, 0.0);
	}
	else
	{
		RewindLog::Event(this, TEXT("PlacePlayerForVerification: refused, use threshold, fuse or courtyard_socket"));
		return false;
	}

	Pawn->SetActorLocationAndRotation(
		Location, Rotation, false, nullptr, ETeleportType::ResetPhysics);

	for (TActorIterator<ARewindCameraRig> It(World); It; ++It)
	{
		It->SnapToPlayer();
	}

	RewindLog::Event(this, FString::Printf(
		TEXT("PlacePlayerForVerification %s: (%.0f, %.0f, %.0f)"),
		*Pose.ToString(), Location.X, Location.Y, Location.Z));
	return true;
}

FTransform URewindWorldStateSubsystem::GetLoopStartPose() const
{
	const UWorld* World = GetWorld();
	if (!World)
	{
		return FTransform::Identity;
	}

	APlayerStart* Tagged = nullptr;
	APlayerStart* First = nullptr;
	for (TActorIterator<APlayerStart> It(World); It; ++It)
	{
		APlayerStart* Start = *It;
		if (!First)
		{
			First = Start;
		}
		if (Start->PlayerStartTag == FName(TEXT("LoopStart")))
		{
			Tagged = Start;
			break;
		}
	}

	if (const APlayerStart* Chosen = Tagged ? Tagged : First)
	{
		return Chosen->GetActorTransform();
	}

	return FTransform::Identity;
}
