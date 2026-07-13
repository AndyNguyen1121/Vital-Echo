// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/CS_AbilitySystemComponent.h"

#include "Capstone/DebugMacros.h"
#include "GameplayTags/CSTags.h"


void UCS_AbilitySystemComponent::OnGiveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	Super::OnGiveAbility(AbilitySpec);

	if (!AbilitySpec.Ability) return;

	HandleAutoActivatedAbilities(AbilitySpec);
}

void UCS_AbilitySystemComponent::HandleAutoActivatedAbilities(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AutoActivatedAbilities.Contains(AbilitySpec.Handle)) return;

	if (AbilitySpec.Ability->GetAssetTags().HasTag(CSTags::Abilities::ActivateOnGiven))
	{
		AutoActivatedAbilities.Add(AbilitySpec.Handle);
		TryActivateAbility(AbilitySpec.Handle);
	}
}
