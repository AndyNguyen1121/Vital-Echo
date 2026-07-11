// Fill out your copyright notice in the Description page of Project Settings.


#include "Capstone/Public/Player/CS_PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "Player/CS_PlayerState.h"
#include "Structs/BufferedInput.h"


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

	BindAxisInput(EnhancedInputComponent,LeftAction, &LeftInputValue, EInputType::EIT_Left, EInputType::EIT_Right);
	BindAxisInput(EnhancedInputComponent,RightAction, &RightInputValue, EInputType::EIT_Right, EInputType::EIT_Left);
	BindAxisInput(EnhancedInputComponent,UpAction, &UpInputValue, EInputType::EIT_Up, EInputType::EIT_Down);
	BindAxisInput(EnhancedInputComponent,DownAction, &DownInputValue, EInputType::EIT_Down, EInputType::EIT_Up);
	EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Completed, this, &ACS_PlayerController::ResetAllMovementInputs);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
	EnhancedInputComponent->BindAction(PrimaryAction, ETriggerEvent::Started, this, &ThisClass::Primary);
}

void ACS_PlayerController::Move(const FInputActionValue& Value)
{
	if (!CachedPlayerState->bCanMove)
	{
		ResetAllMovementInputs();
		return;
	}
	
	ACharacter* CachedCharacter = GetCharacter();
	if (!IsValid(CachedCharacter)) return;

	FVector2D MovementValue = Value.Get<FVector2D>();
	PlayerInputDirection = MovementValue;
	ClampInputDirection(MovementValue, Deadzone);
	SetMoveAmount();
	
	FRotator YawRotation(0, GetControlRotation().Yaw, 0);
	
	FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	FVector MovementDirection = (ForwardDirection * MovementValue.Y) + (RightDirection * MovementValue.X);
	GetCharacter()->AddMovementInput(MovementDirection , MoveAmount);
}

void ACS_PlayerController::Primary(const FInputActionValue& Value)
{
	InputBuffer.Add(FBufferedInput(EInputType::EIT_Primary));
}

void ACS_PlayerController::Look(const FInputActionValue& Value)
{
	FVector2D LookValue = Value.Get<FVector2D>();
	AddYawInput(LookValue.X * LookSensitivity);
	AddPitchInput(-LookValue.Y * LookSensitivity);
}

void ACS_PlayerController::ResetAllMovementInputs()
{
	ClampedPlayerInputDirection = FVector2D::ZeroVector;
	PlayerInputDirection = FVector2D::ZeroVector;
	MoveAmount = 0;
}

void ACS_PlayerController::SetInputEnabled(bool bEnabled)
{
	if (bEnabled)
	{
		EnableInput(this);
	}
	else
	{
		DisableInput(this);
		ResetAllMovementInputs();
	}
}

ACS_PlayerState* ACS_PlayerController::GetCSPlayerState()
{
	return CachedPlayerState;
}

void ACS_PlayerController::ClampInputDirection(FVector2D InputDirection, float InputDeadzone)
{
	auto QuantizeAxis = [InputDeadzone](float Value)
	{
		if (FMath::Abs(Value) <= InputDeadzone)
		{
			return 0.0f;
		}

		return FMath::RoundToFloat(Value * 2.0f) / 2.0f;
	};

	ClampedPlayerInputDirection.X = QuantizeAxis(InputDirection.X);
	ClampedPlayerInputDirection.Y = QuantizeAxis(InputDirection.Y);
}

void ACS_PlayerController::SetMoveAmount()
{
	MoveAmount = 0;

	float MagnitudeSquared = ClampedPlayerInputDirection.SizeSquared();
	if (MagnitudeSquared >= 0.25 && MagnitudeSquared < 1)
	{
		MoveAmount = 0.5f;
	}
	else if (MagnitudeSquared > 0.25)
	{
		MoveAmount = 1;
	}
	
}

void ACS_PlayerController::BindAxisInput(UEnhancedInputComponent* EnhancedInputComponent,
	UInputAction* InputAction, float* Value, EInputType PositiveType, EInputType NegativeType)
{
	if (!IsValid(EnhancedInputComponent) || !IsValid(InputAction) || !Value)
		return;

	EnhancedInputComponent->BindActionValueLambda(InputAction, ETriggerEvent::Started,
		[this, Value, PositiveType, NegativeType](const FInputActionValue& InputValue)
	{
		*Value = InputValue.Get<float>();

		FBufferedInput* PreviousHeld = InputBuffer.FindByPredicate([PositiveType, NegativeType](const FBufferedInput& Input)
		{
			if (Input.Input != PositiveType && Input.Input != NegativeType)
				return false;

			return Input.bIsBeingHeld;
		});

		if (PreviousHeld)
			PreviousHeld->bIsBeingHeld = false;

		FBufferedInput NewInput;
		NewInput.Input = PositiveType;
		NewInput.bIsBeingHeld = true;
		InputBuffer.Add(NewInput);
	});

	EnhancedInputComponent->BindActionValueLambda(InputAction, ETriggerEvent::Completed,
		[this, Value, PositiveType, NegativeType](const FInputActionValue& InputValue)
	{
		*Value = 0;

		FBufferedInput* PreviousHeld = InputBuffer.FindByPredicate([PositiveType, NegativeType](const FBufferedInput& Input)
		{
			if (Input.Input != PositiveType && Input.Input != NegativeType)
				return false;

			return Input.bIsBeingHeld;
		});

		if (PreviousHeld)
			PreviousHeld->bIsBeingHeld = false;
	});
}

void ACS_PlayerController::TickInputBuffer()
{
	for (FBufferedInput& BufferedInput : InputBuffer)
	{
		BufferedInput.FramesElapsedSincePressed++;

		if (BufferedInput.bIsBeingHeld)
			BufferedInput.FramesHeld++;
	}

	InputBuffer.RemoveAll([] (FBufferedInput& Input)
	{
		return Input.FramesElapsedSincePressed > 150 && !Input.bIsBeingHeld;
	});
}

void ACS_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	CachedPlayerState = Cast<ACS_PlayerState>(PlayerState);
}

void ACS_PlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	while (FixedUpdateAccumulator >= FixedUpdateInterval)
	{
		FixedUpdate();
		FixedUpdateAccumulator -= FixedUpdateInterval;
	}
	FixedUpdateAccumulator += DeltaSeconds;
}

void ACS_PlayerController::FixedUpdate()
{
	TickInputBuffer();
}

float ACS_PlayerController::GetMoveAmount()
{
	return MoveAmount;
}

FVector2D ACS_PlayerController::GetClampedInputDirection()
{
	return ClampedPlayerInputDirection;
}
