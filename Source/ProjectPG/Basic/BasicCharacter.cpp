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

// Sets default values
ABasicCharacter::ABasicCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->TargetArmLength = 200.0f;
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
}

void ABasicCharacter::EndFire()
{
	bIsFire = false;
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

