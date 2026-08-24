// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_VibrateControllerOvertime.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONE_API UANS_VibrateControllerOvertime : public UAnimNotifyState
{
	GENERATED_BODY()

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	
public:
	UPROPERTY(EditAnywhere)
	float Intensity = 1.f;
};
