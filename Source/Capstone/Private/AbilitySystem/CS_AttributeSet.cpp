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
	FGameplayEventData Payload;

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		if (!GetOwningAbilitySystemComponent()->HasMatchingGameplayTag(CSTags::Status::GameplayStates::PlayerStates::ParryWindowActive))
		{
			
			Payload.Instigator = Context.GetInstigator();
			Payload.Target = GetOwningAbilitySystemComponent()->GetAvatarActor();
			
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningAbilitySystemComponent()->GetAvatarActor(), FGameplayTag(CSTags::Events::Gameplay::HitReact), Payload);
			
			SetHealth(FMath::Clamp(GetHealth() - GetDamage(), 0, GetMaxHealth()));
			SetDamage(0);
		}
		else
		{
			Payload.Instigator = GetOwningAbilitySystemComponent()->GetAvatarActor();;
			Payload.Target = Context.GetInstigator();
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Context.GetInstigator(), CSTags::Events::Gameplay::ParryReceive, Payload);
		}
	}

	if (Data.EvaluatedData.Attribute == GetPostureDamageAttribute())
	{
		AddPostureSafe(GetPostureDamage());
		SetPostureDamage(0.f);
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

void UCS_AttributeSet::AddPostureSafe(float value)
{
	SetPosture(FMath::Clamp(GetPosture() + value, 0, GetPostureThreshold()));
}
