#include "MuseumAfterDark/UI/InteractionPromptWidget.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"
#include "Components/SizeBox.h"
#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"

void UInteractionPromptWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// Configure text block for auto-sizing
	if (PromptTextBlock)
	{
		// Enable auto-sizing - text will expand to fit content
		PromptTextBlock->SetAutoWrapText(false);
		
		// Set minimum desired width (prevents it from being too narrow)
		PromptTextBlock->SetMinDesiredWidth(200.0f);
		
		// Allow text to expand horizontally
		PromptTextBlock->SetJustification(ETextJustify::Center);
	}
	
	// Hide by default
	HidePrompt();
}

void UInteractionPromptWidget::SetPromptText(const FText& InPromptText)
{
	if (PromptTextBlock)
	{
		PromptTextBlock->SetText(InPromptText);
		
		// Force the widget to update its size
		// The text block will automatically size to content if configured correctly
		InvalidateLayoutAndVolatility();
	}
}

void UInteractionPromptWidget::ShowPrompt()
{
	SetVisibility(ESlateVisibility::Visible);
}

void UInteractionPromptWidget::HidePrompt()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

