// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachine/CS_PlayerStateMachineManager.h"

#include "AbilitySystemComponent.h"
#include "Capstone/DebugMacros.h"
#include "Characters/CS_PlayerCharacter.h"
#include "DataAssets/Base/CS_CombatStateBase.h"
#include "DataAssets/CS_PlayerCombatState.h"
#include "DataAssets/Base/CS_CombatProfile.h"
#include "Player/CS_PlayerController.h"
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
	CurrentStateTransitionSet = NextState->TransitionStates;
	PlayerCharacter->GetAbilitySystemComponent()->TryActivateAbilityByClass(NextState->GameplayAbility);

	return true;
}

bool UCS_PlayerStateMachineManager::SwitchState(UCS_CombatStateBase* NextState)
{
	if (UCS_PlayerCombatState* PlayerCombatState = Cast<UCS_PlayerCombatState>(NextState))
	{
		SwitchState(PlayerCombatState);
		return true;
	}
	return false;
}

void UCS_PlayerStateMachineManager::ResetToDefaultState()
{
	SwitchState(CombatDefault);
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
	for (const auto& State : CurrentStateTransitionSet)
	{
		if (UCS_PlayerCombatState* PlayerCombatState = Cast<UCS_PlayerCombatState>(State))
		{
			if (CheckIfCanTransitionState(PlayerCombatState))
			{
				SwitchState(PlayerCombatState);
				return;
			}
		}
		
	}
}

bool UCS_PlayerStateMachineManager::CheckIfCanTransitionState(UCS_PlayerCombatState* State)
{
	if (!IsValid(PlayerCharacter) || !IsValid(PlayerCharacter->GetCSPlayerState())) return false;
	
	if (PlayerCharacter->GetCSPlayerState()->bIsPerformingAction && !PlayerCharacter->GetCSPlayerState()->bAttackWindowOpen)
		return false;
	
	if (!PlayerCharacter->GetCSPlayerController()->CheckInputCriteria(State))
	{
		return false;
	}

	PlayerCharacter->GetCSPlayerState()->bAttackWindowOpen = false;
	return true;
}

