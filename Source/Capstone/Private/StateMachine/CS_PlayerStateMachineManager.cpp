// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachine/CS_PlayerStateMachineManager.h"

#include "AbilitySystemComponent.h"
#include "Characters/CS_PlayerCharacter.h"
#include "DataAssets/Base/CS_CombatStateBase.h"
#include "DataAssets/CS_PlayerCombatState.h"
#include "Player/CS_PlayerState.h"


// Sets default values for this component's properties
UCS_PlayerStateMachineManager::UCS_PlayerStateMachineManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UCS_PlayerStateMachineManager::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerCharacter = Cast<ACS_PlayerCharacter>(GetOwner());
	if (!IsValid(PlayerCharacter)) return;
	
	ConvertStatesToPlayerCombatState(CombatStateList);
}

void UCS_PlayerStateMachineManager::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckForStateTransitions();
}

bool UCS_PlayerStateMachineManager::SwitchState(UCS_PlayerCombatState* NextState)
{
	if (!NextState || !IsValid(PlayerCharacter)) return false;
	if (!ValidPlayerStates.Contains(NextState)) return false;

	CurrentState = NextState;
	CurrentTransitionStateSet = NextState->TransitionStates;
	PlayerCharacter->GetAbilitySystemComponent()->TryActivateAbilityByClass(NextState->GameplayAbility);

	return true;
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

void UCS_PlayerStateMachineManager::CheckForStateTransitions()
{
	for (const auto& State : CurrentTransitionStateSet)
	{
		if (CheckIfCanTransitionState(State))
		{
			if (UCS_PlayerCombatState* PlayerCombatState = Cast<UCS_PlayerCombatState>(State))
			{
				SwitchState(PlayerCombatState);
				return;
			}
		}
	}
}

bool UCS_PlayerStateMachineManager::CheckIfCanTransitionState(UCS_CombatStateBase* State)
{
	if (!IsValid(PlayerCharacter) || !IsValid(PlayerCharacter->GetCSPlayerState()))
	if (PlayerCharacter->GetCSPlayerState()->bIsPerformingAction && !PlayerCharacter->GetCSPlayerState()->bAttackWindowOpen)
		return false;
	
	return false;
}

