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

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<ACS_PlayerCharacter> PlayerCharacter;

private:
	void ConvertStatesToPlayerCombatState(const TSet<UCS_CombatStateBase*>& StateBases);

private:
	UPROPERTY(VisibleAnywhere)
	TSet<UCS_PlayerCombatState*> ValidPlayerStates;
};
