// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/Base/CS_CombatStateBase.h"

#include "GameplayEffect.h"

UCS_CombatStateBase::UCS_CombatStateBase()
{
	ConstructorHelpers::FClassFinder<UGameplayEffect> DefaultDamageEffectFinder(TEXT("/Game/Capstone/GameplayEffects/Damage/GE_Damage_Base.GE_Damage_Base_C"));

	if (DefaultDamageEffectFinder.Succeeded())
	{
		DamageAttributes.DamageEffect = DefaultDamageEffectFinder.Class;
	}
}
