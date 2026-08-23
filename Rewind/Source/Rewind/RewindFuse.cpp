#include "RewindFuse.h"

#include "RewindLog.h"
#include "Components/StaticMeshComponent.h"
#include "EngineUtils.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"

ARewindFuse::ARewindFuse()
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Mesh->SetCollisionResponseToAllChannels(ECR_Overlap);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (Cube.Succeeded())
	{
		Mesh->SetStaticMesh(Cube.Object);
	}
	Mesh->SetRelativeScale3D(FVector(0.12f, 0.12f, 0.28f));
}

ARewindFuse* ARewindFuse::Find(const UWorld* World)
{
	if (!World)
	{
		return nullptr;
	}
	for (TActorIterator<ARewindFuse> It(const_cast<UWorld*>(World)); It; ++It)
	{
		return *It;
	}
	return nullptr;
}

const TCHAR* ARewindFuse::SocketName(ERewindFuseSocket Which)
{
	return Which == ERewindFuseSocket::Building ? TEXT("building") : TEXT("courtyard");
}

void ARewindFuse::SetCarryCollision(bool bCarried)
{
	// A carried fuse follows the player, so it would otherwise sit inside the
	// interaction sphere permanently and shadow every socket the player walks
	// up to. In hand it is not a thing you interact with.
	Mesh->SetCollisionEnabled(bCarried ? ECollisionEnabled::NoCollision : ECollisionEnabled::QueryOnly);
}

void ARewindFuse::RestoreFromBaseline()
{
	if (!bHomeCaptured)
	{
		HomeLocation = GetActorLocation();
		bHomeCaptured = true;
	}
	State = EState::AtRest;
	SetActorLocation(HomeLocation);
	SetCarryCollision(false);
	Mesh->SetVisibility(true);
	RewindLog::Baseline(TEXT("Fuse: at rest in 4C, both sockets empty"));
}

void ARewindFuse::ApplyAnchorOverride(FName AnchorId)
{
	(void)AnchorId;
}

bool ARewindFuse::TryInteract(APawn* InstigatorPawn)
{
	(void)InstigatorPawn;

	if (State != EState::AtRest)
	{
		return false;
	}

	State = EState::Carried;
	SetCarryCollision(true);
	RewindLog::Event(this, TEXT("Fuse: picked up"));
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Fuse: carried"));
	}
	return true;
}

bool ARewindFuse::SeatInto(ERewindFuseSocket Which)
{
	if (State != EState::Carried)
	{
		return false;
	}
	State = EState::Seated;
	Seated = Which;
	SetCarryCollision(false);
	RewindLog::Event(this, FString::Printf(TEXT("Fuse: seated in the %s socket"), SocketName(Which)));
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green,
			FString::Printf(TEXT("Fuse: in the %s socket"), SocketName(Which)));
	}
	return true;
}

bool ARewindFuse::TakeFrom(ERewindFuseSocket Which)
{
	if (!IsSeatedIn(Which))
	{
		return false;
	}
	State = EState::Carried;
	SetCarryCollision(true);
	RewindLog::Event(this, FString::Printf(TEXT("Fuse: taken from the %s socket"), SocketName(Which)));
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow,
			FString::Printf(TEXT("Fuse: taken from the %s socket, carried"), SocketName(Which)));
	}
	return true;
}

void ARewindFuse::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	(void)DeltaSeconds;

	if (State != EState::Carried)
	{
		return;
	}

	const UWorld* World = GetWorld();
	const APlayerController* Controller = World ? World->GetFirstPlayerController() : nullptr;
	const APawn* Pawn = Controller ? Controller->GetPawn() : nullptr;
	if (Pawn)
	{
		SetActorLocation(Pawn->GetActorLocation() + FVector(0.f, 0.f, 40.f));
	}
}
