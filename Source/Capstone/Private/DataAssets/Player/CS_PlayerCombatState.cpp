// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/Player/CS_PlayerCombatState.h"

#include "GameplayEffect.h"

UCS_PlayerCombatState::UCS_PlayerCombatState()
{
	ConstructorHelpers::FClassFinder<UCameraShakeBase> DefaultCameraShakeFinder(TEXT("/Game/Capstone/GameplayFeel/CameraShakes/BP_CameraShake_Attack.BP_CameraShake_Attack_C"));

	if (DefaultCameraShakeFinder.Succeeded())
	{
		DamageFeedback.CameraShakeClass = DefaultCameraShakeFinder.Class;
	}

	ConstructorHelpers::FObjectFinder<UForceFeedbackEffect> DefaultVibrationFinder(TEXT("/Game/Capstone/GameplayFeel/ControllerVibrations/FFE_Medium.FFE_Medium"));

	if (DefaultVibrationFinder.Succeeded())
	{
		DamageFeedback.VibrationEffect = DefaultVibrationFinder.Object;
	}
}

void UCS_PlayerCombatState::SortStatesRecursive(TSet<UCS_CombatStateBase*>& Visited, TArray<UCS_CombatStateBase*> AnyStateTransitions)
{
	if (Visited.Contains(this))
	{
		return; 
	}
	Visited.Add(this);
	CachedAnyStateTransitions = AnyStateTransitions;
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

	if (!CanTransitionToSelf)
	{
		CombinedStates.Remove(this);
	}

	for (UCS_CombatStateBase* Child : TransitionStates)
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

void UCS_PlayerCombatState::SortStates()
{
	TArray<UCS_CombatStateBase*> CombinedStates(TransitionStates);
	CombinedStates.Append(CachedAnyStateTransitions);

	if (!CanTransitionToSelf)
	{
		CombinedStates.Remove(this);
	}
	
	CombinedStates.Sort([](const UCS_CombatStateBase& A, const UCS_CombatStateBase& B)
	{
		const UCS_PlayerCombatState* StateA = Cast<UCS_PlayerCombatState>(&A);
		const UCS_PlayerCombatState* StateB = Cast<UCS_PlayerCombatState>(&B);

		const int32 NumInputsA = StateA ? StateA->RequiredInputs.Num() : 0;
		const int32 NumInputsB = StateB ? StateB->RequiredInputs.Num() : 0;

		return NumInputsA > NumInputsB;
	});

	SortedTransitionStates = CombinedStates;
}

void UCS_PlayerCombatState::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	bHasPauseInput = RequiredInputs.Contains(EInputType::EIT_Pause);
	SortStates();

	//if (GameplayAbility->GetName() == )
}
