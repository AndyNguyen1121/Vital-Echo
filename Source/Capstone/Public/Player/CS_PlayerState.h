// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "CS_PlayerState.generated.h"

UCLASS()
class CAPSTONE_API ACS_PlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACS_PlayerState();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanMove{true};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsPerformingAction{false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanRotate{true};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAttackWindowOpen{false};

	UFUNCTION(BlueprintCallable)
	void ResetBooleans(bool CanMove, bool IsPerformingAction, bool CanRotate);

private:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
};
