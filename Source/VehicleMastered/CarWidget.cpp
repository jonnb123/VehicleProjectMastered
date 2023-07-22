// Fill out your copyright notice in the Description page of Project Settings.

#include "CarWidget.h"
#include "VehicleGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "UMG/Public/Components/TextBlock.h"

void UCarWidget::NativeConstruct()
{
    Super::NativeConstruct();

    FTimerHandle TimerHandle;
    // Set up the timer to call the UpdateTimeRemainingText function every 0.1 seconds
    float UpdateInterval = 0.1f;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UCarWidget::UpdateTimeLabelText, UpdateInterval, true);
}

void UCarWidget::BeginPlay()
{
    // Get the specific Game Mode class directly
    // AVehicleGameMode *GameMode = GetWorld()->GetAuthGameMode<AVehicleGameMode>();
}

void UCarWidget::UpdateTimeLabelText()
{
    // Get the specific Game Mode class directly
    AVehicleGameMode *GameMode = Cast<AVehicleGameMode>(UGameplayStatics::GetGameMode(this));
    if (GameMode)
    {
        float TimeRemaining = GameMode->TimeRemaining;

        // Update the TimeRemainingText with the new value
        FString TimeLabelString = FString::Printf(TEXT("%.1f"), TimeRemaining);

        TimeLabel->SetText(FText::FromString(TimeLabelString));
    }
}
