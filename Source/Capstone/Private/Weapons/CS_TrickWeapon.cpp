// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CS_TrickWeapon.h"

#include "Characters/CS_BaseCharacter.h"


void ACS_TrickWeapon::PlayWeaponTransformAnimation(EWeaponTransformType TransformType, AActor* ParentActor)
{
	UAnimMontage* AnimationToPlay = nullptr;
	EWeaponType WeaponType;

	switch (TransformType)
	{
	case EWeaponTransformType::EWTS_Primary:
		AnimationToPlay = GetPrimaryWeaponAnimation();
		WeaponType = PrimaryWeaponType;
		break;
		
	case EWeaponTransformType::EWTS_Secondary:
		AnimationToPlay = GetSecondaryWeaponAnimation();
		WeaponType = SecondaryWeaponType;
		break;

	default:
		WeaponType = PrimaryWeaponType;
		break;
	}

	if (!AnimationToPlay || !GetWeaponMesh()) return;

	GetWeaponMesh()->GetAnimInstance()->Montage_Play(AnimationToPlay, 1);

	if (ACS_BaseCharacter* BaseCharacter = Cast<ACS_BaseCharacter>(ParentActor))
	{
		BaseCharacter->ChangeWeaponType(WeaponType);
	}
}