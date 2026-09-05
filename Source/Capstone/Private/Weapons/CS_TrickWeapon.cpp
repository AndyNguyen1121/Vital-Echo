// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CS_TrickWeapon.h"


void ACS_TrickWeapon::PlayWeaponTransformAnimation(EWeaponTransformType TransformType)
{
	UAnimMontage* AnimationToPlay = nullptr;

	switch (TransformType)
	{
	case EWeaponTransformType::EWTS_Primary:
		AnimationToPlay = GetPrimaryWeaponAnimation();
		break;
		
	case EWeaponTransformType::EWTS_Secondary:
		AnimationToPlay = GetSecondaryWeaponAnimation();
		break;

	default:
		break;
	}

	if (!AnimationToPlay || !GetWeaponMesh()) return;

	GetWeaponMesh()->GetAnimInstance()->Montage_Play(AnimationToPlay, 1);
}
