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
#include "UMG/Public/Animation/WidgetAnimation.h"
#include "LoadingScreenWidget.h"
#include "ChaosWheeledVehicleMovementComponent.h"

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

    // Create the audio component and attach it to the root component
    EngineAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("EngineAudioComponent"));
    EngineAudioComponent->SetupAttachment(RootComponent);
}

void AVehiclePawn::BeginPlay()
{
    Super::BeginPlay();
}

void AVehiclePawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    SetupEngineAudio();

    UpdateInAirControl(DeltaTime);
}

void AVehiclePawn::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // bindings - these are deprecated, will need upgrading
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
    APlayerController *PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
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
        LoadingWidgetInstance = CreateWidget<ULoadingScreenWidget>(GetWorld(), LoadingWidgetClass);

        // Check if the created widget instance is valid before adding it to the viewport.
        if (LoadingWidgetInstance)
        {
            // Add the widget to the viewport.
            LoadingWidgetInstance->AddToViewport();

            LoadingWidgetInstance->PlayAnimationForward(LoadingWidgetInstance->FadeOff);
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
    if (GetVehicleMovementComponent())
    {
        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActor(this);

        const FVector TraceStart = GetActorLocation() + FVector(0.f, 0.f, 500.f); // go higher up the car, at 0 it doesn't work
        const FVector TraceEnd = GetActorLocation() + FVector(0.f, 0.f, 200.f);

        FHitResult Hit;
        DrawDebugLine(
            GetWorld(),    // The World context in which to draw the debug line
            TraceStart,    // The starting point of the line trace
            TraceEnd,      // The ending point of the line trace
            FColor::Green, // The color of the debug line (green in this case)
            false,         // Whether to persist the debug line (false means it will disappear after a short duration)
            -1.f,          // The duration for which the debug line will be visible (-1.f means it will be visible for a short duration)
            0,             // The depth priority of the debug line (0 is the default value)
            2.f            // The thickness of the debug line
        );

        // check if car is flipped on its side, and check if the car is in the air
        // const bool bInAir = !GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, QueryParams);
        const bool bInAir = !GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, QueryParams);
        // compares car upvector to the global upvector using dot product if less than 0.1f it's flipped
        const bool bNotGrounded = FVector::DotProduct(GetActorUpVector(), FVector::UpVector) < 0.1f;
        UE_LOG(LogTemp, Warning, TEXT("bInAir: %s"), bInAir ? TEXT("true") : TEXT("false"));
        UE_LOG(LogTemp, Warning, TEXT("bNotGrounded: %s"), bNotGrounded ? TEXT("true") : TEXT("false"));
        // only allow in-air movement if we are not on the ground, or in the air
        if (bInAir || bNotGrounded)
        {
            const float ForwardInput = InputComponent->GetAxisValue("Throttle");
            const float SteerInput = InputComponent->GetAxisValue("Steer");

            // If the car is grounded allow the player to roll the car over
            const float AirMovementForcePitch = 1.5f; // this is front and back flip
            // const float AirMovementForcePitch = !bInAir && bNotGrounded ? 10.f : 1.5f; // this is front and back flip
            const float AirMovementForceRoll = !bInAir && bNotGrounded ? 20.f : 1.5f; // side flip - if not in the air and not grounded increase the force
            UE_LOG(LogTemp, Warning, TEXT("AirMovementForceRoll: %f"), AirMovementForceRoll);
           

            // taking the input values for throttle and steering and multiplying them against pitch and roll values
            // z value is left as 0 here, as we don't care about yaw. Only want to influence the pitch and roll.
            // multiply the values by delta time to make it frame rate independent
            const FVector MovementVector = FVector(SteerInput * -AirMovementForceRoll, ForwardInput * AirMovementForcePitch, 0.f) * DeltaTime * 200.f;

            // this applies the movement vector to our car actor.
            const FVector NewAngularMovement = GetActorRotation().RotateVector(MovementVector);

            GetMesh()->SetPhysicsAngularVelocityInDegrees(NewAngularMovement, true); // the true adds this velocity onto the vehicles current velocity
        }
    }
}

void AVehiclePawn::SetupEngineAudio()
{
    UChaosWheeledVehicleMovementComponent *ChaosVehicleMovementComponent = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent());
    // The engine rotation speed
    float EngineRotationSpeed = ChaosVehicleMovementComponent->GetEngineRotationSpeed();

    // Define the original range of the EngineRotationSpeed (from minimum to maximum value it can have)
    float MinOriginalSpeed = 1000.0f;
    float MaxOriginalSpeed = 9000.0f; // Adjust this to the actual range of EngineRotationSpeed

    // Define the target range you want to map the EngineRotationSpeed to (from minimum to maximum value)
    float MinTargetSpeed = 0.0f; // Adjust this to the minimum value of the target range
    float MaxTargetSpeed = 1.0f; // Adjust this to the maximum value of the target range

    // Map and clamp the EngineRotationSpeed to the target range
    float MappedSpeed = FMath::GetMappedRangeValueClamped(FVector2D(MinOriginalSpeed, MaxOriginalSpeed), FVector2D(MinTargetSpeed, MaxTargetSpeed), EngineRotationSpeed);
    EngineAudioComponent->SetFloatParameter(FName(TEXT("RPM Range")), MappedSpeed);
}
