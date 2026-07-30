// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/NotifyStates/ANS_ActivateInputWindow.h"

#include "Characters/CS_PlayerCharacter.h"
#include "Player/CS_PlayerState.h"

void UANS_ActivateInputWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                  float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!IsValid(MeshComp))
		return;

	ACS_PlayerCharacter* PlayerCharacter = Cast<ACS_PlayerCharacter>(MeshComp->GetOwner());

	if (!IsValid(PlayerCharacter))
		return;

	if (ACS_PlayerState* PlayerState = PlayerCharacter->GetCSPlayerState())
	{
		PlayerState->bInputWindowOpen = true;
	}
}


void UANS_ActivateInputWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!IsValid(MeshComp))
		return;

	ACS_PlayerCharacter* PlayerCharacter = Cast<ACS_PlayerCharacter>(MeshComp->GetOwner());

	if (!IsValid(PlayerCharacter))
		return;

	if (ACS_PlayerState* PlayerState = PlayerCharacter->GetCSPlayerState())
	{
		PlayerState->bInputWindowOpen = false;
	}
	
}
