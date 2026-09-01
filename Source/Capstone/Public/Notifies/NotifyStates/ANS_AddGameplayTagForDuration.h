// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "BaseScript/ANS_CS_Base.h"
#include "ANS_AddGameplayTagForDuration.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONE_API UANS_AddGameplayTagForDuration : public UANS_CS_Base
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FGameplayTag Tag;

	TSet<TWeakObjectPtr<AActor>> ActiveCharacters;
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
