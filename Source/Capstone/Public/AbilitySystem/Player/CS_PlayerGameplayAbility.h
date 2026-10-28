// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CS_PlayerGameplayAbility.generated.h"

class ACS_PlayerCharacter;
/**
 * 
 */
UCLASS()
class CAPSTONE_API UCS_PlayerGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category="Ability")
	TObjectPtr<ACS_PlayerCharacter> PlayerCharacter;
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void RotateToControllerDirection(float RotateDuration);
};
