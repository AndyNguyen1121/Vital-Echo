// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "BaseScript/ANS_CS_Base.h"
#include "ANS_DragHitActors.generated.h"

class ACS_BaseCharacter;
class ACS_PlayerCharacter;
/**
 * 
 */
UCLASS()
class CAPSTONE_API UANS_DragHitActors : public UANS_CS_Base
{
	GENERATED_BODY()

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	

public:
	UPROPERTY(EditAnywhere)
	FVector EndLaunchVelocity;

	UPROPERTY(EditAnywhere)
	FVector MovementMultiplier = FVector::OneVector;

private:

	UPROPERTY()
	TWeakObjectPtr<ACS_BaseCharacter> CachedCharacter;
};
