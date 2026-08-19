#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RewindFourCBlockout.generated.h"

UCLASS()
class REWIND_API ARewindFourCBlockout : public AActor
{
	GENERATED_BODY()

public:
	ARewindFourCBlockout();

	virtual void BeginPlay() override;

	void EnsureLoopStart() const;
	static FVector GetLoopStartLocation();
	static FRotator GetLoopStartRotation();

private:
	void AddBox(FName Name, FVector RelativeLocation, FVector SizeCm);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY()
	TObjectPtr<UStaticMesh> CubeMesh;
};
