// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "Components/AudioComponent.h"
#include "VehiclePawn.generated.h"

/**
 *
 */
UCLASS()
class VEHICLEMASTERED_API AVehiclePawn : public AWheeledVehiclePawn // using the ChaosVehiclesPlugin we can inherit from AWheeledVehiclePawn
{
	GENERATED_BODY()

public:
	AVehiclePawn(); // Constructor

	// widget functions
	void ShowWidgetOnTimeUp();
	void ShowLoadingWidget();
	void HideWidgetOnTimeUp();
	void HideLoadingWidget();
	

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
	// Input 
	virtual void SetupPlayerInputComponent(UInputComponent *PlayerInputComponent) override; 

	void ApplyThrottle(float val);

	void ApplySteering(float val);

	void ApplyReverse(float val);

	void LookUp(float val);

	void Turn(float val);

	void OnHandbrakePressed();

	void OnHandbrakeReleased();

	// This is called in Tick
	void EngineAudio();

	// Update in-air physics, this uses the tick-function
	void UpdateInAirControl(float DeltaTime);

	// Quit
	void QuitGame();

	// spring arm that will offset the camera
	UPROPERTY(Category = Camera, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent *SpringArm;

	// spring arm that will offset the camera
	UPROPERTY(Category = Camera, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent *Camera;

	// Instances of widgets

	// this is technically of type UCarWidget but using TSubclass with UUserWidget works too
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> WidgetClass;
	UUserWidget *MainWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ULoadingScreenWidget> LoadingWidgetClass;
	ULoadingScreenWidget *LoadingWidgetInstance;

	// Audio
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAudioComponent *EngineAudioComponent;

	


	
};
