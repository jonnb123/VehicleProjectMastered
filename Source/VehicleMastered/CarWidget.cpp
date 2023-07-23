// Fill out your copyright notice in the Description page of Project Settings.

#include "CarWidget.h"
#include "VehicleGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "UMG/Public/Components/TextBlock.h"
#include "VehiclePawn.h"
#include "ChaosVehicleMovementComponent.h"

void UCarWidget::NativeConstruct()
{
    Super::NativeConstruct();

    FTimerHandle TimerHandleTime;
    FTimerHandle TimerHandleSpeed;
    // Set up the timer to call the UpdateTimeRemainingText function every 0.1 seconds
    float UpdateInterval = 0.1f;
    GetWorld()->GetTimerManager().SetTimer(TimerHandleTime, this, &UCarWidget::UpdateTimeLabelText, UpdateInterval, true);
    GetWorld()->GetTimerManager().SetTimer(TimerHandleSpeed, this, &UCarWidget::UpdateSpeedText, UpdateInterval, true);

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

void UCarWidget::UpdateSpeedText()
{
    APlayerController *PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    // Get the pawn possessed by the player controller (should be your AVehiclePawn)
    AVehiclePawn *MyVehiclePawn = Cast<AVehiclePawn>(PlayerController->GetPawn());

    float Speed = MyVehiclePawn->GetVehicleMovementComponent()->GetForwardSpeed();
    float SpeedMph = FMath::FloorToDouble(FMath::Abs(Speed * 0.0237)); // multiply by 0.0237 to get mph

    // Update the TimeRemainingText with the new value
    FString SpeedLabelString = FString::Printf(TEXT("%.1f mph"), SpeedMph);

    SpeedLabel->SetText(FText::FromString(SpeedLabelString));
}
