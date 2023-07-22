// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
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

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent *PlayerInputComponent) override; // registers when player presses wasd

	// throttle and steering, where val is how much throttle or steering to apply
	void ApplyThrottle(float val);
	void ApplySteering(float val);
	void ApplyReverse(float val);

	// Look around
	void LookUp(float val);
	void Turn(float val);

	// handbrake
	void OnHandbrakePressed();
	void OnHandbrakeReleased();

	// Update in-air physics, this uses the tick-function
	void UpdateInAirControl(float DeltaTime);

	// add the widget, this is technically of type UCarWidget but using TSubclass with UUserWidget works too
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> WidgetClass;

protected:
	// spring arm that will offset the camera
	UPROPERTY(Category = Camera, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent *SpringArm;

	// spring arm that will offset the camera
	UPROPERTY(Category = Camera, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent *Camera;
};
