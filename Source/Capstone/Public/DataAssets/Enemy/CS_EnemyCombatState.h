// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/Base/CS_CombatStateBase.h"
#include "Structs/CharacterStateInfo.h"
#include "CS_EnemyCombatState.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONE_API UCS_EnemyCombatState : public UCS_CombatStateBase
{
	GENERATED_BODY()

public:
	UCS_EnemyCombatState();
	
	UPROPERTY(EditAnywhere, Category="SetFlags")
	FCharacterStateInfo StateInfo;
};
