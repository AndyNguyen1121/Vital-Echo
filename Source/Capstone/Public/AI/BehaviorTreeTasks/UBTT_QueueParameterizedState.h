// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "UBTT_QueueParameterizedState.generated.h"

class UCS_EnemyCombatState;
class UCS_CombatStateBase;
/**
 * 
 */
UCLASS()
class CAPSTONE_API UBTT_QueueParameterizedState : public UBTTaskNode
{
	GENERATED_BODY()

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector QueuedStateKey;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector WalkRadiusKey;

private:
	float CalculateWeight(FVector2D MinMaxDistance, float DistanceToTarget);
};
