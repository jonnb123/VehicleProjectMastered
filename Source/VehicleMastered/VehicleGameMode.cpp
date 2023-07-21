// Fill out your copyright notice in the Description page of Project Settings.

#include "VehicleGameMode.h"
#include "Checkpoint.h"
#include "Kismet/GameplayStatics.h"

void AVehicleGameMode::BeginPlay()
{
	// TArray<AActor *> FoundActors;
	// UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACheckpoint::StaticClass(), FoundActors);

	// // Now you have an array of all actors of the specified class.
	// for (AActor *Actor : FoundActors)
	// {
	// 	ACheckpoint* Checkpoint = Cast<ACheckpoint>(Actor);
	// 	if (Checkpoint)
	// 	{
	// 		// Do something with the Checkpoint actor.
	// 		// For example, you can call functions on it or access its properties.
	// 	}
	// }

	// start the 5 second countdown timer
	StartCountdown();
	OutOfTime();
}

void AVehicleGameMode::GameStart()
{
	UE_LOG(LogTemp, Log, TEXT("The game is starting")); // this is called after the startcountdown, i.e. after 3 seconds
}

void AVehicleGameMode::GameEnd(bool Win)
{
	if (Win == true)
	{
		UE_LOG(LogTemp, Log, TEXT("You Win!!!"));
	}
	if (Win == false)
	{
		UE_LOG(LogTemp, Log, TEXT("You Lost!!!"));
	}
}

void AVehicleGameMode::CrossFinishLine()
{
	GameEnd(true);
}

void AVehicleGameMode::StartCountdown()
{
	// Delay of 3 seconds before calling the GameStart function
	float BeginCountdown = 3.0f;

	// Create a TimerHandle to manage the timer
	FTimerHandle TimerHandle;

	// Define the TimerDelegate with the GameStart function as the target
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &AVehicleGameMode::GameStart);

	// Start the timer
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, BeginCountdown, false);
}

void AVehicleGameMode::OutOfTime()
{
	// Delay of 3 seconds before calling the GameStart function
	TimerDuration = 7.0f;

	// Create a TimerHandle to manage the timer
	FTimerHandle TimerHandle;

	// Define the TimerDelegate with the GameStart function as the target
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([this]()
							 {
								 GameEnd(false); // if the timer runs out, then win is false
							 });

	// Start the timer
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, TimerDuration, false);
}
