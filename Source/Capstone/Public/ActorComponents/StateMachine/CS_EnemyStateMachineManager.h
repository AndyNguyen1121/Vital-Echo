// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CS_StateMachineManagerBase.h"
#include "CS_EnemyStateMachineManager.generated.h"


class ACS_EnemyCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CAPSTONE_API UCS_EnemyStateMachineManager : public UCS_StateMachineManagerBase
{
	GENERATED_BODY()

public:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual bool SwitchState(UCS_CombatStateBase* NextState) override;

public:
	UPROPERTY()
	TObjectPtr<ACS_EnemyCharacter> EnemyCharacter;
};
