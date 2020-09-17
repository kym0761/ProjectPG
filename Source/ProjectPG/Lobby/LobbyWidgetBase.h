// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidgetBase.generated.h"

class UEditableTextBox;
class UScrollBox;

/**
 * 
 */
UCLASS()
class PROJECTPG_API ULobbyWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual void NativeConstruct() override;
	


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UEditableTextBox* ChatInput;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UScrollBox* ChatBox;

	UFUNCTION()
	void ProcessTextCommit(const FText& Text, ETextCommit::Type CommitMethod);

	void AddMessage(FText Message);
};
