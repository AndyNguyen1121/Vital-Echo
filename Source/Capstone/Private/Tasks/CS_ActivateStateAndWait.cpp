// Fill out your copyright notice in the Description page of Project Settings.


#include "Tasks/CS_ActivateStateAndWait.h"

#include "ActorComponents/StateMachine/CS_StateMachineManagerBase.h"
#include "Capstone/DebugMacros.h"

UCS_ActivateStateAndWait* UCS_ActivateStateAndWait::ListenForStateEnd(UCS_StateMachineManagerBase* StateMachine, UCS_CombatStateBase* State)
{
	UCS_ActivateStateAndWait* StateWaitTask = NewObject<UCS_ActivateStateAndWait>(StateMachine->GetOwner());
	StateMachine->OnStateChanged.AddDynamic(StateWaitTask, &UCS_ActivateStateAndWait::StateEnd);
	StateWaitTask->StateMachine = StateMachine;
	StateWaitTask->StateToWaitFor = State;
	StateMachine->SwitchState(State);
	return StateWaitTask;
}

void UCS_ActivateStateAndWait::StateEnd(UCS_CombatStateBase* State)
{
	if (State != StateToWaitFor.Get()) return;

	//DEBUG_LOG(State->GetName());
	OnStateEnded.Broadcast(State);
	EndTask();
}

void UCS_ActivateStateAndWait::EndTask()
{
	if (StateMachine.IsValid())
	{
		StateMachine->OnStateChanged.RemoveDynamic(
			this,
			&UCS_ActivateStateAndWait::StateEnd
		);
	}

	SetReadyToDestroy();
}
