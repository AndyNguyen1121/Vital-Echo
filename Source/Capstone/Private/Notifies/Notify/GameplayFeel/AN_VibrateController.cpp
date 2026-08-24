// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/Notify/GameplayFeel/AN_VibrateController.h"
#include "Kismet/GameplayStatics.h"

void UAN_VibrateController::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (!IsValid(MeshComp))
		return;

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(MeshComp, 0);
	if (PlayerController)
	{
		FForceFeedbackParameters Parameters;
		PlayerController->ClientPlayForceFeedback(VibrationEffect, Parameters);
	}
}
