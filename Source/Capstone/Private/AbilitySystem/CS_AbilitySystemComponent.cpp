// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/CS_AbilitySystemComponent.h"

#include "Capstone/DebugMacros.h"
#include "GameplayTags/CSTags.h"


bool UCS_AbilitySystemComponent::CanActivateAbilityByClass(TSubclassOf<UGameplayAbility> AbilityClass) const
{
	if (!AbilityClass)
		return false;

	// Find the granted ability
	FGameplayAbilitySpec* Spec = FindAbilitySpecFromClass(AbilityClass);

	if (!Spec)
		return false;
	
	UGameplayAbility* Ability = Spec->GetPrimaryInstance();

	if (!Ability)
		Ability = Spec->Ability;

	if (!Ability)
		return false;

	// Get the ASC's actor information
	const FGameplayAbilityActorInfo* ActorInfo = AbilityActorInfo.Get();

	if (!ActorInfo || !ActorInfo->OwnerActor.IsValid()
		|| !ActorInfo->AvatarActor.IsValid())
		return false;

	// Check without activating
	return Ability->CanActivateAbility(
		Spec->Handle,
		ActorInfo,
		nullptr,
		nullptr,
		nullptr
	);
}

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
