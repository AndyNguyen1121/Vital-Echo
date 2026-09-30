// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/CS_AttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "Capstone/DebugMacros.h"
#include "GameplayTags/CSTags.h"

void UCS_AttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FGameplayEffectContextHandle Context = Data.EffectSpec.GetEffectContext();

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		float OldValue = GetHealth();
		SetHealth(GetHealth() - GetDamage());

		if (GetHealth() < OldValue)
		{
			OnDamaged.Broadcast(OldValue, GetHealth(), Context.GetInstigator());
		}
		
		SetDamage(0);
	}

	if (Data.EvaluatedData.Attribute == GetPostureDamageAttribute())
	{
		SetPosture(GetPosture() + GetPostureDamage());
		SetPostureDamage(0.f);
	}

	if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		SetHealth(GetMaxHealth());
	}
}

void UCS_AttributeSet::PostAttributesInitialized()
{
	if (!bAttributesInitialized)
	{
		bAttributesInitialized = true;
		OnAttributesInitialized.Broadcast();
	}
}

void UCS_AttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetMaxHealth());
	}

	if (Attribute == GetPostureAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetPostureThreshold());
	}
}
