// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/CS_AbilitySystemComponent.h"
#include "GameFramework/PlayerState.h"
#include "CS_PlayerState.generated.h"

class UCS_AttributeSet;
struct FCharacterStateInfo;

UCLASS()
class CAPSTONE_API ACS_PlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACS_PlayerState();
	virtual UCS_AbilitySystemComponent* GetAbilitySystemComponent() const override;

public:
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanMove{true};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsPerformingAction{false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanRotate{true};*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsHardLockedOn{false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsBlocking{false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsSprinting{false};

	/*UFUNCTION(BlueprintCallable)
	void ResetBooleans(bool CanMove, bool IsPerformingAction, bool CanRotate, bool ResetToDefaultState);

	UFUNCTION(BlueprintCallable)
	void ResetBooleansWithStateInfo(FCharacterStateInfo StateInfo, bool ResetToDefaultState);*/

	UFUNCTION(BlueprintPure)
	UCS_AttributeSet* GetAttributeSet() { return AttributeSet; }

private:
	UPROPERTY()
	TObjectPtr<UCS_AbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UCS_AttributeSet> AttributeSet;
};
