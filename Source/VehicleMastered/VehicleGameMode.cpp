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
	APlayerController *PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AVehiclePawn *MyVehiclePawn = Cast<AVehiclePawn>(PlayerController->GetPawn());
	MyVehiclePawn->ShowLoadingWidget();
	LoadGame();
}

void AVehicleGameMode::LoadGame()
{
	// Set a  delay before calling GameStart function
	DelayTime = 1.5;

	StartTimerDelegate.BindLambda([this]()
								  {
	APlayerController *PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AVehiclePawn *MyVehiclePawn = Cast<AVehiclePawn>(PlayerController->GetPawn());
	MyVehiclePawn->HideLoadingWidget();
	MyVehiclePawn->ShowWidgetOnTimeUp();
		StartGame(); 
		}); // once the timer is complete call the main game timer!
	GetWorldTimerManager().SetTimer(StartTimerHandle, StartTimerDelegate, DelayTime, false);
}

void AVehicleGameMode::StartGame()
{
	TimeRemaining = 25.0f;

	TimerDelegate.BindLambda([this]()
							 {
								 // Check if TimeRemaining has reached zero
								 if (TimeRemaining <= 0.0f)
								 {
									// If the timer runs out, call the GameEnd function with win set to false
									GameEnd(false);
									GetWorldTimerManager().ClearTimer(TimerHandle); 
								 }
								 else
								 {
									 // Decrement TimeRemaining by 1 second
									 TimeRemaining -= 1.0f;
								 } });

	// Start the repeating timer with an interval of 1 second
	float TimerInterval = 1.0f;
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, TimerInterval, true);
}

void AVehicleGameMode::GameEnd(bool Win)
{
	DisablePlayerInput();
	// Get the default player controller
	APlayerController *PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AVehiclePawn *MyVehiclePawn = Cast<AVehiclePawn>(PlayerController->GetPawn());
	MyVehiclePawn->HideWidgetOnTimeUp();
	PlayerController->SetShowMouseCursor(true);
	if (Win == true)
	{
		GetWorldTimerManager().ClearTimer(TimerHandle);
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

void AVehicleGameMode::DisablePlayerInput()
{
	APlayerController *PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	// Get the pawn possessed by the player controller (should be your AVehiclePawn)
	AVehiclePawn *MyVehiclePawn = Cast<AVehiclePawn>(PlayerController->GetPawn());
	MyVehiclePawn->DisableInput(PlayerController);
	MyVehiclePawn->GetVehicleMovementComponent()->SetSteeringInput(0.0f);
	MyVehiclePawn->GetVehicleMovementComponent()->SetThrottleInput(0.0f);
	MyVehiclePawn->GetVehicleMovementComponent()->SetBrakeInput(0.0f);
}


// This is used in Checkpoint.cpp
void AVehicleGameMode::CrossFinishLine()
{
	GameEnd(true);
}



// This is used in Checkpoint.cpp
void AVehicleGameMode::AddTimeToTimerDuration(float TimeToAdd)
{
	TimeRemaining += TimeToAdd;
}
