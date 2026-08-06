// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Editor/AnimationModifiers/Public/AnimationModifier.h"
#include "CS_AM_ZeroXYRootMotion.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONE_API UCS_AM_ZeroXYRootMotion : public UAnimationModifier
{
	GENERATED_BODY()

	virtual void OnApply_Implementation(UAnimSequence* AnimationSequence) override;
	virtual void OnRevert_Implementation(UAnimSequence* AnimationSequence) override;

	UPROPERTY()
	TArray<FTransform> OriginalTransformKeys;
	TArray<float> OriginalRootTimes;

	UPROPERTY(EditAnywhere)
	FName BoneName;
};
