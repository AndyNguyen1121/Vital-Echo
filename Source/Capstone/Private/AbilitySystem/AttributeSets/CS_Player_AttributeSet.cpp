// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AttributeSets/CS_Player_AttributeSet.h"

#include "GameplayEffectExtension.h"

void UCS_Player_AttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHeatGaugeLimitAttribute())
	{
		SetHeatGauge(GetHeatGaugeLimit());
	}
}

void UCS_Player_AttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHeatGaugeAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetHeatGaugeLimit());
	}
}



























