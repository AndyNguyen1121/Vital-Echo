// Fill out your copyright notice in the Description page of Project Settings.


#include "Capstone/Public/Player/CS_PlayerState.h"

#include "AbilitySystemComponent.h"
#include "Capstone/DebugMacros.h"
#include "Characters/CS_PlayerCharacter.h"
#include "StateMachine/CS_PlayerStateMachineManager.h"

ACS_PlayerState::ACS_PlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
}

UAbilitySystemComponent* ACS_PlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACS_PlayerState::ResetBooleans(bool CanMove, bool IsPerformingAction, bool CanRotate, bool ResetToDefaultState)
{
	bCanMove = CanMove;
	bIsPerformingAction = IsPerformingAction;
	bCanRotate = CanRotate;

	if (ResetToDefaultState)
	{
		if (ACS_PlayerCharacter* PlayerCharacter = Cast<ACS_PlayerCharacter>(GetPawn()))
		{
			PlayerCharacter->GetStateMachineManager()->ResetToDefaultState();
		}
	}
}
