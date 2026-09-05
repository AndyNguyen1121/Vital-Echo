// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Base/CS_CombatStateBase.h"
#include "Engine/DataAsset.h"
#include "Enums/InputTypes.h"
#include "Structs/CharacterStateInfo.h"
#include "Structs/DamageAttributes.h"
#include "CS_PlayerCombatState.generated.h"

/**
 * 
 */

UCLASS()
class CAPSTONE_API UCS_PlayerCombatState : public UCS_CombatStateBase
{
	GENERATED_BODY()

public:
	UCS_PlayerCombatState();
	
	UPROPERTY(EditAnywhere, Category="Inputs")
	TArray<EInputType> RequiredInputs;

	UPROPERTY(EditAnywhere, Category="Inputs")
	int InputBufferWindow;

	UPROPERTY(EditAnywhere, meta= (EditCondition = "bHasPauseInput == true", EditConditionHides), Category="Inputs")
	int PauseMinFrames;

	UPROPERTY(EditAnywhere, meta= (EditCondition = "bHasPauseInput == true", EditConditionHides), Category="Inputs")
	int PauseMaxFrames;

	UPROPERTY(EditAnywhere, Category="Inputs")
	bool bRequireLockOn;

	UPROPERTY(EditAnywhere, Category="Criteria")
	bool Grounded;

	UPROPERTY(EditAnywhere, Category="SetFlags")
	FCharacterStateInfo StateInfo;

	UPROPERTY()
	TArray<UCS_CombatStateBase*> SortedTransitionStates;

	TArray<UCS_CombatStateBase*> CachedAnyStateTransitions;

	UPROPERTY()
	bool bHasPauseInput;
	

public:
	void SortStatesRecursive(TSet<UCS_CombatStateBase*>& Visited, TArray<UCS_CombatStateBase*> AnyStateTransitions);
	void SortStates();

private:
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
};
