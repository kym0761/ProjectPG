// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));

}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (CurrentHP <= 0.0f) // 이미 죽었으면 처리 안함.
	{
		return 0.0f;
	}

	//UE_LOG(LogClass, Warning, TEXT("Damage : %f"), DamageAmount);

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		FPointDamageEvent* pointEvent = (FPointDamageEvent*)(&DamageEvent);

		//UE_LOG(LogClass, Warning, TEXT("Point Damage -----> Bone Name : %s"), *pointEvent->HitInfo.BoneName.ToString());

		if (pointEvent->HitInfo.BoneName.Compare(TEXT("head")) == 0)
		{
			CurrentHP = 0.0f;
		}
		else
		{
			CurrentHP = FMath::Clamp<float>(CurrentHP - DamageAmount, 0.0f, MaxHP);
		}

	}
	else if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{
		FRadialDamageEvent* radialEvent = (FRadialDamageEvent*)(&DamageEvent);

		UE_LOG(LogClass, Warning, TEXT("Radial Damage"));
	}
	else
	{
		CurrentHP = FMath::Clamp<float>(CurrentHP - DamageAmount, 0.0f, MaxHP);
	}



	if (CurrentHP <= 0.0f)
	{
		//Destroy();

		////physics
		//GetMesh()->SetSimulatePhysics(true);
		//GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//GetMesh()->AddImpulse(impulseDirection* 30000.0f, TEXT("head"));
		//SetLifeSpan(5.0f);



	}
	else
	{

	}

	return 0.0f;
}

