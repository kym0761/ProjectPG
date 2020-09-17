// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidgetBase.h"
#include "Components/EditableTextBox.h"
#include "LobbyPlayerController.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "../PGGameInstance.h"
#include "Kismet/GameplayStatics.h"
void ULobbyWidgetBase::NativeConstruct()
{

	Super::NativeConstruct();

	ChatInput = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("MessageInputBox")));

	if (ChatInput)
	{
		ChatInput->OnTextCommitted.AddDynamic(this, &ULobbyWidgetBase::ProcessTextCommit);
	}

	ChatBox = Cast<UScrollBox>(GetWidgetFromName(TEXT("ChatBox")));
}

void ULobbyWidgetBase::ProcessTextCommit(const FText & Text, ETextCommit::Type CommitMethod)
{
	switch(CommitMethod)
	{
	case ETextCommit::OnEnter:
	{//local -> host
		ALobbyPlayerController* pc = GetOwningPlayer<ALobbyPlayerController>();
		if (pc)
		{

			UPGGameInstance* gameInst = Cast
				<UPGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
			if (gameInst)
			{
				FString temp = FString::Printf(TEXT("%s : %s"), *gameInst->GetUserID(), *Text.ToString());
				pc->C2S_SendMessage(FText::FromString(temp));
				ChatInput->SetText(FText::FromString(TEXT("")));
			}


		}
	}
		break;
	case ETextCommit::OnCleared:
	{
		ChatInput->SetUserFocus(GetOwningPlayer());
	}
		break;
	default:

		break;
	}
}

void ULobbyWidgetBase::AddMessage(FText Message)
{
	if (ChatBox)
	{
		UTextBlock* newTextBlock = NewObject<UTextBlock>(ChatBox);
		if (newTextBlock)
		{
			newTextBlock->SetText(Message);
			newTextBlock->Font.Size = 18;
			ChatBox->AddChild(newTextBlock);
			ChatBox->ScrollToEnd();
		}
	}
}
