// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/Player/CS_PlayerCombatProfile.h"
void UCS_PlayerCombatProfile::SortByInput()
{
	TSet<UCS_CombatStateBase*> Visited;
	SortStatesRecursive(Visited);
}

void UCS_PlayerCombatProfile::SortStatesRecursive(TSet<UCS_CombatStateBase*>& Visited)
{
	if (Visited.Contains(this))
	{
		return; 
	}
	Visited.Add(this);

	TArray<UCS_CombatStateBase*> CombinedStates(TransitionStates);
	CombinedStates.Append(AnyStateTransitions);

	CombinedStates.Sort([](const UCS_CombatStateBase& A, const UCS_CombatStateBase& B)
	{
		const UCS_PlayerCombatState* StateA = Cast<UCS_PlayerCombatState>(&A);
		const UCS_PlayerCombatState* StateB = Cast<UCS_PlayerCombatState>(&B);

		const int32 NumInputsA = StateA ? StateA->RequiredInputs.Num() : 0;
		const int32 NumInputsB = StateB ? StateB->RequiredInputs.Num() : 0;

		return NumInputsA > NumInputsB;
	});

	SortedTransitionStates = CombinedStates;

	for (UCS_CombatStateBase* Child : CombinedStates)
	{
		if (UCS_PlayerCombatState* ChildState = Cast<UCS_PlayerCombatState>(Child))
		{
			ChildState->SortStatesRecursive(Visited, AnyStateTransitions);
		}
	}

#if WITH_EDITOR
	PostEditChange();
	MarkPackageDirty();
#endif
}

void UCS_PlayerCombatProfile::SortStates()
{
	SortByInput();
}

