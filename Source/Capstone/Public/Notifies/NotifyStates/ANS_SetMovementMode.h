// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_SetMovementMode.generated.h"

class ACS_BaseCharacter;
/**
 * 
 */
UCLASS()
class CAPSTONE_API UANS_SetMovementMode : public UAnimNotifyState
{
	GENERATED_BODY()

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool Interrupted);

public:
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EMovementMode> StartMode = MOVE_Flying;
	
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EMovementMode> EndMode = MOVE_Falling;

	UPROPERTY()
	TWeakObjectPtr<ACS_BaseCharacter> CachedCharacter;
};
