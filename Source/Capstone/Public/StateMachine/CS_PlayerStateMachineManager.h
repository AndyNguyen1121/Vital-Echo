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
	virtual bool SwitchState(UCS_PlayerCombatState* NextState) override;
	bool SwitchState(UCS_CombatStateBase* NextState);
	void ResetToDefaultState();

protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
	TObjectPtr<ACS_PlayerCharacter> PlayerCharacter;

private:
	void ConvertStatesToPlayerCombatState(const TSet<UCS_CombatStateBase*>& StateBases);
	void CheckForStateTransitions();
	bool CheckIfCanTransitionState(UCS_PlayerCombatState* State);

private:
	UPROPERTY(VisibleAnywhere)
	TSet<UCS_PlayerCombatState*> ValidPlayerStates;
};
