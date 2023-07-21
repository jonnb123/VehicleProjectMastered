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

public:

	UFUNCTION(BlueprintCallable)
	void GameStart();

	UFUNCTION(BlueprintCallable)
	void GameEnd(bool Win);

	UFUNCTION(BlueprintCallable)
	void CrossFinishLine();

	UFUNCTION(BlueprintCallable)
	void StartCountdown();

	UFUNCTION(BlueprintCallable)
	void OutOfTime();

	UPROPERTY(EditAnywhere)
	float TimerDuration;


};