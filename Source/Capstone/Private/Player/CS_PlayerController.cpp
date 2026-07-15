// Fill out your copyright notice in the Description page of Project Settings.


#include "Capstone/Public/Player/CS_PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CS_CameraActor.h"
#include "Capstone/DebugMacros.h"
#include "Characters/CS_PlayerCharacter.h"
#include "DataAssets/CS_PlayerCombatState.h"
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
	EnhancedInputComponent->BindAction(LockOnAction, ETriggerEvent::Started, this, &ACS_PlayerController::LockOn);
	
}

void ACS_PlayerController::Move(const FInputActionValue& Value)
{
	FVector2D MovementValue = Value.Get<FVector2D>();
	PlayerInputDirection = MovementValue;
	
	if (!CachedPlayerState->bCanMove)
	{
		ClampedPlayerInputDirection = FVector2D::ZeroVector;
		MoveAmount = 0;
		return;
	}
	
	ACharacter* CachedCharacter = GetCharacter();
	if (!IsValid(CachedCharacter)) return;
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
	if (!GetCSPlayerState() || GetCSPlayerState()->bIsLockedOn) return;
	
	FVector2D LookValue = Value.Get<FVector2D>();
	
	AddYawInput(LookValue.X * LookSensitivity);
	AddPitchInput(-LookValue.Y * LookSensitivity);
}

void ACS_PlayerController::LockOn()
{
	if (ACS_PlayerCharacter* PlayerCharacter = Cast<ACS_PlayerCharacter>(GetCharacter()))
	{
		PlayerCharacter->ToggleLockOn();
	}
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

void ACS_PlayerController::ConsumeInput(const TArray<EInputType>& InputsToRemove)
{
	int NumInputsToCheck = InputsToRemove.Num();
	
	if (InputBuffer.Num() < NumInputsToCheck || NumInputsToCheck < 0)
		return;
	
	for (int32 i = InputBuffer.Num() - 1; i >= 0; --i)
	{
		EInputType InputType = InputBuffer[i].Input;

		if (InputType == EInputType::EIT_Left ||
			InputType == EInputType::EIT_Right ||
			InputType == EInputType::EIT_Up ||
			InputType == EInputType::EIT_Down ||
			InputType == EInputType::EIT_Neutral)
		{
			continue; // keep movement and neutral
		}

		if (InputsToRemove.Contains(InputType))
		{
			InputBuffer[i].bIsConsumed = true;
		}
	}
}

void ACS_PlayerController::CleanExpiredInputs()
{
	InputBuffer.RemoveAll([] (FBufferedInput& Input)
	{
		return Input.FramesElapsedSincePressed > 60 && !Input.bIsBeingHeld;
	});
}

bool ACS_PlayerController::CheckInputsInBuffer(const TArray<EInputType>& InputsToSearch, const int FramesToCheck)
{
	int NumInputsToCheck = InputsToSearch.Num();

	if (NumInputsToCheck == 0)
		return true;

	// Make a copy of InputsToSearch to remove EIT_Neutral from list as needed
	TArray<EInputType> CopyOfInputsToSearch(InputsToSearch);

	// SPECIAL CASE: Account for neutral input
	if (InputsToSearch.Contains(EInputType::EIT_Neutral))
	{
		if (!ClampedPlayerInputDirection.IsNearlyZero())
		{
			return false;
		}
		
		NumInputsToCheck -= 1;
		CopyOfInputsToSearch.Remove(EInputType::EIT_Neutral);
	}

	if (InputBuffer.Num() < NumInputsToCheck || InputBuffer.Num() == 0)
		return false;

	bool bRequiresDirectionalInput = 
	CopyOfInputsToSearch.Contains(EInputType::EIT_Left)  ||
	CopyOfInputsToSearch.Contains(EInputType::EIT_Back)  ||
	CopyOfInputsToSearch.Contains(EInputType::EIT_Up)    ||
	CopyOfInputsToSearch.Contains(EInputType::EIT_Down);

	if (bRequiresDirectionalInput && ClampedPlayerInputDirection.IsNearlyZero())
		return false;
	
	for (int i = 0; i < InputBuffer.Num(); ++i)
	{

		if (!InputBuffer[i].bIsConsumed && (InputBuffer[i].FramesElapsedSincePressed <= FramesToCheck || InputBuffer[i].bIsBeingHeld))
		{
			if (CopyOfInputsToSearch.Contains(InputBuffer[i].Input))
			{
				CopyOfInputsToSearch.Remove(InputBuffer[i].Input);
			}
			else if ((InputBuffer[i].Input == EInputType::EIT_Up || InputBuffer[i].Input == EInputType::EIT_Down
				|| InputBuffer[i].Input == EInputType::EIT_Left || InputBuffer[i].Input == EInputType::EIT_Right)
				&& !ClampedPlayerInputDirection.IsNearlyZero())
			{
				EInputType DirectionalInput = GetDirectionalInputInBuffer(InputBuffer[i].Input);
	
				if (CopyOfInputsToSearch.Contains(DirectionalInput))
				{
					CopyOfInputsToSearch.Remove(DirectionalInput);
				}
			}
		}
		
		// SUCCESS: consume inputs if check is successful
		if (CopyOfInputsToSearch.Num() == 0)
		{
			ConsumeInput(InputsToSearch);
			return true;
		}
	}

	return false;
}

EInputType ACS_PlayerController::GetDirectionalInputInBuffer(EInputType InputType) const
{
	if (InputType != EInputType::EIT_Up && InputType != EInputType::EIT_Down && InputType != EInputType::EIT_Left && InputType != EInputType::EIT_Right)
	{
		return EInputType::EIT_Neutral;
	}

	FVector DotA = GetCharacter()->GetActorForwardVector();

	FVector DotB;

	if (InputType == EInputType::EIT_Up)
	{
		DotB = PlayerCameraManager->GetActorForwardVector();
	}
	else if (InputType == EInputType::EIT_Right)
	{
		DotB = PlayerCameraManager->GetActorRightVector();
	}
	else if (InputType == EInputType::EIT_Down)
	{
		DotB = -PlayerCameraManager->GetActorForwardVector();
	}
	else if (InputType == EInputType::EIT_Left)
	{
		DotB = -PlayerCameraManager->GetActorRightVector();
	}

	float DotProduct = FVector::DotProduct(DotA, DotB);
	
	if (DotProduct >= 0.f)
	{
		return EInputType::EIT_Forward;
	}
	else
	{
		return EInputType::EIT_Back;
	}
}

bool ACS_PlayerController::CheckInputCriteria(const UCS_PlayerCombatState* CombatStateInfo)
{
	if (!CombatStateInfo)
		return false;
	
	CleanExpiredInputs();

	return CheckInputsInBuffer(CombatStateInfo->RequiredInputs, CombatStateInfo->InputBufferWindow);
}

FVector ACS_PlayerController::GetWorldDirectionFromInput()
{
	if (PlayerInputDirection.IsNearlyZero()) return FVector::ZeroVector;
	
	FVector TargetDirection =
		PlayerCameraManager->GetActorForwardVector() * PlayerInputDirection.Y +
			PlayerCameraManager->GetActorRightVector() * PlayerInputDirection.X;

	TargetDirection.Z = 0;
	TargetDirection.Normalize();

	return TargetDirection;
}

void ACS_PlayerController::TickInputBuffer()
{
	for (FBufferedInput& BufferedInput : InputBuffer)
	{
		BufferedInput.FramesElapsedSincePressed++;

		if (BufferedInput.bIsBeingHeld)
			BufferedInput.FramesHeld++;
	}
}

void ACS_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	CachedPlayerState = Cast<ACS_PlayerState>(PlayerState);

	FActorSpawnParameters SpawnParams;

	CameraActor = Cast<ACS_CameraActor>(GetWorld()->SpawnActor(CameraClass));
	SetViewTarget(CameraActor);
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

