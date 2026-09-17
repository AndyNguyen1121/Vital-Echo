// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/NotifyStates/ANS_Player_TargetLockOnWarp.h"

#include "Characters/CS_PlayerCharacter.h"

void UANS_Player_TargetLockOnWarp::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (!IsValid(MeshComp))
		return;

	ACS_PlayerCharacter* PlayerCharacter = Cast<ACS_PlayerCharacter>(MeshComp->GetOwner());

	if (!IsValid(PlayerCharacter))
		return;

	PlayerCharacter->WarpToLockOnTarget(MinRange, MaxRange, DistanceOffset);
}
