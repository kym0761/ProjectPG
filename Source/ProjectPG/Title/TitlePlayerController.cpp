// Fill out your copyright notice in the Description page of Project Settings.


#include "TitlePlayerController.h"
#include "Blueprint/UserWidget.h" //CreateWidget
#include "TitleWidgetBase.h"
void ATitlePlayerController::BeginPlay()
{
	if (TitleWidgetClass)
	{
		TitleWidgetObject = CreateWidget<UTitleWidgetBase>(this, TitleWidgetClass);
		TitleWidgetObject->AddToViewport();
	
		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
	}
	else
	{
		UE_LOG(LogClass, Warning, TEXT("You Must Add TitleWidgetClass"));
	}
}