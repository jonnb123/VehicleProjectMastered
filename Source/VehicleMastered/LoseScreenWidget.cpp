// Fill out your copyright notice in the Description page of Project Settings.

#include "LoseScreenWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UMG/Public/Components/Button.h"

void ULoseScreenWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (RestartGameButton)
    {
        RestartGameButton->OnClicked.AddDynamic(this, &ULoseScreenWidget::RestartGame);
    }
}

void ULoseScreenWidget::RestartGame()
{
    // Get the player controller
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

    // Open the level by name (replace "YourLevelName" with the actual name of the level you want to open)
    UGameplayStatics::OpenLevel(PlayerController, FName("CarLevel"));
}
