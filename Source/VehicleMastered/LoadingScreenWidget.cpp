// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingScreenWidget.h"
#include "VehicleGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "UMG/Public/Animation/WidgetAnimation.h"
#include "VehiclePawn.h"

UWidgetAnimation* ULoadingScreenWidget::GetFadeOffAnimation() const
{
	return FadeOff;
}

