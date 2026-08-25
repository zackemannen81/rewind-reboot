#pragma once

#include "CoreMinimal.h"
#include "Brushes/SlateColorBrush.h"
#include "Widgets/SCompoundWidget.h"

class SBorder;
class STextBlock;

/**
 * Bottom-of-frame line, light text on a dark backing. Hit-test invisible so it
 * cannot steal input from the authored camera composition.
 */
class SRewindMessageOverlay : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SRewindMessageOverlay) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	void SetLine(const FString& Text);

private:
	FSlateColorBrush BackingBrush{FLinearColor::White};
	TSharedPtr<SBorder> Backing;
	TSharedPtr<STextBlock> Label;
};
