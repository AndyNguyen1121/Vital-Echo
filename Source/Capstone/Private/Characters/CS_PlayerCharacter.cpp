// Fill out your copyright notice in the Description page of Project Settings.


#include "Capstone/Public/Characters/CS_PlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Camera/CS_CameraActor.h"
#include "Camera/CS_CameraContainer.h"
#include "Camera/CS_PlayerCameraManager.h"
#include "Capstone/DebugMacros.h"
#include "Capstone/Public/Player/CS_PlayerState.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameplayTags/CSTags.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/CS_PlayerController.h"
#include "StateMachine/CS_PlayerStateMachineManager.h"


// Sets default values
ACS_PlayerCharacter::ACS_PlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PlayerStateMachineManager = CreateDefaultSubobject<UCS_PlayerStateMachineManager>("StateMachine");

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 540, 0);
	GetCharacterMovement()->JumpZVelocity = 500;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.f;
	

	GameplayTags.AddTag(CSTags::IDs::Player);

}

void ACS_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	CachedPlayerState = Cast<ACS_PlayerState>(GetPlayerState());
	CachedPlayerController = Cast<ACS_PlayerController>(GetController());
}

void ACS_PlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (LockOnTarget.IsValid() && GetCSPlayerState()->bCanRotate)
	{
		FVector DirToLockOnEnemy = CachedPlayerController->GetCameraContainer()->GetLockOnActor()->GetActorLocation() - GetActorLocation();
		DirToLockOnEnemy.Z = 0;
		DirToLockOnEnemy.Normalize();

		FRotator DesiredRotation = UKismetMathLibrary::RInterpTo_Constant(GetActorRotation(), DirToLockOnEnemy.Rotation(), DeltaSeconds, 500.f);

		SetActorRotation(DesiredRotation);
	}
}

UAbilitySystemComponent* ACS_PlayerCharacter::GetAbilitySystemComponent() const
{
	
	if (ACS_PlayerState* CurrentPlayerState = Cast<ACS_PlayerState>(GetPlayerState()))
	{
		return CurrentPlayerState->GetAbilitySystemComponent();
	}

	return nullptr;
}

void ACS_PlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!GetAbilitySystemComponent()) return;

	GetAbilitySystemComponent()->InitAbilityActorInfo(GetPlayerState(), this);
}

#pragma region Lock On
ACS_PlayerCameraManager* ACS_PlayerCharacter::GetCSCameraManager() const
{
	return Cast<ACS_PlayerCameraManager>(UGameplayStatics::GetPlayerCameraManager(this, 0));
}

void ACS_PlayerCharacter::ToggleLockOn()
{
	ACS_PlayerCameraManager* CachedCamera = GetCSCameraManager();
	if (!IsValid(CachedCamera)) return;
	
	if (!GetCSPlayerState()->bIsLockedOn)
	{
		LockOnTarget = CachedCamera->EnableLockOn(LockOnSearchRadius, LockOnMaxAngle);
	}
	else
	{
		LockOnTarget = nullptr;
		CachedCamera->DisableLockOn();
	}
}
#pragma endregion 




