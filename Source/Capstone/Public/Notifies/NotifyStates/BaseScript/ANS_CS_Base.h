// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_CS_Base.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONE_API UANS_CS_Base : public UAnimNotifyState
{
	GENERATED_BODY()

protected:
	void BindMontageEnd(UObject* InUserObject, UAnimInstance* AnimInstance, UAnimMontage* Montage, TFunction<void(UAnimMontage*, bool)> Callback);
};
