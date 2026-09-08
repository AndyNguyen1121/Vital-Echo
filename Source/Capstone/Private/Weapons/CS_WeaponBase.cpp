// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CS_WeaponBase.h"

#include "Characters/CS_BaseCharacter.h"

ACS_WeaponBase::ACS_WeaponBase()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(SceneComponent);
	
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	WeaponMesh->SetupAttachment(RootComponent);
}

void ACS_WeaponBase::Unequip(AActor* ParentActor)
{
	SetActorHiddenInGame(true);
	BP_Unequip(ParentActor);
}

void ACS_WeaponBase::Equip(AActor* ParentActor)
{
	SetActorHiddenInGame(false);

	if (WeaponMesh->GetAnimInstance())
	{
		WeaponMesh->GetAnimInstance()->StopAllMontages(0);
	}
	
	BP_Equip(ParentActor);

	if (ACS_BaseCharacter* BaseCharacter = Cast<ACS_BaseCharacter>(ParentActor))
	{
		BaseCharacter->ChangeWeaponType(PrimaryWeaponType);
	}
}
