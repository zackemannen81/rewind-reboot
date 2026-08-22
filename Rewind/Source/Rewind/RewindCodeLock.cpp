#include "RewindCodeLock.h"

#include "RewindLog.h"
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
		// FL-06 distinguishes the lock accepting a typed code from the game
		// typing it. Name the path so the record does not have to infer it.
		RewindLog::Event(this, TEXT("Lock: auto-submit from stored radio_code_7312"));
		Submit(TEXT("7312"));
		return true;
	}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Lock: enter 4 digits, or learn 7312 from the radio"));
	}
	RewindLog::Event(this, TEXT("Lock: locked, no stored code"));
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
		RewindLog::Event(this, FString::Printf(TEXT("Lock: manual entry %s"), *Buffer.Left(4)));
		Submit(Buffer.Left(4));
		Buffer.Reset();
	}
}

void ARewindCodeLock::RestoreFromBaseline()
{
	Buffer.Reset();
	SetLocked(true);
	RewindLog::Baseline(TEXT("Lock: locked"));
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
		RewindLog::Event(this, TEXT("Lock: 7312 accepted, doorway open"));
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Lock: 7312 accepted"));
		}
	}
	else
	{
		SetLocked(true);
		RewindLog::Event(this, FString::Printf(TEXT("Lock: %s rejected"), *Code));
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
