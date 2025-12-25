#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionPromptWidget.generated.h"

class UTextBlock;

/**
 * Widget that displays interaction prompts (e.g., "Press E to Pick Up")
 */
UCLASS(BlueprintType, Blueprintable)
class MUSEUMAFTERDARK_API UInteractionPromptWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Set the prompt text to display */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void SetPromptText(const FText& InPromptText);

	/** Show the prompt widget */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void ShowPrompt();

	/** Hide the prompt widget */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void HidePrompt();

protected:
	virtual void NativeConstruct() override;

	/** Text block that displays the prompt */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> PromptTextBlock;

	/** The root panel containing the prompt */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPanelWidget> RootPanel;
};

