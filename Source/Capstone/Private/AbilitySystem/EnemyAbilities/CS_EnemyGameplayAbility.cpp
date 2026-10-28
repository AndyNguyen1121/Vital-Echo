// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/EnemyAbilities/CS_EnemyGameplayAbility.h"

#include "Characters/Enemy/CS_EnemyCharacter.h"

void UCS_EnemyGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                               const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                               const FGameplayEventData* TriggerEventData)
{
	EnemyCharacter = Cast<ACS_EnemyCharacter>(GetAvatarActorFromActorInfo());
	
	if (!IsValid(EnemyCharacter))
	{
		K2_EndAbility();
	}
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
