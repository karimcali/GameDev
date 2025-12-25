#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

class UTextBlock;
class UImage;

/**
 * Main HUD widget that displays:
 * - Objective text (top-right)
 * - Inventory icon with key input (bottom-right)
 * - Timer (top-center) - shows run duration
 */
UCLASS(BlueprintType, Blueprintable)
class MUSEUMAFTERDARK_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Set objective text */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetObjectiveText(const FText& ObjectiveText);

	/** Show objective text */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowObjective();

	/** Hide objective text */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void HideObjective();

	/** Set default objective (called automatically) */
	void SetDefaultObjective();

	/** Update timer display */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateTimer(float ElapsedTime);

	/** Start the timer */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void StartTimer();

	/** Stop the timer */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void StopTimer();

	/** Reset the timer */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ResetTimer();

	/** Start countdown timer with time limit */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void StartCountdown(float TimeLimitInSeconds);

	/** Get remaining time */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	float GetRemainingTime() const { return RemainingTime; }

	/** Check if timer has reached zero */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	bool IsTimeUp() const { return bTimerRunning && RemainingTime <= 0.0f; }

	/** Delegate fired when timer reaches zero */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTimeUp);
	UPROPERTY(BlueprintAssignable, Category = "HUD")
	FOnTimeUp OnTimeUp;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	/** Objective text block (top-right) */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ObjectiveTextBlock;

	/** Timer text block (top-center) */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TimerTextBlock;

	/** Inventory icon (bottom-right) */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> InventoryIcon;

	/** Inventory key input text (bottom-right, next to icon) */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> InventoryKeyText;

private:
	/** Whether timer is running */
	bool bTimerRunning;

	/** Whether timer is in countdown mode */
	bool bCountdownMode;

	/** Remaining time in seconds (for countdown) */
	float RemainingTime;

	/** Format time as MM:SS */
	FString FormatTime(float TimeInSeconds) const;

	/** Update timer display color based on remaining time */
	void UpdateTimerColor();
};
