// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/Player/CS_PlayerCombatState.h"

void UCS_PlayerCombatState::SortByInput()
{
	TSet<UCS_CombatStateBase*> Visited;
	SortByInputRecursive(Visited);
}

void UCS_PlayerCombatState::SortByInputRecursive(TSet<UCS_CombatStateBase*>& Visited)
{
	if (Visited.Contains(this))
	{
		return; 
	}
	Visited.Add(this);

	TransitionStates.Sort([](const UCS_CombatStateBase& A, const UCS_CombatStateBase& B)
	{
		const UCS_PlayerCombatState* StateA = Cast<UCS_PlayerCombatState>(&A);
		const UCS_PlayerCombatState* StateB = Cast<UCS_PlayerCombatState>(&B);

		const int32 NumInputsA = StateA ? StateA->RequiredInputs.Num() : 0;
		const int32 NumInputsB = StateB ? StateB->RequiredInputs.Num() : 0;

		return NumInputsA > NumInputsB;
	});

	for (UCS_CombatStateBase* Child : TransitionStates)
	{
		if (UCS_PlayerCombatState* ChildState = Cast<UCS_PlayerCombatState>(Child))
		{
			ChildState->SortByInputRecursive(Visited);
		}
	}
}
