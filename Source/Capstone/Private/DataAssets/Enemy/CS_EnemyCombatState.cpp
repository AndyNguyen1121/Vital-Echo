// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/Enemy/CS_EnemyCombatState.h"

#include "GameplayTags/CSTags.h"

UCS_EnemyCombatState::UCS_EnemyCombatState()
{
	DamageAttributes.IDsToIgnore = FGameplayTagContainer(CSTags::IDs::Enemy);
}
