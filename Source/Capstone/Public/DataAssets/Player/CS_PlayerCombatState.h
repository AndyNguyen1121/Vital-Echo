// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Base/CS_CombatStateBase.h"
#include "Engine/DataAsset.h"
#include "Enums/InputTypes.h"
#include "Structs/DamageAttributes.h"
#include "CS_PlayerCombatState.generated.h"

/**
 * 
 */
USTRUCT(Blueprintable)
struct FPlayerStateInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	bool bIsPerformingAction = true;

	UPROPERTY(EditAnywhere)
	bool bCanMove = false;

	UPROPERTY(EditAnywhere)
	bool bCanRotate = false;

	UPROPERTY(EditAnywhere)
	float GravityScale = -1;
};

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

	UPROPERTY(EditAnywhere, Category="Criteria")
	bool Grounded;

	UPROPERTY(EditAnywhere, Category="SetFlags")
	FPlayerStateInfo StateInfo;

	UPROPERTY(EditAnywhere)
	FDamageAttributes DamageAttributes;

public:
	void SortByInput();

	void SortByInputRecursive(TSet<UCS_CombatStateBase*>& Visited);
};
