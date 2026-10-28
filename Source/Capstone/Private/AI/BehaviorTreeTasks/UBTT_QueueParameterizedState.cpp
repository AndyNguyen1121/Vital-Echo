// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BehaviorTreeTasks/UBTT_QueueParameterizedState.h"

#include "AIController.h"
#include "ActorComponents/StateMachine/CS_EnemyStateMachineManager.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Capstone/DebugMacros.h"
#include "Characters/CS_BaseCharacter.h"
#include "Characters/Enemy/CS_EnemyCharacter.h"
#include "DataAssets/Enemy/CS_EnemyCombatProfile.h"
#include "DataAssets/Enemy/CS_EnemyCombatState.h"

EBTNodeResult::Type UBTT_QueueParameterizedState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACS_EnemyCharacter* EnemyCharacter = Cast<ACS_EnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	
	AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(TargetKey.SelectedKeyName));
	
	if (!IsValid(EnemyCharacter) || !Blackboard || !IsValid(TargetActor) || QueuedStateKey.IsNone()) 
	{
		DEBUG_LOG("HUH")
		return EBTNodeResult::Failed;
	}
	
	UCS_EnemyStateMachineManager* CachedEnemyStateMachine = EnemyCharacter->GetEnemyStateMachineManager();
	UCS_EnemyCombatProfile* CombatProfile = Cast<UCS_EnemyCombatProfile>(CachedEnemyStateMachine->CombatProfile);

	if (!CachedEnemyStateMachine || !CachedEnemyStateMachine->CombatProfile )
	{
		return EBTNodeResult::Failed;
	}

	float Distance = FVector::Distance(TargetActor->GetActorLocation(), EnemyCharacter->GetActorLocation());

	TSet<TPair<UCS_EnemyCombatState*, float>> ValidStates;
	float TotalWeight = 0.f;
	
	for (UCS_CombatStateBase* CombatState : CombatProfile->TransitionStates)
	{
		if (UCS_EnemyCombatState* EnemyCombatState = Cast<UCS_EnemyCombatState>(CombatState))
		{
			float Weight = CalculateWeight(EnemyCombatState->MinMaxDistance, Distance);
			TotalWeight += Weight;

			UE_LOG(LogTemp, Warning, TEXT("%s | Range: %.1f - %.1f | Distance: %.1f | Weight: %.4f"),
				*EnemyCombatState->GetName(),
				EnemyCombatState->MinMaxDistance.X,
				EnemyCombatState->MinMaxDistance.Y,
				Distance,
				Weight
			);
			
			TPair<UCS_EnemyCombatState*, float> NewPair(EnemyCombatState, Weight);
			ValidStates.Add(NewPair);
		}
	}

	float RandomValue = FMath::RandRange(0.f, TotalWeight);
	float RunningWeight = 0.f;
	UCS_EnemyCombatState* ChosenEnemyState = nullptr;
	
	for (auto StatePair  : ValidStates)
	{
		RunningWeight += StatePair.Value;
    
		if (RandomValue <= RunningWeight)
		{
			ChosenEnemyState = StatePair.Key;
			break;
		}
	}
	
	Blackboard->SetValueAsObject(QueuedStateKey.SelectedKeyName, ChosenEnemyState);
	Blackboard->SetValueAsFloat(WalkRadiusKey.SelectedKeyName, ChosenEnemyState ? ChosenEnemyState->MinMaxDistance.Y : 0); 
	
	UE_LOG(LogTemp, Warning, TEXT("Chosen: %s | Value: %.5f "),
			ChosenEnemyState ? *ChosenEnemyState->GetName() : *FString("None"),
			RandomValue);
	UE_LOG(LogTemp, Warning, TEXT("--------------------------------------------------------"));
	
	return Super::ExecuteTask(OwnerComp, NodeMemory);
	
}

float UBTT_QueueParameterizedState::CalculateWeight(FVector2D MinMaxDistance, float DistanceToTarget)
{
	const float MinDistance = MinMaxDistance.X;
	const float MaxDistance = MinMaxDistance.Y;

	if (DistanceToTarget < MinDistance)
	{
		return 1.f / (1.f + (MinDistance - DistanceToTarget));
	}

	if (DistanceToTarget > MaxDistance)
	{
		return 1.f / (1.f + (DistanceToTarget - MaxDistance));
	}
	return 1.f;
}
