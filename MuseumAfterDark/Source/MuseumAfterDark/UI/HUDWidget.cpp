#include "MuseumAfterDark/UI/HUDWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Styling/SlateColor.h"

void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Set default objective and show it
	if (ObjectiveTextBlock)
	{
		ObjectiveTextBlock->SetText(FText::FromString(TEXT("Find the exit before dawn")));
		ObjectiveTextBlock->SetVisibility(ESlateVisibility::Visible);
	}

	// Initialize timer
	bTimerRunning = false;
	bCountdownMode = true;
	RemainingTime = 900.0f; // 15 minutes = 900 seconds

	// Start countdown automatically (15 minutes = 900 seconds)
	StartCountdown(900.0f);
}

void UHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Update timer if running
	if (bTimerRunning)
	{
		if (bCountdownMode)
		{
			// Countdown mode: decrement time
			RemainingTime -= InDeltaTime;
			if (RemainingTime <= 0.0f)
			{
				RemainingTime = 0.0f;
				bTimerRunning = false;
				UpdateTimer(0.0f);
				UpdateTimerColor();
				// Broadcast time up event
				OnTimeUp.Broadcast();
			}
			else
			{
				UpdateTimer(RemainingTime);
				UpdateTimerColor();
			}
		}
	}
}

void UHUDWidget::SetObjectiveText(const FText& ObjectiveText)
{
	if (ObjectiveTextBlock)
	{
		ObjectiveTextBlock->SetText(ObjectiveText);
		// Automatically show when text is set
		ShowObjective();
	}
}

void UHUDWidget::SetDefaultObjective()
{
	SetObjectiveText(FText::FromString(TEXT("Find the exit before dawn")));
}

void UHUDWidget::ShowObjective()
{
	if (ObjectiveTextBlock)
	{
		ObjectiveTextBlock->SetVisibility(ESlateVisibility::Visible);
	}
}

void UHUDWidget::HideObjective()
{
	if (ObjectiveTextBlock)
	{
		ObjectiveTextBlock->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UHUDWidget::UpdateTimer(float TimeInSeconds)
{
	if (TimerTextBlock)
	{
		FString FormattedTime = FormatTime(TimeInSeconds);
		TimerTextBlock->SetText(FText::FromString(FormattedTime));
	}
}

void UHUDWidget::StartTimer()
{
	bTimerRunning = true;
	bCountdownMode = false; // Legacy elapsed time mode
	if (TimerTextBlock)
	{
		TimerTextBlock->SetVisibility(ESlateVisibility::Visible);
	}
}

void UHUDWidget::StartCountdown(float TimeLimitInSeconds)
{
	bTimerRunning = true;
	bCountdownMode = true;
	RemainingTime = TimeLimitInSeconds;
	if (TimerTextBlock)
	{
		TimerTextBlock->SetVisibility(ESlateVisibility::Visible);
		UpdateTimer(RemainingTime);
		UpdateTimerColor();
	}
}

void UHUDWidget::StopTimer()
{
	bTimerRunning = false;
}

void UHUDWidget::ResetTimer()
{
	RemainingTime = 900.0f; // Reset to 15 minutes
	bTimerRunning = false;
	bCountdownMode = true;
	if (TimerTextBlock)
	{
		TimerTextBlock->SetText(FText::FromString(TEXT("15:00")));
		TimerTextBlock->SetVisibility(ESlateVisibility::Collapsed);
		TimerTextBlock->SetColorAndOpacity(FSlateColor(FLinearColor::White));
	}
}

void UHUDWidget::UpdateTimerColor()
{
	if (!TimerTextBlock)
	{
		return;
	}

	// Change color based on remaining time
	if (RemainingTime <= 30.0f)
	{
		// Red when less than 30 seconds
		TimerTextBlock->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
	}
	else if (RemainingTime <= 60.0f)
	{
		// Yellow when less than 1 minute
		TimerTextBlock->SetColorAndOpacity(FSlateColor(FLinearColor::Yellow));
	}
	else
	{
		// White for normal time
		TimerTextBlock->SetColorAndOpacity(FSlateColor(FLinearColor::White));
	}
}

FString UHUDWidget::FormatTime(float TimeInSeconds) const
{
	int32 TotalSeconds = FMath::Max(0, FMath::FloorToInt(TimeInSeconds));
	int32 Minutes = TotalSeconds / 60;
	int32 Seconds = TotalSeconds % 60;
	
	return FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
}
