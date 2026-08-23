// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Structs/DamageAttributes.h"
#include "CS_CombatStateBase.generated.h"

class UGameplayAbility;
/**
 * 
 */
UCLASS()
class CAPSTONE_API UCS_CombatStateBase : public UDataAsset
{
	GENERATED_BODY()

	

public:
	UCS_CombatStateBase();
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> GameplayAbility;
	
	UPROPERTY(EditAnywhere)
	TArray<UCS_CombatStateBase*> TransitionStates;

	UPROPERTY(EditAnywhere)
	FDamageAttributes DamageAttributes;

	UPROPERTY(EditAnywhere)
	FDamageVisuals DamageVisuals;
};
