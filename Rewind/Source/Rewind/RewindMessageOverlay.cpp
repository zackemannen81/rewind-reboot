#include "RewindMessageOverlay.h"

#include "Styling/CoreStyle.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Text/STextBlock.h"

void SRewindMessageOverlay::Construct(const FArguments& InArgs)
{
	(void)InArgs;

	SetVisibility(EVisibility::HitTestInvisible);
	SetCanTick(false);

	ChildSlot
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Bottom)
		.Padding(FMargin(48.f, 0.f, 48.f, 72.f))
		[
			SAssignNew(Backing, SBorder)
			.Padding(FMargin(18.f, 10.f))
			.BorderImage(&BackingBrush)
			.BorderBackgroundColor(FLinearColor(0.015f, 0.015f, 0.03f, 0.82f))
			.Visibility(EVisibility::Collapsed)
			[
				SAssignNew(Label, STextBlock)
				.ColorAndOpacity(FLinearColor(0.93f, 0.91f, 0.84f, 1.f))
				.Font(FCoreStyle::GetDefaultFontStyle("Regular", 20))
				.Justification(ETextJustify::Center)
				.AutoWrapText(true)
				.WrapTextAt(820.f)
				.ShadowOffset(FVector2D(1.f, 1.f))
				.ShadowColorAndOpacity(FLinearColor(0.f, 0.f, 0.f, 0.9f))
			]
		]
	];
}

void SRewindMessageOverlay::SetLine(const FString& Text)
{
	if (!Backing.IsValid() || !Label.IsValid())
	{
		return;
	}

	if (Text.IsEmpty())
	{
		Backing->SetVisibility(EVisibility::Collapsed);
		Label->SetText(FText::GetEmpty());
		return;
	}

	Backing->SetVisibility(EVisibility::HitTestInvisible);
	Label->SetText(FText::FromString(Text));
}
