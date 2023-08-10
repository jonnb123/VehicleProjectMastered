// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CarWidget.generated.h"

UCLASS()
class VEHICLEMASTERED_API UCarWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	// this means the blueprint must include a textbox with the name TimeLabel
	UPROPERTY(meta = ( BindWidget ))
	class UTextBlock* TimeLabel; // forward declared so no need to include header

	UPROPERTY(meta = ( BindWidget ))
	class UTextBlock* TimeRemainingText;

	UPROPERTY(meta = ( BindWidget ))
	class UTextBlock* SpeedLabel;

	UPROPERTY(meta = ( BindWidget ))
	class UCanvasPanel* CanvasPanel; 

	void NativeConstruct() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay();

	UFUNCTION(BlueprintCallable)
	void UpdateTimeLabelText();

	UFUNCTION(BlueprintCallable)
	void UpdateSpeedText();

};
