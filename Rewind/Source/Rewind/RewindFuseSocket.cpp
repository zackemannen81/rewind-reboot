#include "RewindFuseSocket.h"

#include "RewindLog.h"
#include "Components/StaticMeshComponent.h"
#include "EngineUtils.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"

ARewindFuseSocket::ARewindFuseSocket()
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("/Engine/BasicShapes/Cube.Cube"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Mesh->SetCollisionResponseToAllChannels(ECR_Overlap);
	Mesh->SetRelativeScale3D(FVector(0.3f, 0.5f, 0.5f));

	// A visible plug, so an occupied socket reads without a caption.
	SeatedFuse = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SeatedFuse"));
	SeatedFuse->SetupAttachment(Root);
	SeatedFuse->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SeatedFuse->SetRelativeLocation(FVector(-18.f, 0.f, 0.f));
	SeatedFuse->SetRelativeScale3D(FVector(0.12f, 0.12f, 0.28f));

	if (Cube.Succeeded())
	{
		Mesh->SetStaticMesh(Cube.Object);
		SeatedFuse->SetStaticMesh(Cube.Object);
	}
}

ARewindFuseSocket* ARewindFuseSocket::Find(const UWorld* World, ERewindFuseSocket Which)
{
	if (!World)
	{
		return nullptr;
	}
	for (TActorIterator<ARewindFuseSocket> It(const_cast<UWorld*>(World)); It; ++It)
	{
		if (It->GetSocket() == Which)
		{
			return *It;
		}
	}
	return nullptr;
}

bool ARewindFuseSocket::IsOccupied() const
{
	const ARewindFuse* Fuse = ARewindFuse::Find(GetWorld());
	return Fuse && Fuse->IsSeatedIn(Which);
}

bool ARewindFuseSocket::TryInteract(APawn* InstigatorPawn)
{
	(void)InstigatorPawn;

	ARewindFuse* Fuse = ARewindFuse::Find(GetWorld());
	if (!Fuse)
	{
		return false;
	}

	if (Fuse->IsSeatedIn(Which))
	{
		return Fuse->TakeFrom(Which);
	}

	if (Fuse->IsCarried())
	{
		return Fuse->SeatInto(Which);
	}

	// Neither in hand nor here: say which, because "nothing happened" is the
	// worst thing a socket can tell a player who is mid-puzzle.
	const TCHAR* Where = Fuse->IsSeated() ? TEXT("in the other socket") : TEXT("still where it started");
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow,
			FString::Printf(TEXT("Socket: empty. The fuse is %s"), Where));
	}
	RewindLog::Event(this, FString::Printf(TEXT("Socket %s: refused, fuse is %s"),
		Which == ERewindFuseSocket::Building ? TEXT("building") : TEXT("courtyard"), Where));
	return false;
}

void ARewindFuseSocket::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	(void)DeltaSeconds;
	SeatedFuse->SetVisibility(IsOccupied());
}
