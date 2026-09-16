// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Weapons/CS_TrickWeapon.h"
#include "AN_TransformWeapon.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONE_API UAN_TransformWeapon : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
public:
	UPROPERTY(EditAnywhere)
	EWeaponTransformType TransformMode;
};
