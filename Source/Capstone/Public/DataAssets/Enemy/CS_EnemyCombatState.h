// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/Base/CS_CombatStateBase.h"
#include "Structs/DamageAttributes.h"
#include "CS_EnemyCombatState.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONE_API UCS_EnemyCombatState : public UCS_CombatStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FDamageAttributes DamageAttributes;

	UPROPERTY(EditAnywhere)
	FDamageVisuals DamageVisuals;
};
