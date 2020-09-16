// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TitleWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPG_API UTitleWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	//UMG와 Control 변수 연결.
	virtual void NativeConstruct() override;
};
