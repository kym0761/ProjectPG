// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "WeaponComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "BulletDamageType.h"
#include "Components/DecalComponent.h"

// Sets default values
ABasicCharacter::ABasicCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->TargetArmLength = 500.0f;
	SpringArm->SocketOffset = FVector(0.0f, 40.0f, 88.0f);
	//bUseControllerRotationYaw = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;


	Weapon = CreateDefaultSubobject<UWeaponComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), FName("WeaponSocket"));
	
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCharacterMovement()->CrouchedHalfHeight = 88.0f;

	NormalSpringArmPosition = SpringArm->GetRelativeLocation();
	CrouchSpringArmPosition = NormalSpringArmPosition + FVector(0.0f,0.0f,-GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()/2);

	CurrentHP = 100.0f;
	MaxHP = 100.0f;
}

// Called when the game starts or when spawned
void ABasicCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasicCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABasicCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Forward",this, &ABasicCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Right", this, &ABasicCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ABasicCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ABasicCharacter::LookUp);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABasicCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ABasicCharacter::StopJumping);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ABasicCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ABasicCharacter::Unsprint);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABasicCharacter::Jump);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ABasicCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ABasicCharacter::EndFire);
	PlayerInputComponent->BindAction("Ironsight", IE_Pressed, this, &ABasicCharacter::StartIronsight);
	PlayerInputComponent->BindAction("Ironsight", IE_Released, this, &ABasicCharacter::EndIronsight);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ABasicCharacter::StartCrouch);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ABasicCharacter::Reload);
	
	PlayerInputComponent->BindAction("LeftLean", IE_Pressed, this, &ABasicCharacter::StartLeftLean);
	PlayerInputComponent->BindAction("LeftLean", IE_Released, this, &ABasicCharacter::EndLeftLean);

	PlayerInputComponent->BindAction("RightLean", IE_Pressed, this, &ABasicCharacter::StartRightLean);
	PlayerInputComponent->BindAction("RightLean", IE_Released, this, &ABasicCharacter::EndRightLean);

}

void ABasicCharacter::MoveForward(float Value)
{

	FVector cameraLocation;
	FRotator cameraRotation;

	GetController()->GetPlayerViewPoint(cameraLocation, cameraRotation);


	//FRotator controlRot = FRotator(0.0f, GetControlRotation().Yaw, 0.0f);
	FRotator controlRot = FRotator(0.0f, cameraRotation.Yaw, 0.0f);

	FVector temp
		= UKismetMathLibrary::GetForwardVector(controlRot);

	AddMovementInput(temp, Value);
}

void ABasicCharacter::MoveRight(float Value)
{

	FVector cameraLocation;
	FRotator cameraRotation;

	GetController()->GetPlayerViewPoint(cameraLocation, cameraRotation);

	//FRotator controlRot = FRotator(0.0f, GetControlRotation().Yaw, 0.0f);
	FRotator controlRot = FRotator(0.0f, cameraRotation.Yaw, 0.0f);

	FVector temp
		= UKismetMathLibrary::GetRightVector(controlRot);

	AddMovementInput(temp, Value);
}

void ABasicCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void ABasicCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void ABasicCharacter::Sprint()
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void ABasicCharacter::Unsprint()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ABasicCharacter::StartFire()
{
	bIsFire = true;

	GetWorldTimerManager().SetTimer(FireTimer, this, &ABasicCharacter::OnFire, 0.12f, true, 0.0f);


	//OnFire();
}

void ABasicCharacter::EndFire()
{
	bIsFire = false;

	GetWorldTimerManager().ClearTimer(FireTimer);
}

void ABasicCharacter::OnFire()
{
	if (!bIsFire)
	{
		return;
	}


	APlayerController* playerController = Cast<APlayerController>(GetController());
	if (playerController)
	{
		int32 sizeX, sizeY;
		FVector crosshairLocation, crosshairDirection; // this is world location & direction.
		playerController->GetViewportSize(sizeX, sizeY);
		playerController->DeprojectScreenPositionToWorld(
			sizeX / 2, sizeY / 2, crosshairLocation, crosshairDirection);

		FVector cameraLocation;
		FRotator cameraRotation;
		playerController->GetPlayerViewPoint(cameraLocation, cameraRotation);

		FVector traceStart = cameraLocation;
		FVector traceEnd = traceStart + crosshairDirection * 100000.0f;

		TArray<TEnumAsByte<EObjectTypeQuery>> objects;

		objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
		objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
		//objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
		objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));


		TArray<AActor*> ignores;
		ignores.Add(this);

		FHitResult hit;
		
		bool result = UKismetSystemLibrary::LineTraceSingleForObjects(
			GetWorld(),
			traceStart,
			traceEnd,
			objects,
			true,
			ignores,
			EDrawDebugTrace::None, //EDrawDebugTrace::ForDuration,
			hit,
			true,
			FLinearColor::Red,
			FLinearColor::Blue,
			5.0f
		);

		if (result)
		{
			//UE_LOG(LogClass, Warning, TEXT("Hit : %s"), *hit.GetActor()->GetName());

			if (Cast<ACharacter>(hit.GetActor()))
			{

				if (BloodEffect)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodEffect, hit.ImpactPoint + (hit.ImpactNormal*10.0f));
				}
			}
			else
			{
				if (HitEffect)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, hit.ImpactPoint + (hit.ImpactNormal*10.0f));
				}

				//데칼은 스켈레톤 메시에 쓰면 안됨.
				UDecalComponent* NewDecal = UGameplayStatics::SpawnDecalAtLocation(
					GetWorld(),
					NormalDecal,
					FVector(10.0f, 10.0f, 10.0f),
					hit.ImpactPoint,
					hit.ImpactNormal.Rotation(),
					20.0f
					);

					NewDecal->SetFadeScreenSize(0.005f);
					UE_LOG(LogClass, Warning, TEXT("Decal Spawn"));
			}

			////
			//UGameplayStatics::ApplyDamage(
			//	hit.GetActor(),
			//	30.0f,
			//	GetController(),
			//	this,
			//	UBulletDamageType::StaticClass()
			//);

			////
			//UGameplayStatics::ApplyRadialDamage(
			//	GetWorld(),
			//	25.0f, 
			//	hit.ImpactPoint, //위치
			//	500.0f, //범위
			//	UBulletDamageType::StaticClass(),
			//	ignores,
			//	this,
			//	GetController(),
			//	true, // 풀데미지 여부
			//	ECC_Visibility // 막힘 처리
			//);

			UGameplayStatics::ApplyPointDamage(
				hit.GetActor(),
				10.0f,
				-hit.ImpactNormal, //방향
				hit,
				GetController(),
				this,
				UBulletDamageType::StaticClass()
			);

		}
	}
	//UE_LOG(LogClass, Warning, TEXT("Fire"));
	
	if (MuzzleFlash)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, Weapon->GetSocketTransform(TEXT("Muzzle")));
	}

	if (WeaponSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), WeaponSound, Weapon->GetComponentLocation());
	}

}

void ABasicCharacter::StartIronsight()
{
	bIsIronsight = true;
}

void ABasicCharacter::EndIronsight()
{
	bIsIronsight = false;
}

void ABasicCharacter::StartCrouch()
{
	if (CanCrouch())
	{
		Crouch();
	}
	else
	{
		UnCrouch();
	}
}

float ABasicCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{

	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (CurrentHP <= 0.0f) // 이미 죽었으면 처리 안함.
	{
		return 0.0f;
	}

	//UE_LOG(LogClass, Warning, TEXT("Damage : %f"), DamageAmount);

	FVector impulseDirection(0.0f, 0.0f, 0.0f);

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
		impulseDirection = pointEvent->ShotDirection;
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

		//animation ->애니메이션 몽타주 사용.
		if (DeathMontage)
		{
			FString sectionName = FString::Printf(TEXT("Death_%d"), FMath::RandRange(1, 3));

			PlayAnimMontage(DeathMontage, 1.0f, FName(sectionName)); // 애님블루프린트의 defaultslot 을 사용한다.
		}

	}
	else
	{
		if(HitReactMontage)
		{ 
			FString sectionName = FString::Printf(TEXT("Hit_React_%d"), FMath::RandRange(1, 4));

			PlayAnimMontage(HitReactMontage, 1.0f, FName(sectionName));
		}
	}

	return 0.0f;
}

void ABasicCharacter::Reload()
{
	UAnimInstance* animInstance = (GetMesh()) ? GetMesh()->GetAnimInstance() : nullptr;

	if (ReloadMontage && animInstance)
	{
		if (!animInstance->Montage_IsPlaying(ReloadMontage))
		{
			PlayAnimMontage(ReloadMontage);
		}
	}
}

void ABasicCharacter::StartLeftLean()
{
	bLeftLean = true;
}

void ABasicCharacter::EndLeftLean()
{
	bLeftLean = false;
}

void ABasicCharacter::StartRightLean()
{
	bRightLean = true;
}

void ABasicCharacter::EndRightLean()
{
	bRightLean = false;
}

