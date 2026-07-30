// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachine/CS_PlayerStateMachineManager.h"

#include "AbilitySystemComponent.h"
#include "Capstone/DebugMacros.h"
#include "Characters/CS_PlayerCharacter.h"
#include "DataAssets/Base/CS_CombatStateBase.h"
#include "DataAssets/Player/CS_PlayerCombatState.h"
#include "DataAssets/Base/CS_CombatProfile.h"
#include "GameFramework/CharacterMovementComponent.h"
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

	PlayerCharacter->ResetGravityScale();
	CurrentState = NextState;
	CurrentStateTransitionSet = NextState->TransitionStates;

	if (UCS_PlayerCombatState* CombatState = Cast<UCS_PlayerCombatState>(NextState))
	{
		PlayerCharacter->CurrentDamageAttribute = CombatState->DamageAttributes;
		PlayerCharacter->GetCSPlayerState()->ResetBooleansWithStateInfo(CombatState->StateInfo, false);

		if (CombatState->StateInfo.GravityScale > -1)
		{
			PlayerCharacter->GetCharacterMovement()->StopMovementImmediately();
			PlayerCharacter->GetCharacterMovement()->GravityScale = CombatState->StateInfo.GravityScale;
		}
		else
		{
			PlayerCharacter->ResetGravityScale();
		}
	}
	else
	{
		PlayerCharacter->CurrentDamageAttribute = FDamageAttributes();
	}

	if (NextState->GameplayAbility)
	{
		PlayerCharacter->GetAbilitySystemComponent()->TryActivateAbilityByClass(NextState->GameplayAbility);
	}
	
	PlayerCharacter->ActorsHit.Empty();

	return true;
}

void UCS_PlayerStateMachineManager::ResetToDefaultState()
{
	SwitchState(CombatProfile);
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
	
	if (PlayerCharacter->GetCSPlayerState()->bIsPerformingAction && !PlayerCharacter->GetCSPlayerState()->bInputWindowOpen)
	{
		return false;
	}

	if (!PlayerCharacter->GetCSPlayerState()->bIsLockedOn && CombatState->bRequireLockOn)
	{
		return false;
	}

	if (!(PlayerCharacter->GetCharacterMovement()->IsFalling() || PlayerCharacter->GetCharacterMovement()->IsFlying()) != CombatState->Grounded)
	{
		return false;
	}

	// INPUT SHOULD BE CHECKED LAST TO AVOID CONSUMPTION UPON FAILING
	if (!PlayerCharacter->GetCSPlayerController()->CheckInputCriteria(CombatState))
	{
		return false;
	}
	
	PlayerCharacter->GetCSPlayerState()->bInputWindowOpen = false;
	return true;
}

