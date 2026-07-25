// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Player/CS_PlayerGameplayAbility.h"

#include "Capstone/DebugMacros.h"
#include "Characters/CS_PlayerCharacter.h"
#include "Player/CS_PlayerState.h"

void UCS_PlayerGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                const FGameplayEventData* TriggerEventData)
{
	PlayerCharacter = Cast<ACS_PlayerCharacter>(GetAvatarActorFromActorInfo());
	
	if (!IsValid(PlayerCharacter) || !PlayerCharacter->GetCSPlayerState())
	{
		K2_EndAbility();
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
