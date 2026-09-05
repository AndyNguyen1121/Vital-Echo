// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/StateMachine/CS_EnemyStateMachineManager.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Characters/CS_EnemyCharacter.h"
#include "DataAssets/Enemy/CS_EnemyCombatProfile.h"
#include "DataAssets/Enemy/CS_EnemyCombatState.h"

// Called when the game starts
void UCS_EnemyStateMachineManager::BeginPlay()
{
	EnemyCharacter = Cast<ACS_EnemyCharacter>(GetOwner());
	if (!IsValid(EnemyCharacter)) return;
	Super::BeginPlay();
}

bool UCS_EnemyStateMachineManager::SwitchState(UCS_CombatStateBase* NextState)
{
	if (!NextState || !IsValid(EnemyCharacter)) return false;
	if (!CombatStateList.Contains(NextState)) return false;

	if (NextState->GameplayAbility)
	{
		bool bCanActivate = EnemyCharacter->GetAbilitySystemComponent()->TryActivateAbilityByClass(NextState->GameplayAbility);
		if (!bCanActivate)
		{
			return false;
		}
	}

	if (UCS_EnemyCombatState* CombatState = Cast<UCS_EnemyCombatState>(NextState))
	{
		EnemyCharacter->CurrentDamageAttribute = CombatState->DamageAttributes;
		EnemyCharacter->CurrentDamageVisuals = CombatState->DamageFeedback;
		EnemyCharacter->SetGameplayState(CombatState->StateInfo.bCanMove, CombatState->StateInfo.bIsPerformingAction, CombatState->StateInfo.bCanRotate, false);
	}

	return true;
}


