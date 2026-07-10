// Fill out your copyright notice in the Description page of Project Settings.


#include "Capstone/Public/Characters/CS_PlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Capstone/Public/Player/CS_PlayerState.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
ACS_PlayerCharacter::ACS_PlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bUsePawnControlRotation = true;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>("Camera");
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

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



