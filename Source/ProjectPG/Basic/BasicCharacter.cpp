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

		FHitResult hit;
		
		bool result = UKismetSystemLibrary::LineTraceSingleForObjects(
			GetWorld(),
			traceStart,
			traceEnd,
			objects,
			true,
			ignores,
			EDrawDebugTrace::ForDuration,
			hit,
			true,
			FLinearColor::Red,
			FLinearColor::Blue,
			5.0f
		);

		if (result)
		{
			UE_LOG(LogClass, Warning, TEXT("Hit : %s"), *hit.GetActor()->GetName());

			UGameplayStatics::ApplyDamage(
				hit.GetActor(),
				30.0f,
				GetController(),
				this,
				UBulletDamageType::StaticClass()
			);

			if (HitEffect)
			{
				
			}
			if (BloodEffect)
			{

			}


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

	UE_LOG(LogClass, Warning, TEXT("Damage : %f"), DamageAmount);

	return 0.0f;
}

