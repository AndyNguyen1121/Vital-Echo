// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CS_PlayerController.generated.h"

struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class CAPSTONE_API ACS_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="Capstone", meta=(ToolTip="Gives move amount based on player input. Returns 0 idle, 0.5 walking, 1 running"))
	float GetMoveAmount();

	UFUNCTION(BlueprintCallable, Category="Capstone")
	FVector2D GetClampedInputDirection();

	UFUNCTION(BlueprintCallable, Category="Capstone")
	void ResetAllMovementInputs();

	UFUNCTION(BlueprintCallable, Category="Capstone")
	void SetInputEnabled(bool bEnabled);

private:
	virtual void SetupInputComponent() override;

	void Move(const FInputActionValue& Value);
	void Primary(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	
	void ClampInputDirection(FVector2D InputDirection, float InputDeadzone);
	void SetMoveAmount();
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "CS|Input", meta=(AllowPrivateAccess))
	TArray<TObjectPtr<UInputMappingContext>> InputMappingContexts;

	UPROPERTY(EditDefaultsOnly, Category = "CS|Input", meta=(AllowPrivateAccess))
	TObjectPtr<UInputAction> MovementAction;

	UPROPERTY(EditDefaultsOnly, Category = "CS|Input", meta=(AllowPrivateAccess))
	TObjectPtr<UInputAction> PrimaryAction;

	UPROPERTY(EditDefaultsOnly, Category = "CS|Input", meta=(AllowPrivateAccess))
	TObjectPtr<UInputAction> LookAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "CS|Input", meta=(AllowPrivateAccess))
	float LookSensitivity{0.5f};

	UPROPERTY(VisibleAnywhere)
	FVector2D PlayerInputDirection;
	
	UPROPERTY(VisibleAnywhere)
	FVector2D ClampedPlayerInputDirection;
	
	UPROPERTY(EditDefaultsOnly)
	float Deadzone{0.2f};

	UPROPERTY(VisibleAnywhere, meta=(AllowPrivateAccess="true"))
	float MoveAmount;
	
};


