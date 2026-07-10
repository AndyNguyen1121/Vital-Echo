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

private:
	virtual void SetupInputComponent() override;

	void Move(const FInputActionValue& Value);
	void Primary(const FInputActionValue& Value);
	
private:
	
	UPROPERTY(EditDefaultsOnly, Category = "CS|Input", meta=(AllowPrivateAccess))
	TArray<TObjectPtr<UInputMappingContext>> InputMappingContexts;

	UPROPERTY(EditDefaultsOnly, Category = "CS|Input", meta=(AllowPrivateAccess))
	TObjectPtr<UInputAction> MovementInput;

	UPROPERTY(EditDefaultsOnly, Category = "CS|Input", meta=(AllowPrivateAccess))
	TObjectPtr<UInputAction> PrimaryInput;

	
};


