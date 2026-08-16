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
	
	Character->GetCharacterMovement()->SetMovementMode(StartMode);

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	if (!AnimInstance) return;

	
	UAnimMontage* Montage = Cast<UAnimMontage>(Animation);
	
	if (Montage && AnimInstance->Montage_IsPlaying(Montage))
	{
		// Clear any existing end delegate before setting a new one
		FOnMontageEnded EmptyDelegate;
		AnimInstance->Montage_SetEndDelegate(EmptyDelegate, Montage);
	}

	TWeakObjectPtr<ACharacter> WeakCharacter = Character;
	EMovementMode CapturedEndMode = EndMode;
	
	FOnMontageEnded EndDelegate = FOnMontageEnded::CreateWeakLambda(Character, [WeakCharacter, CapturedEndMode] (UAnimMontage* Montage, bool bInterrupted)
	{
		if (WeakCharacter.IsValid())
		{
			WeakCharacter.Get()->GetCharacterMovement()->SetMovementMode(CapturedEndMode);
		}
	});
	
	AnimInstance->Montage_SetEndDelegate(EndDelegate);

}

void UANS_SetMovementMode::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!IsValid(MeshComp)) return;

	ACS_BaseCharacter* Character = Cast<ACS_BaseCharacter>(MeshComp->GetOwner());
	if (!IsValid(Character)) return;
	
	Character->GetCharacterMovement()->SetMovementMode(EndMode);

	
}