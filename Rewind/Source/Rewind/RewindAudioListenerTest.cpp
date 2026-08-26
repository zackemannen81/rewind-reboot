#include "RewindCameraRig.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Misc/AutomationTest.h"

namespace
{
	APawn* SpawnListenerPawn(UWorld* World, const TCHAR* RootName, const FVector& Location,
		const FRotator& Rotation)
	{
		APawn* Pawn = World ? World->SpawnActor<APawn>() : nullptr;
		if (!Pawn)
		{
			return nullptr;
		}

		USceneComponent* Root = NewObject<USceneComponent>(Pawn, RootName);
		Pawn->SetRootComponent(Root);
		Root->RegisterComponentWithWorld(World);
		Pawn->SetActorLocationAndRotation(Location, Rotation);
		return Pawn;
	}

	bool TestListenerTransform(FAutomationTestBase& Test, APlayerController* Controller,
		const FVector& ExpectedLocation, const FRotator& ExpectedRotation, const TCHAR* Context)
	{
		FVector ActualLocation = FVector::ZeroVector;
		FVector ActualFront = FVector::ZeroVector;
		FVector ActualRight = FVector::ZeroVector;
		Controller->GetAudioListenerPosition(ActualLocation, ActualFront, ActualRight);

		const FVector ExpectedFront = FRotationMatrix(ExpectedRotation).GetUnitAxis(EAxis::X);
		const FVector ExpectedRight = FRotationMatrix(ExpectedRotation).GetUnitAxis(EAxis::Y);
		const FString Prefix(Context);
		bool bPassed = true;
		bPassed &= Test.TestTrue(*FString::Printf(TEXT("%s listener is on the possessed root"), *Prefix),
			ActualLocation.Equals(ExpectedLocation, 0.001));
		bPassed &= Test.TestTrue(*FString::Printf(TEXT("%s listener front follows the camera"), *Prefix),
			ActualFront.Equals(ExpectedFront, 0.001));
		bPassed &= Test.TestTrue(*FString::Printf(TEXT("%s listener right follows the camera"), *Prefix),
			ActualRight.Equals(ExpectedRight, 0.001));
		return bPassed;
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FRewindPlayerRelativeAudioListenerTest,
	"Rewind.Audio.Listener.PlayerPositionCameraOrientation",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FRewindPlayerRelativeAudioListenerTest::RunTest(const FString& Parameters)
{
	(void)Parameters;

	UWorld* World = UWorld::CreateWorld(EWorldType::Game, false);
	TestNotNull(TEXT("transient test world"), World);
	if (!World)
	{
		return false;
	}

	APlayerController* Controller = World->SpawnActor<APlayerController>();
	ARewindCameraRig* Camera = World->SpawnActor<ARewindCameraRig>();
	APawn* FirstPawn = SpawnListenerPawn(
		World, TEXT("FirstListenerRoot"), FVector(125.0, -340.0, 96.0), FRotator(0.0, 37.0, 0.0));
	TestNotNull(TEXT("player controller"), Controller);
	TestNotNull(TEXT("detached authored camera"), Camera);
	TestNotNull(TEXT("first possessed pawn"), FirstPawn);
	if (!Controller || !Camera || !FirstPawn)
	{
		World->DestroyWorld(false);
		return false;
	}

	// UWorld::CreateWorld does not initialize actors far enough to run
	// AController::PostInitializeComponents, which normally adds the controller
	// to the world's player-zero list. Establish that runtime relationship
	// explicitly for this isolated test world.
	World->AddController(Controller);
	Controller->Possess(FirstPawn);
	TestTrue(TEXT("fixture registers the runtime player-zero controller"),
		World->GetFirstPlayerController() == Controller);
	TestTrue(TEXT("fixture controller possesses the first pawn"),
		Controller->GetPawn() == FirstPawn);
	const FVector DetachedCameraLocation(2400.0, 1700.0, 1350.0);
	const FRotator FirstCameraRotation(-12.0, 163.0, 7.0);
	Camera->SetActorLocationAndRotation(DetachedCameraLocation, FirstCameraRotation);
	Camera->SnapToPlayer();

	const FRewindAudioListenerContract& FirstContract = Camera->GetAudioListenerContract();
	TestTrue(TEXT("listener override is applied"), FirstContract.bOverrideApplied);
	TestTrue(TEXT("listener attaches to the possessed pawn root"),
		FirstContract.PositionAttachment.Get() == FirstPawn->GetRootComponent());
	TestTrue(TEXT("listener tracks the possessed pawn"), FirstContract.TrackedPawn.Get() == FirstPawn);
	TestTrue(TEXT("requested listener position equals the pawn position"),
		FirstContract.RequestedWorldLocation.Equals(FirstPawn->GetActorLocation(), 0.001));
	TestTrue(TEXT("requested listener rotation equals the camera rotation"),
		FirstContract.RequestedWorldRotation.Equals(FirstCameraRotation, 0.001));
	TestTrue(TEXT("detached camera is not used as listener position"),
		!FirstContract.RequestedWorldLocation.Equals(DetachedCameraLocation, 1.0));
	TestEqual(TEXT("initial possession is observed"), FirstContract.PossessionGeneration, 1u);
	TestEqual(TEXT("initial snap refresh is observed"), FirstContract.SnapGeneration, 1u);
	TestListenerTransform(*this, Controller, FirstPawn->GetActorLocation(), FirstCameraRotation,
		TEXT("initial"));

	APawn* SecondPawn = SpawnListenerPawn(
		World, TEXT("SecondListenerRoot"), FVector(-810.0, 520.0, 196.0), FRotator(18.0, -71.0, 11.0));
	TestNotNull(TEXT("replacement possessed pawn"), SecondPawn);
	if (SecondPawn)
	{
		Controller->Possess(SecondPawn);
		const FRotator SecondCameraRotation(21.0, -142.0, -9.0);
		Camera->SetActorRotation(SecondCameraRotation);
		Camera->Tick(0.0f);

		const FRewindAudioListenerContract& PossessionContract = Camera->GetAudioListenerContract();
		TestTrue(TEXT("possession change replaces the listener attachment"),
			PossessionContract.PositionAttachment.Get() == SecondPawn->GetRootComponent());
		TestTrue(TEXT("possession change replaces the tracked pawn"),
			PossessionContract.TrackedPawn.Get() == SecondPawn);
		TestEqual(TEXT("possession change refresh is observed"),
			PossessionContract.PossessionGeneration, 2u);
		TestListenerTransform(*this, Controller, SecondPawn->GetActorLocation(),
			SecondCameraRotation, TEXT("after possession"));

		const FVector ResetLocation(45.0, 65.0, 1296.0);
		const FRotator ResetCameraRotation(-3.0, 180.0, 0.0);
		SecondPawn->SetActorLocation(ResetLocation);
		Camera->SetActorRotation(ResetCameraRotation);
		Camera->SnapToPlayer();

		const FRewindAudioListenerContract& ResetContract = Camera->GetAudioListenerContract();
		TestEqual(TEXT("loop/player snap refresh is observed"), ResetContract.SnapGeneration, 2u);
		TestTrue(TEXT("snap updates the requested player position"),
			ResetContract.RequestedWorldLocation.Equals(ResetLocation, 0.001));
		TestTrue(TEXT("snap updates the requested camera orientation"),
			ResetContract.RequestedWorldRotation.Equals(ResetCameraRotation, 0.001));
		TestListenerTransform(*this, Controller, ResetLocation, ResetCameraRotation,
			TEXT("after reset snap"));
	}

	World->DestroyWorld(false);
	return !HasAnyErrors();
}

#endif
