// Fill out your copyright notice in the Description page of Project Settings.

// The AWheeledVehiclePawn comes with a UWheeledVehicleMovementComponent

#include "VehiclePawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "ChaosVehicleMovementComponent.h" // this is used now instead of WheeledVehicleMovementComponent4W.h
#include "CarWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"


// make the inputs variables so we I don't mistype
// static const FName NAME_SteerInput("Steer");
// static const FName NAME_ThrottleInput("Throttle");


AVehiclePawn::AVehiclePawn() // constructor
{
    // this gets the vehicle movement component
    // UChaosWheeledVehicleMovementComponent* Vehicle = CastChecked<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement());
   
    // Create a spring arm component for our chase camera
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = 250.0f;
    SpringArm->bUsePawnControlRotation = true;

    // create the chase camera component 
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ChaseCamera"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
    Camera->FieldOfView = 90.f;
}

void AVehiclePawn::BeginPlay()
{
    Super::BeginPlay();

    
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    APlayerCameraManager* PlayerCameraManager = PlayerController->PlayerCameraManager;
    PlayerCameraManager->StartCameraFade(1.0f, 1.0f, 3.0f, FLinearColor::Black);

    
}

void AVehiclePawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UpdateInAirControl(DeltaTime);
}

void AVehiclePawn::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // bindings
    PlayerInputComponent->BindAxis("Throttle", this, &AVehiclePawn::ApplyThrottle);
    PlayerInputComponent->BindAxis("Reverse", this, &AVehiclePawn::ApplyReverse);
    PlayerInputComponent->BindAxis("Steer", this, &AVehiclePawn::ApplySteering);
    PlayerInputComponent->BindAxis("LookUp", this, &AVehiclePawn::LookUp);
    PlayerInputComponent->BindAxis("Turn", this, &AVehiclePawn::Turn);

    PlayerInputComponent->BindAction("Handbrake", IE_Pressed, this, &AVehiclePawn::OnHandbrakePressed);
    PlayerInputComponent->BindAction("Handbrake", IE_Released, this, &AVehiclePawn::OnHandbrakeReleased);
    PlayerInputComponent->BindAction("Quit", IE_Pressed, this, &AVehiclePawn::QuitGame);


}

void AVehiclePawn::ApplyThrottle(float val)
{

     // Print the throttle input value for debugging

    GetVehicleMovementComponent()->SetThrottleInput(val);
}

void AVehiclePawn::ApplySteering(float val)
{
    GetVehicleMovementComponent()->SetSteeringInput(val);
}

void AVehiclePawn::ApplyReverse(float val)
{
    GetVehicleMovementComponent()->SetBrakeInput(val);
}

void AVehiclePawn::LookUp(float val)
{
    if (val != 0.0f)
    {
        AddControllerPitchInput(val);
    }
}

void AVehiclePawn::Turn(float val)
{
    if (val != 0.0f)
    {
        AddControllerYawInput(val);
    }
}

void AVehiclePawn::OnHandbrakePressed()
{
    GetVehicleMovementComponent()->SetHandbrakeInput(true);
}

void AVehiclePawn::OnHandbrakeReleased()
{
    GetVehicleMovementComponent()->SetHandbrakeInput(false);
}

void AVehiclePawn::QuitGame()
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController)
    {
        PlayerController->ConsoleCommand("quit");
    }
}

void AVehiclePawn::ShowWidgetOnTimeUp()
{
    // Make sure to check if the widget class is valid before creating it.
    if (WidgetClass)
    {
        // Create the widget and store the instance in the variable.
        MainWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);

        // Check if the created widget instance is valid before adding it to the viewport.
        if (MainWidgetInstance)
        {
            // Add the widget to the viewport.
            MainWidgetInstance->AddToViewport();
        }
    }
}

void AVehiclePawn::ShowLoadingWidget()
{
    // Make sure to check if the widget class is valid before creating it.
    if (LoadingWidgetClass)
    {
        // Create the widget and store the instance in the variable.
        LoadingWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), LoadingWidgetClass);

        // Check if the created widget instance is valid before adding it to the viewport.
        if (LoadingWidgetInstance)
        {
            // Add the widget to the viewport.
            LoadingWidgetInstance->AddToViewport();
        }
    }
}

void AVehiclePawn::HideWidgetOnTimeUp()
{
    if (MainWidgetInstance)
    {
        // Remove the widget from the viewport.
        MainWidgetInstance->RemoveFromViewport();
        MainWidgetInstance = nullptr; // Set the pointer to nullptr to avoid using invalid references.
    }
}

void AVehiclePawn::HideLoadingWidget()
{
    if (LoadingWidgetInstance)
    {
        // Remove the widget from the viewport.
        LoadingWidgetInstance->RemoveFromViewport();
        LoadingWidgetInstance = nullptr; // Set the pointer to nullptr to avoid using invalid references.
    }
}

void AVehiclePawn::UpdateInAirControl(float DeltaTime)
{
    // if (GetVehicleMovementComponent())
    // {
    //     FCollisionQueryParams QueryParams;
    //     QueryParams.AddIgnoredActor(this);

    //     const FVector TraceStart = GetActorLocation() + FVector(0.f, 0.f, 50.f);
    //     const FVector TraceEnd = GetActorLocation() + FVector(0.f, 0.f, 200.f);

    //     FHitResult Hit;

    //     // check if car is flipped on its side, and check if the car is in the air
    //     const bool bInAir = !GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, QueryParams);
    //     // compares car upvector to the global upvector using dot product if less than 0.1f it's flipped
    //     const bool bNotGrounded = FVector::DotProduct(GetActorUpVector(), FVector::UpVector) < 0.1f; 

    //     // only allow in-air movement if we are not on the ground, or in the air
    //     if (bInAir || bNotGrounded)
    //     {
    //         const float ForwardInput = InputComponent->GetAxisValue("Throttle");
    //         const float SteerInput = InputComponent->GetAxisValue("Steer");

    //         // If the car is grounded allow the player to roll the car over
    //         const float AirMovementForcePitch = 3.f;
    //         const float AirMovementForceRoll = !bInAir && bNotGrounded ? 20.f : 3.f; // if we are rolled over need to increase the force

    //         // taking the input values for throttle and steering and multiplying them against pitch and roll values
    //         // z value is left as 0 here, as we don't care about yaw. Only want to influence the pitch and roll.
    //         // multiply the values by delta time to make it frame rate independent 
    //         const FVector MovementVector = FVector(SteerInput * -AirMovementForceRoll, ForwardInput * AirMovementForcePitch, 0.f) * DeltaTime * 200.f; 

    //         // this applies the movement vector to our car actor.
    //         const FVector NewAngularMovement = GetActorRotation().RotateVector(MovementVector);

    //         GetMesh()->SetPhysicsAngularVelocityInDegrees(NewAngularMovement, true); // the true adds this velocity onto the vehicles current velocity
        // }
    // }
}
