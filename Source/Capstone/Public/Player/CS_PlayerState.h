// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/CS_AbilitySystemComponent.h"
#include "GameFramework/PlayerState.h"
#include "CS_PlayerState.generated.h"

struct FPlayerStateInfo;

UCLASS()
class CAPSTONE_API ACS_PlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACS_PlayerState();
	virtual UCS_AbilitySystemComponent* GetAbilitySystemComponent() const override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanMove{true};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsPerformingAction{false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanRotate{true};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAttackWindowOpen{false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsLockedOn{false};

	UFUNCTION(BlueprintCallable)
	void ResetBooleans(bool CanMove, bool IsPerformingAction, bool CanRotate, bool ResetToDefaultState);

	UFUNCTION(BlueprintCallable)
	void ResetBooleansWithStateInfo(FPlayerStateInfo StateInfo, bool ResetToDefaultState);

private:
	UPROPERTY()
	TObjectPtr<UCS_AbilitySystemComponent> AbilitySystemComponent;
};
