// Fill out your copyright notice in the Description page of Project Settings.


#include "BattlePlayerController.h"
#include "BattleWidgetBase.h"
void ABattlePlayerController::BeginPlay()
{
	if (IsLocalPlayerController())
	{
		if (BattleWidgetClass)
		{
			BattleWidgetObject = CreateWidget<UBattleWidgetBase>(this, BattleWidgetClass);
			if (BattleWidgetObject)
			{
				BattleWidgetObject->AddToViewport();
				bShowMouseCursor = false;
				SetInputMode(FInputModeGameOnly());
			}
		}
	}
}
