// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicAnimInstance.h"
#include "BasicCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBasicAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	ABasicCharacter* character = Cast<ABasicCharacter>(TryGetPawnOwner());
	if (character && character->IsValidLowLevel())
	{
		Direction = CalculateDirection(character->GetCharacterMovement()->Velocity, character->GetActorRotation());

		Speed = character->GetCharacterMovement()->Velocity.Size();

		bIsFire = character->bIsFire;
		bIsSprint = character->bIsSprint;
		bIsIronsight = character->bIsIronsight;
		bIsCrouched = character->bIsCrouched;
	}
}