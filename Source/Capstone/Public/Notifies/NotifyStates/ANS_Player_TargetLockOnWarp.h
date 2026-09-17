// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_Player_TargetLockOnWarp.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONE_API UANS_Player_TargetLockOnWarp : public UAnimNotifyState
{
	GENERATED_BODY()


public:
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	
	UPROPERTY(EditAnywhere)
	float MinRange;

	UPROPERTY(EditAnywhere)
	float MaxRange = 200.f;

	UPROPERTY(EditAnywhere)
	FVector DistanceOffset;
};
