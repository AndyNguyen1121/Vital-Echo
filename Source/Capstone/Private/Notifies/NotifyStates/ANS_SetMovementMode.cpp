// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/NotifyStates/ANS_SetMovementMode.h"

#include "Characters/CS_BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UANS_SetMovementMode::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                       float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!IsValid(MeshComp)) return;

	ACS_BaseCharacter* Character = Cast<ACS_BaseCharacter>(MeshComp->GetOwner());
	if (!IsValid(Character)) return;

	CachedCharacter = Character;

	if (UAnimInstance* AnimInstance = MeshComp->GetAnimInstance())
	{
		AnimInstance->OnMontageBlendingOut.AddDynamic(this, &ThisClass::OnMontageEnded);
	}

	Character->GetCharacterMovement()->SetMovementMode(StartMode);
}

void UANS_SetMovementMode::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!CachedCharacter.IsValid()) return;
	CachedCharacter->GetCharacterMovement()->SetMovementMode(EndMode);
}

void UANS_SetMovementMode::OnMontageEnded(UAnimMontage* Montage, bool Interrupted)
{
	if (!CachedCharacter.IsValid()) return;
	CachedCharacter->GetCharacterMovement()->SetMovementMode(EndMode);
}
