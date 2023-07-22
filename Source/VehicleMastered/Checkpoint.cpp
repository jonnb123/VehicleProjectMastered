// Fill out your copyright notice in the Description page of Project Settings.

#include "Checkpoint.h"
#include "VehicleGameMode.h"
#include "VehiclePawn.h"

// Sets default values
ACheckpoint::ACheckpoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this actor to call Tick() every frame
	PrimaryActorTick.bCanEverTick = false;

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

	// Bind the OnBoxBeginOverlap function to the OnComponentBeginOverlap event
	BoxCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::OnBoxBeginOverlap);


}

// this function is bound in begin play, without binding it it doesn't work
void ACheckpoint::OnBoxBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
									int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	// Get the specific Game Mode class directly
	AVehicleGameMode *GameMode = GetWorld()->GetAuthGameMode<AVehicleGameMode>();

	// Check if the OtherActor is the player's vehicle (you might need to adjust this check based on your game logic)
	if (OtherActor && OtherActor->IsA<AVehiclePawn>())
	{
		// Cast the OtherActor to your vehicle pawn class (assuming AVehiclePawn is your vehicle pawn class)
		AVehiclePawn* VehiclePawn = Cast<AVehiclePawn>(OtherActor);
		if (VehiclePawn)
		{
			// Check if this checkpoint is a finish line
			if (IsFinishLine)
			{
				// Add your comment here, or do any other action you want when the player drives through a finish line
				UE_LOG(LogTemp, Log, TEXT("Congratulations! You crossed the finish line!"));
				GameMode->CrossFinishLine();
			}
			else
			{
				// Add any other logic you want to perform when the player drives through a regular checkpoint
				// (not the finish line)
				UE_LOG(LogTemp, Log, TEXT("You drove through a regular checkpoint."));
				GameMode->AddTimeToTimerDuration(5.0f);
			}
			Destroy(); // destroy the checkpoint after crossing
		}
	}
}

// Called every frame
void ACheckpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
