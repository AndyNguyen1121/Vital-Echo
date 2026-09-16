// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/Notify/AN_TransformWeapon.h"
#include "Weapons/CS_TrickWeapon.h"
#include "Characters/CS_BaseCharacter.h"

void UAN_TransformWeapon::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                 const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (!IsValid(MeshComp))
		return;

	ACS_BaseCharacter* Character = Cast<ACS_BaseCharacter>(MeshComp->GetOwner());

	if (!IsValid(Character) || !IsValid(Character->GetCurrentEquippedWeapon()))
		return;
	

	if (ACS_TrickWeapon* TrickWeapon = Cast<ACS_TrickWeapon>(Character->GetCurrentEquippedWeapon()))
	{
		TrickWeapon->PlayWeaponTransformAnimation(TransformMode, Character);
	}
}
