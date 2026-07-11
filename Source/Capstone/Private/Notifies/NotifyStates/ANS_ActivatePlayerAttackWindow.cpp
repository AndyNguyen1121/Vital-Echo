// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/NotifyStates/ANS_ActivatePlayerAttackWindow.h"

#include "Characters/CS_PlayerCharacter.h"
#include "Player/CS_PlayerState.h"

void UANS_ActivatePlayerAttackWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                  float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!IsValid(MeshComp) || !IsValid(MeshComp->GetOwner())) return;

	if (ACS_PlayerCharacter* PlayerCharacter = Cast<ACS_PlayerCharacter>(MeshComp->GetOwner()); PlayerCharacter->GetCSPlayerState())
	{
		PlayerCharacter->GetCSPlayerState()->bAttackWindowOpen = true;
	}
}


void UANS_ActivatePlayerAttackWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!IsValid(MeshComp) || !IsValid(MeshComp->GetOwner())) return;

	if (ACS_PlayerCharacter* PlayerCharacter = Cast<ACS_PlayerCharacter>(MeshComp->GetOwner()); PlayerCharacter->GetCSPlayerState())
	{
		PlayerCharacter->GetCSPlayerState()->bAttackWindowOpen = false;
	}
	
}
