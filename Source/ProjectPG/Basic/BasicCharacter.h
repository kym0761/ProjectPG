// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BasicCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UWeaponComponent;
UCLASS()
class PROJECTPG_API ABasicCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABasicCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UWeaponComponent* Weapon;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float WalkSpeed = 300.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float SprintSpeed = 600.0f;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);

	void Sprint();
	void Unsprint();


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "status")
		uint64 bIsFire : 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "status")
		uint64 bIsSprint : 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "status")
		uint64 bIsIronsight : 1;


	void StartFire();
	void EndFire();

	void OnFire();

	void StartIronsight();
	void EndIronsight();

	void StartCrouch(); // toggle crouch

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HP")
	float CurrentHP;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HP")
	float MaxHP;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
		UAnimMontage* HitReactMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpringArm")
	FVector NormalSpringArmPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpringArm")
	FVector CrouchSpringArmPosition;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LineTrace")
	TArray<TEnumAsByte<EObjectTypeQuery>> Objects;


	FTimerHandle FireTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UParticleSystem* HitEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UParticleSystem* BloodEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	USoundBase* WeaponSound;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* ReloadMontage;

	void Reload();


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Decal")
		UMaterialInstance* NormalDecal;




	void StartLeftLean();
	void EndLeftLean();

	void StartRightLean();
	void EndRightLean();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "status")
	uint8 bLeftLean : 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "status")
	uint8 bRightLean : 1;

	FRotator GetAimOffset() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CameraShake")
	TSubclassOf<UCameraShake> WorldCameraShake;

};
