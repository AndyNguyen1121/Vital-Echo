// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachine/CS_PlayerStateMachineManager.h"

#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
#include "Abilities/GameplayAbility.h"
#include "Characters/CS_PlayerCharacter.h"
#include "DataAssets/Base/CS_CombatStateBase.h"
#include "DataAssets/CS_PlayerCombatState.h"


// Sets default values for this component's properties
UCS_PlayerStateMachineManager::UCS_PlayerStateMachineManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

bool UCS_PlayerStateMachineManager::SwitchState(UCS_PlayerCombatState* NextState)
{
	if (!NextState || !IsValid(PlayerCharacter)) return false;
	if (!ValidPlayerStates.Contains(NextState)) return false;

	CurrentState = NextState;
	PlayerCharacter->GetAbilitySystemComponent()->TryActivateAbilityByClass(NextState->GameplayAbility);

	return true;
}


// Called when the game starts
void UCS_PlayerStateMachineManager::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerCharacter = Cast<ACS_PlayerCharacter>(GetOwner());
	if (!IsValid(PlayerCharacter)) return;
	
	ConvertStatesToPlayerCombatState(CombatStateList);
}

void UCS_PlayerStateMachineManager::ConvertStatesToPlayerCombatState(const TSet<UCS_CombatStateBase*>& StateBases)
{
	for (UCS_CombatStateBase* State : StateBases)
	{
		if (UCS_PlayerCombatState* PlayerState = Cast<UCS_PlayerCombatState>(State))
		{
			ValidPlayerStates.Add(PlayerState);
		}
	}
}

