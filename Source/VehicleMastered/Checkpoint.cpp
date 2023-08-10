// Fill out your copyright notice in the Description page of Project Settings.

#include "Checkpoint.h"
#include "VehicleGameMode.h"
#include "VehiclePawn.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACheckpoint::ACheckpoint()
{
	// Create the Root Scene Component
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	RootComponent = RootSceneComponent;

	// Create the first cube and attach it to the root
	Cube1MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube1MeshComponent"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube1MeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));
	if (Cube1MeshAsset.Succeeded())
	{
		Cube1MeshComponent->SetStaticMesh(Cube1MeshAsset.Object);
	}
	Cube1MeshComponent->AttachToComponent(RootSceneComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// Create the second cube and attach it to the root
	Cube2MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube2MeshComponent"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube2MeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));
	if (Cube2MeshAsset.Succeeded())
	{
		Cube2MeshComponent->SetStaticMesh(Cube2MeshAsset.Object);
	}
	Cube2MeshComponent->AttachToComponent(RootSceneComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// Create the box collision and attach it to the root
	BoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollisionComponent"));
	BoxCollisionComponent->InitBoxExtent(FVector(100.f, 100.f, 100.f));
	BoxCollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	BoxCollisionComponent->AttachToComponent(RootSceneComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();

	BoxCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::OnBoxBeginOverlap);
}


void ACheckpoint::OnBoxBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
									int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	// Play the sound at the location of the checkpoint if the sound cue is valid
	if (CheckpointSoundCue)
	{
		UGameplayStatics::PlaySound2D(this, CheckpointSoundCue);
	}
	// Get the specific Game Mode class directly
	AVehicleGameMode *GameMode = GetWorld()->GetAuthGameMode<AVehicleGameMode>();

	// Check if the OtherActor is the car
	if (OtherActor && OtherActor->IsA<AVehiclePawn>())
	{
		AVehiclePawn* VehiclePawn = Cast<AVehiclePawn>(OtherActor);
		if (VehiclePawn)
		{
			if (IsFinishLine)
			{
				UE_LOG(LogTemp, Log, TEXT("Congratulations! You crossed the finish line!"));
				GameMode->CrossFinishLine();
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("You drove through a regular checkpoint."));
				GameMode->AddTimeToTimerDuration(5.0f);
			}
			Destroy(); 
		}
	}
}

