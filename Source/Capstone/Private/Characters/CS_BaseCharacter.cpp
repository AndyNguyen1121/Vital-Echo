// Fill out your copyright notice in the Description page of Project Settings.


#include "Capstone/Public/Characters/CS_BaseCharacter.h"

#include "Capstone/Public/Player/CS_PlayerState.h"


// Sets default values
ACS_BaseCharacter::ACS_BaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

UAbilitySystemComponent* ACS_BaseCharacter::GetAbilitySystemComponent() const
{
	return nullptr;
}

