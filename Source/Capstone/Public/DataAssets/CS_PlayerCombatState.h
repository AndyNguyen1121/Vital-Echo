// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/CS_CombatStateBase.h"
#include "Engine/DataAsset.h"
#include "Enums/InputTypes.h"
#include "CS_PlayerCombatState.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONE_API UCS_PlayerCombatState : public UCS_CombatStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="Inputs")
	TArray<EInputType> RequiredInputs;

	UPROPERTY(EditAnywhere, Category="Inputs")
	int InputBufferWindow;

	UPROPERTY(EditAnywhere, Category="Inputs")
	bool bRequireLockOn;
	
};
