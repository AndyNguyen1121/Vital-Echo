// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/NotifyStates/ANS_AddGameplayTagForDuration.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagAssetInterface.h"
#include "Characters/CS_BaseCharacter.h"

void UANS_AddGameplayTagForDuration::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                 float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!IsValid(MeshComp)) return;

	ACS_BaseCharacter* Character = Cast<ACS_BaseCharacter>(MeshComp->GetOwner());
	if (!IsValid(Character) || !Character->GetAbilitySystemComponent()) return;
	
	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	if (!AnimInstance) return;
	
	UAnimMontage* Montage = Cast<UAnimMontage>(Animation);
	TWeakObjectPtr<ACS_BaseCharacter> WeakCharacter = Character;

	Character->GetAbilitySystemComponent()->AddLooseGameplayTag(Tag);
	ActiveCharacters.Add(WeakCharacter);

	BindMontageEnd(WeakCharacter.Get(), AnimInstance, Montage, [this, WeakCharacter](UAnimMontage*, bool)
	{
		if (!ActiveCharacters.Contains(WeakCharacter))
			return;
		
		if (WeakCharacter.IsValid() && WeakCharacter->GetAbilitySystemComponent())
		{
			WeakCharacter->GetAbilitySystemComponent()->RemoveLooseGameplayTag(Tag);
			ActiveCharacters.Remove(WeakCharacter);
		}
	});
}

void UANS_AddGameplayTagForDuration::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!IsValid(MeshComp)) return;

	ACS_BaseCharacter* Character = Cast<ACS_BaseCharacter>(MeshComp->GetOwner());
	if (!IsValid(Character) || !Character->GetAbilitySystemComponent()) return;

	if (!ActiveCharacters.Contains(Character)) return;

	Character->GetAbilitySystemComponent()->RemoveLooseGameplayTag(Tag);
	ActiveCharacters.Remove(Character);
}
