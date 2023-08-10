// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WinScreenWidget.generated.h" // should always be the last include

/**
 * 
 */
UCLASS()
class VEHICLEMASTERED_API UWinScreenWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	// this means the blueprint must include a textbox with the name WinText
	UPROPERTY(meta = ( BindWidget ))
	class UTextBlock* WinText; 

	// this means the blueprint must include a textbox with the name TimeLabel
	UPROPERTY(meta = ( BindWidget ))
	class UTextBlock* RestartGameText;

	UPROPERTY(meta = ( BindWidget ))
	class UButton* RestartGameButton;

	UPROPERTY(meta = ( BindWidget ))
	class UCanvasPanel* CanvasPanel; 

	void NativeConstruct() override;

	UFUNCTION()
	void RestartGame();
};
