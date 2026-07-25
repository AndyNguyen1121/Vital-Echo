// Fill out your copyright notice in the Description page of Project Settings.


#include "Capstone/Public/Player/CS_PlayerState.h"

#include "AbilitySystemComponent.h"
#include "Capstone/DebugMacros.h"
#include "Characters/CS_PlayerCharacter.h"
#include "DataAssets/Player/CS_PlayerCombatState.h"
#include "StateMachine/CS_PlayerStateMachineManager.h"

ACS_PlayerState::ACS_PlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UCS_AbilitySystemComponent>("AbilitySystemComponent");
}

UCS_AbilitySystemComponent* ACS_PlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACS_PlayerState::ResetBooleans(bool CanMove, bool IsPerformingAction, bool CanRotate, bool ResetToDefaultState)
{
	bCanMove = CanMove;
	bIsPerformingAction = IsPerformingAction;
	bCanRotate = CanRotate;
	
	ACS_PlayerCharacter* PlayerCharacter = Cast<ACS_PlayerCharacter>(GetPawn());
	if (!IsValid(PlayerCharacter)) return;
	
	if (ResetToDefaultState)
	{
		PlayerCharacter->GetStateMachineManager()->ResetToDefaultState();
	}
}

void ACS_PlayerState::ResetBooleansWithStateInfo(FPlayerStateInfo StateInfo, bool ResetToDefaultState)
{
	bCanMove = StateInfo.bCanMove;
	bIsPerformingAction =  StateInfo.bIsPerformingAction;
	bCanRotate =  StateInfo.bCanRotate;
	
	ACS_PlayerCharacter* PlayerCharacter = Cast<ACS_PlayerCharacter>(GetPawn());
	if (!IsValid(PlayerCharacter)) return;
	
	if (ResetToDefaultState)
	{
		PlayerCharacter->GetStateMachineManager()->ResetToDefaultState();
	}
}
