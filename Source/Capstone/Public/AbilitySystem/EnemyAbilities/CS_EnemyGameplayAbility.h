// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CS_EnemyGameplayAbility.generated.h"

class ACS_EnemyCharacter;
/**
 * 
 */
UCLASS()
class CAPSTONE_API UCS_EnemyGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category="Ability")
	TObjectPtr<ACS_EnemyCharacter> EnemyCharacter;
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
