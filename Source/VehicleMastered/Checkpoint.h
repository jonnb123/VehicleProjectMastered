// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Checkpoint.generated.h"


UCLASS()
class VEHICLEMASTERED_API ACheckpoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACheckpoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Root component for the actor
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USceneComponent* RootSceneComponent;

    // Child cube components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* Cube1MeshComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* Cube2MeshComponent;

    // Child box collision component
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* BoxCollisionComponent;

	// Function to handle the begin overlap event
    UFUNCTION()
    void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    // is this the finish line?
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (InstanceEditable = "true"))
    bool IsFinishLine = false;

    // Time to add
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (InstanceEditable = "true"))
    int TimeToAdd = 10;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
