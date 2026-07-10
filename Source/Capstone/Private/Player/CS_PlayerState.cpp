// Fill out your copyright notice in the Description page of Project Settings.


#include "Capstone/Public/Player/CS_PlayerState.h"

#include "AbilitySystemComponent.h"

ACS_PlayerState::ACS_PlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
}

UAbilitySystemComponent* ACS_PlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
