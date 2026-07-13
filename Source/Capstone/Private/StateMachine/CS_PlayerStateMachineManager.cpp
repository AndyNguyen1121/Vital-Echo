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
}

void UCS_PlayerStateMachineManager::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckForStateTransitions();
}

bool UCS_PlayerStateMachineManager::SwitchState(UCS_CombatStateBase* NextState)
{
	if (!NextState || !IsValid(PlayerCharacter)) return false;
	if (!CombatStateList.Contains(NextState)) return false;

	CurrentState = NextState;
	CurrentStateTransitionSet = NextState->TransitionStates;
	PlayerCharacter->GetAbilitySystemComponent()->TryActivateAbilityByClass(NextState->GameplayAbility);
	PlayerCharacter->ActorsHit.Empty();

	return true;
}

void UCS_PlayerStateMachineManager::ResetToDefaultState()
{
	SwitchState(CombatDefault);
}

void UCS_PlayerStateMachineManager::CheckForStateTransitions()
{
	if (CurrentStateTransitionSet.Num() == 0) return;
	
	for (const auto& State : CurrentStateTransitionSet)
	{
		if (CheckIfCanTransitionState(State))
		{
			SwitchState(State);
			return;
		}
		
	}
}

bool UCS_PlayerStateMachineManager::CheckIfCanTransitionState(UCS_CombatStateBase* State)
{
	if (!IsValid(PlayerCharacter) || !IsValid(PlayerCharacter->GetCSPlayerState())) return false;
	
	UCS_PlayerCombatState* CombatState = Cast<UCS_PlayerCombatState>(State);
	if (!CombatState) return false;
	
	if (PlayerCharacter->GetCSPlayerState()->bIsPerformingAction && !PlayerCharacter->GetCSPlayerState()->bAttackWindowOpen)
		return false;

	if (!PlayerCharacter->GetCSPlayerState()->bIsLockedOn && CombatState->bRequireLockOn)
		return false;
	 
	if (!PlayerCharacter->GetCSPlayerController()->CheckInputCriteria(CombatState))
	{
		return false;
	}

	PlayerCharacter->GetCSPlayerState()->bAttackWindowOpen = false;
	return true;
}

