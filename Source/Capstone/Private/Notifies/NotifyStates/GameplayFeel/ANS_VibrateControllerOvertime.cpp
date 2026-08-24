// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/NotifyStates/GameplayFeel/ANS_VibrateControllerOvertime.h"

#include "Kismet/GameplayStatics.h"

void UANS_VibrateControllerOvertime::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                 float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!IsValid(MeshComp))
		return;

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(MeshComp, 0);
	if (PlayerController)
	{
		PlayerController->PlayDynamicForceFeedback(Intensity, TotalDuration, true, true, true, true);
	}
}
