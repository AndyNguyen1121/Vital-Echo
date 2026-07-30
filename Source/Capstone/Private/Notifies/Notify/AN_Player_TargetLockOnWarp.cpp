// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/Notify/AN_Player_TargetLockOnWarp.h"

#include "Characters/CS_PlayerCharacter.h"

void UAN_Player_TargetLockOnWarp::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                         const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!IsValid(MeshComp))
		return;

	ACS_PlayerCharacter* PlayerCharacter = Cast<ACS_PlayerCharacter>(MeshComp->GetOwner());

	if (!IsValid(PlayerCharacter))
		return;

	PlayerCharacter->WarpToLockOnTarget(MinRange, MaxRange, DistanceOffset);
}
