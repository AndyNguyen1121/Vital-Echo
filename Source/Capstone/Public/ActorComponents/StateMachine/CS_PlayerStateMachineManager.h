// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CS_StateMachineManagerBase.h"
#include "CS_PlayerStateMachineManager.generated.h"


class ACS_PlayerCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CAPSTONE_API UCS_PlayerStateMachineManager : public UCS_StateMachineManagerBase
{
	GENERATED_BODY()

public:
	UCS_PlayerStateMachineManager();
	virtual bool SwitchState(UCS_CombatStateBase* NextState) override;
	void ResetToDefaultState();
	

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void RetrieveStates(UCS_CombatStateBase* State, TSet<UCS_CombatStateBase*>& StateList) override;

protected:
	UPROPERTY()
	TObjectPtr<ACS_PlayerCharacter> PlayerCharacter;

private:
	void CheckForStateTransitions();
	bool CheckIfCanTransitionState(UCS_CombatStateBase* State);
};
