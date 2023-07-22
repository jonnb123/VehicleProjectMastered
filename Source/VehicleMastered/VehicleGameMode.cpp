// Fill out your copyright notice in the Description page of Project Settings.

#include "VehicleGameMode.h"
#include "Checkpoint.h"
#include "VehiclePawn.h"
#include "Kismet/GameplayStatics.h"
#include "ChaosVehicleMovementComponent.h"
#include "WinScreenWidget.h"

void AVehicleGameMode::BeginPlay()
{
	Super::BeginPlay();

	OutOfTime();
}

void AVehicleGameMode::GameStart()
{
	UE_LOG(LogTemp, Log, TEXT("The game is starting")); // this is called after the startcountdown, i.e. after 3 seconds
														// OutOfTime();										// this starts the overall game timer
}

void AVehicleGameMode::DisablePlayerInput()
{
	// if the game ends disable player input!
	// Get the default player controller
	APlayerController *PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	// Get the pawn possessed by the player controller (should be your AVehiclePawn)
	AVehiclePawn *MyVehiclePawn = Cast<AVehiclePawn>(PlayerController->GetPawn());

	MyVehiclePawn->DisableInput(PlayerController);
	MyVehiclePawn->GetVehicleMovementComponent()->SetSteeringInput(0.0f);

	MyVehiclePawn->GetVehicleMovementComponent()->SetThrottleInput(0.0f);
	MyVehiclePawn->GetVehicleMovementComponent()->SetBrakeInput(0.0f);
}

void AVehicleGameMode::GameEnd(bool Win)
{
	DisablePlayerInput();
	// Get the default player controller
	APlayerController *PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	PlayerController->SetShowMouseCursor(true);
	if (Win == true)
	{
		// Create the widget and add it to the class here
		if (WinWidgetClass)
		{
			CreateWidget<UUserWidget>(GetWorld(), WinWidgetClass)->AddToViewport();
		}
		UE_LOG(LogTemp, Log, TEXT("You Win!!!"));
	}
	if (Win == false)
	{
		// Create the widget and add it to the class here
		if (LoseWidgetClass)
		{
			CreateWidget<UUserWidget>(GetWorld(), LoseWidgetClass)->AddToViewport();
		}
		UE_LOG(LogTemp, Log, TEXT("You Lost!!!"));
	}
}

void AVehicleGameMode::CrossFinishLine()
{
	GameEnd(true);
}

void AVehicleGameMode::OutOfTime()
{
	// Delay of 3 seconds before calling the GameStart function
	TimeRemaining = 10.0f;

	// Create a TimerHandle to manage the timer
	FTimerHandle TimerHandle;

	// Define the TimerDelegate with the GameStart function as the target
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([this, &TimerHandle]()
							 {
								 // Check if TimeRemaining has reached zero
								 if (TimeRemaining <= 0.0f)
								 {
									 // If the timer runs out, call the GameEnd function with win set to false
									 GameEnd(false);
									 GetWorldTimerManager().ClearTimer(TimerHandle); // seem to have an issue with clearning the timer
								 }
								 else
								 {
									 // Decrement TimeRemaining by 1 second
									 TimeRemaining -= 1.0f;
								 } });

	// Start the repeating timer with an interval of 1 second
	float TimerInterval = 1.10f;
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, TimerInterval, true);
}

// this adds timers
void AVehicleGameMode::AddTimeToTimerDuration(float TimeToAdd)
{
	TimeRemaining += TimeToAdd;
}
