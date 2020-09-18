// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BattlePlayerController.generated.h"

class UBattleWidgetBase;
/**
 * 
 */
UCLASS()
class PROJECTPG_API ABattlePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
		TSubclassOf<UBattleWidgetBase> BattleWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
		UBattleWidgetBase* BattleWidgetObject;

	virtual void BeginPlay() override;

};
