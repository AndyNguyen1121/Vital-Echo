// Fill out your copyright notice in the Description page of Project Settings.


#include "Capstone/Public/Player/CS_PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"

void ACS_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (IsValid(InputSubsystem))
	{
		for (UInputMappingContext* Context : InputMappingContexts)
		{
			InputSubsystem->AddMappingContext(Context, 0);
		}
	}

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	
	if (!IsValid(EnhancedInputComponent))
		return;

	EnhancedInputComponent->BindAction(MovementInput, ETriggerEvent::Triggered, this, &ThisClass::Move);
}

void ACS_PlayerController::Move(const FInputActionValue& Value)
{
	ACharacter* CachedCharacter = GetCharacter();
	if (!IsValid(CachedCharacter)) return;

	FVector2D MovementValue = Value.Get<FVector2D>();
	FRotator YawRotation(0, GetControlRotation().Yaw, 0);
	
	FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	FVector MovementDirection = (ForwardDirection * MovementValue.Y) + (RightDirection * MovementValue.X);
	GetCharacter()->AddMovementInput(MovementDirection , 1);
}

void ACS_PlayerController::Primary(const FInputActionValue& Value)
{
}
