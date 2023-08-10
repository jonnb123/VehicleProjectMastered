// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VehicleGameMode.generated.h"

/**
 *
 */
UCLASS()
class VEHICLEMASTERED_API AVehicleGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Functions 
	UFUNCTION(BlueprintCallable)
	void LoadGame();

	UFUNCTION(BlueprintCallable)
	void StartGame();

	UFUNCTION(BlueprintCallable)
	void GameEnd(bool Win);

	UFUNCTION(BlueprintCallable)
	void DisablePlayerInput();
	
	// Widget instances
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> WinWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> LoseWidgetClass;

	// for the start countdown
	FTimerHandle StartTimerHandle;
	FTimerDelegate StartTimerDelegate;
	UPROPERTY(EditAnywhere)
	float DelayTime;

public:
	// Functions

	// Used in checkpoint.cpp keep public
	UFUNCTION(BlueprintCallable)
	void CrossFinishLine();

	// Used in checkpoint.cpp keep public
	UFUNCTION(BlueprintCallable, Category = "Timer")
	void AddTimeToTimerDuration(float TimeToAdd);

	// for the overall game timer
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	UPROPERTY(EditAnywhere)
	float TimeRemaining;
	

};
