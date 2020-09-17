// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"
#include "LobbyWidgetBase.h"
void ALobbyPlayerController::BeginPlay()
{

	Super::BeginPlay();

	if (!IsLocalPlayerController())
	{
		return;
	}

	if (LobbyWidgetClass)
	{
		LobbyWidgetObject = CreateWidget<ULobbyWidgetBase>(this, LobbyWidgetClass);
		if (LobbyWidgetObject)
		{
			LobbyWidgetObject->AddToViewport();
		}
	}
	else
	{
		UE_LOG(LogClass, Warning, TEXT("You Must Add LobbyWidgetClass"));
	}

}