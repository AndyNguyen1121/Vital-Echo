// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CS_PlayerCombatState.h"
#include "../Base/CS_CombatProfile.h"
#include "CS_PlayerCombatProfile.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONE_API UCS_PlayerCombatProfile : public UCS_CombatProfile
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<UCS_CombatStateBase*> AnyStateTransitions;

	UPROPERTY()
	TArray<UCS_CombatStateBase*> SortedTransitionStates;

	void SortStates();

private:
	void SortByInput();

	void SortStatesRecursive(TSet<UCS_CombatStateBase*>& Visited);

	//UFUNCTION(CallInEditor)
	
	
};
