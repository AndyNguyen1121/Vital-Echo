// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CS_CameraActor.h"
#include "Enums/InputTypes.h"
#include "GameFramework/PlayerController.h"
#include "CS_PlayerController.generated.h"

class ACS_CameraActor;
class UCS_PlayerCombatState;
struct FBufferedInput;
class ACS_PlayerState;
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
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
	UFUNCTION(BlueprintPure, Category="Capstone", meta=(ToolTip="Gives move amount based on player input. Returns 0 idle, 0.5 walking, 1 running"))
	float GetMoveAmount() const { return MoveAmount; };

	UFUNCTION(BlueprintCallable, Category="Capstone")
	FVector2D GetClampedInputDirection() const { return ClampedPlayerInputDirection; };;

	UFUNCTION(BlueprintCallable, Category="Capstone")
	void ResetAllMovementInputs();

	UFUNCTION(BlueprintCallable, Category="Capstone")
	void SetInputEnabled(bool bEnabled);

	UFUNCTION(BlueprintCallable, Category="Capstone")
	TSoftObjectPtr<ACS_PlayerState> GetCSPlayerState() const { return CachedPlayerState; };

	UFUNCTION(BlueprintCallable)
	bool CheckInputCriteria(const UCS_PlayerCombatState* CombatStateInfo);

	UFUNCTION(BlueprintPure)
	FVector GetWorldDirectionFromInput();

	UFUNCTION(BlueprintPure)
	USpringArmComponent* GetCameraBoom() const { return CameraActor->CameraBoom; }

public:
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	ACS_CameraActor* CameraActor;

protected:
	void FixedUpdate();

protected:
	float FixedUpdateInterval = 1.0f / 60.0f; 
	float FixedUpdateAccumulator = 0.0f;

private:
	virtual void SetupInputComponent() override;

	void Move(const FInputActionValue& Value);
	void Primary(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void LockOn();
	
	void ClampInputDirection(FVector2D InputDirection, float InputDeadzone);
	void SetMoveAmount();

	void BindAxisInput(UEnhancedInputComponent* EnhancedInputComponent, UInputAction* InputAction, float* Value, EInputType PositiveType, EInputType NegativeType);
	void ConsumeInput(const TArray<EInputType>& InputsToRemove);
	void CleanExpiredInputs();
	bool CheckInputsInBuffer(const TArray<EInputType>& InputsToSearch, const int FramesToCheck);
	EInputType GetDirectionalInputInBuffer(EInputType InputType) const;
	
	void TickInputBuffer();
	
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACS_CameraActor> CameraClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "CS|Input", meta=(AllowPrivateAccess))
	TArray<TObjectPtr<UInputMappingContext>> InputMappingContexts;

	UPROPERTY(EditDefaultsOnly, Category = "CS|Input", meta=(AllowPrivateAccess))
	TObjectPtr<UInputAction> UpAction;

	UPROPERTY(EditDefaultsOnly, Category = "CS|Input", meta=(AllowPrivateAccess))
	TObjectPtr<UInputAction> DownAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "CS|Input", meta=(AllowPrivateAccess))
	TObjectPtr<UInputAction> LeftAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "CS|Input", meta=(AllowPrivateAccess))
	TObjectPtr<UInputAction> RightAction;

	UPROPERTY(EditDefaultsOnly, Category = "CS|Input", meta=(AllowPrivateAccess))
	TObjectPtr<UInputAction> MovementAction;

	UPROPERTY(EditDefaultsOnly, Category = "CS|Input", meta=(AllowPrivateAccess))
	TObjectPtr<UInputAction> PrimaryAction;

	UPROPERTY(EditDefaultsOnly, Category = "CS|Input", meta=(AllowPrivateAccess))
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, Category = "CS|Input", meta=(AllowPrivateAccess))
	TObjectPtr<UInputAction> LockOnAction;
	
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

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TSoftObjectPtr<ACS_PlayerState> CachedPlayerState;

	UPROPERTY(VisibleAnywhere)
	TArray<FBufferedInput> InputBuffer;

	UPROPERTY(VisibleAnywhere)
	float LeftInputValue;
	UPROPERTY(VisibleAnywhere)
	float RightInputValue;
	UPROPERTY(VisibleAnywhere)
	float UpInputValue;
	UPROPERTY(VisibleAnywhere)
	float DownInputValue;
	
};


