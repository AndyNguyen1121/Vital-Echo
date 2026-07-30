// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_Player_TargetLockOnWarp.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONE_API UAN_Player_TargetLockOnWarp : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(EditAnywhere)
	float MinRange;

	UPROPERTY(EditAnywhere)
	float MaxRange = 200.f;

	UPROPERTY(EditAnywhere)
	FVector DistanceOffset;
};
