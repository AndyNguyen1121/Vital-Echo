// Fill out your copyright notice in the Description page of Project Settings.


#include "Capstone/Public/Player/CS_PlayerState.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AttributeSets/CS_AttributeSet.h"
#include "AbilitySystem/AttributeSets/CS_Player_AttributeSet.h"
#include "Capstone/DebugMacros.h"
#include "Characters/CS_PlayerCharacter.h"
#include "DataAssets/Player/CS_PlayerCombatState.h"
#include "ActorComponents/StateMachine/CS_PlayerStateMachineManager.h"

ACS_PlayerState::ACS_PlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UCS_AbilitySystemComponent>("AbilitySystemComponent");
	AttributeSet = CreateDefaultSubobject<UCS_Player_AttributeSet>("AttributeSet");
}

UCS_AbilitySystemComponent* ACS_PlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
