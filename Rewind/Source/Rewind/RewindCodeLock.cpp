#include "RewindCodeLock.h"

#include "RewindIds.h"
#include "RewindSessionSubsystem.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/GameInstance.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"

ARewindCodeLock::ARewindCodeLock()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (Cube.Succeeded())
	{
		Mesh->SetStaticMesh(Cube.Object);
	}
	Mesh->SetRelativeScale3D(FVector(0.3f, 2.2f, 2.4f));
	SetLocked(true);
}

bool ARewindCodeLock::TryInteract(APawn* InstigatorPawn)
{
	if (!bLocked)
	{
		return true;
	}
	bool bHasCode = false;
	if (InstigatorPawn)
	{
		if (UGameInstance* GI = InstigatorPawn->GetGameInstance())
		{
			if (const URewindSessionSubsystem* Session = GI->GetSubsystem<URewindSessionSubsystem>())
			{
				bHasCode = Session->HasKnowledge(RewindIds::KnowledgeRadioCode7312);
			}
		}
	}
	if (bHasCode)
	{
		Submit(TEXT("7312"));
		return true;
	}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Lock: enter 4 digits, or learn 7312 from the radio"));
	}
	return false;
}

void ARewindCodeLock::ReceiveDigit(int32 Digit)
{
	if (!bLocked)
	{
		return;
	}
	Buffer.AppendChar(TCHAR('0' + Digit));
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::White, FString::Printf(TEXT("Code: %s"), *Buffer));
	}
	if (Buffer.Len() >= 4)
	{
		Submit(Buffer.Left(4));
		Buffer.Reset();
	}
}

void ARewindCodeLock::RestoreFromBaseline()
{
	Buffer.Reset();
	SetLocked(true);
}

void ARewindCodeLock::ApplyAnchorOverride(FName AnchorId)
{
	(void)AnchorId;
}

void ARewindCodeLock::Submit(const FString& Code)
{
	if (Code == TEXT("7312"))
	{
		SetLocked(false);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Lock: 7312 accepted"));
		}
	}
	else
	{
		SetLocked(true);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("Lock: %s rejected"), *Code));
		}
	}
}

void ARewindCodeLock::SetLocked(bool bInLocked)
{
	bLocked = bInLocked;
	Mesh->SetCollisionEnabled(bLocked ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
	Mesh->SetVisibility(bLocked);
}
