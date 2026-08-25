#pragma once

#include "CoreMinimal.h"
#include "RewindMessageQueue.h"
#include "Subsystems/WorldSubsystem.h"
#include "RewindMessageSubsystem.generated.h"

class SRewindMessageOverlay;

/**
 * World-owned player message channel. Actors call Show / ShowTimed /
 * ShowSequence / Clear by catalog id. Presentation is a Slate overlay.
 * `docs/design/player-messages.md` owns the rules.
 */
UCLASS()
class REWIND_API URewindMessageSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	static URewindMessageSubsystem* Get(const UObject* WorldContext);

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;

	void Show(FName Id);
	void Show(FName Id, const FString& Arg0);
	void ShowTimed(FName Id, float DurationSeconds);
	void ShowTimed(FName Id, float DurationSeconds, const FString& Arg0);
	void ShowSequence(const TArray<FName>& Ids);
	void Clear();

	bool HasActive() const { return Director.HasActive(); }
	FName GetActiveId() const { return Director.GetActiveId(); }
	FString GetActiveText() const { return Director.GetActiveText(); }

private:
	void EnsureOverlay();
	void DestroyOverlay();
	void SyncOverlay();

	FRewindMessageDirector Director;
	TSharedPtr<SRewindMessageOverlay> Overlay;
};
