// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/CS_AttributeSet.h"

#include "GameplayEffectExtension.h"

void UCS_AttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		SetHealth(GetHealth() - GetDamage());
		SetDamage(0);
	}
}
