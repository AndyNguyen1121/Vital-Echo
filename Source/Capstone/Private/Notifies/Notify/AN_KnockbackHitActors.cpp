// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/Notify/AN_KnockbackHitActors.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Characters/CS_PlayerCharacter.h"
#include "GameplayTags/CSTags.h"
#include "Utils/CS_BlueprintLibrary.h"

void UAN_KnockbackHitActors::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                    const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!IsValid(MeshComp))
		return;

	ACS_PlayerCharacter* PlayerCharacter = Cast<ACS_PlayerCharacter>(MeshComp->GetOwner());

	if (!IsValid(PlayerCharacter))
		return;

	for (AActor* HitActor : PlayerCharacter->ActorsHit)
	{
		if (ACS_BaseCharacter* TargetActor = Cast<ACS_BaseCharacter>(HitActor))
		{
			FGameplayEventData Payload;
			Payload.Instigator = PlayerCharacter;
			Payload.Target = TargetActor;

			UCS_BlueprintLibrary::KnockUpActor(TargetActor, PlayerCharacter, KnockbackForce, bUseSourceActorRotation);
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetActor, FGameplayTag(CSTags::Events::HitReact), Payload);
		}
	}
}
