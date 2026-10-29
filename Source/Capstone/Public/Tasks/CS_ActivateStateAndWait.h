// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "DataAssets/Base/CS_CombatStateBase.h"
#include "CS_ActivateStateAndWait.generated.h"

class UCS_StateMachineManagerBase;
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateEnded, UCS_CombatStateBase*, State);
UCLASS(BlueprintType, meta=(ExposedAsyncProxy = AsyncTask))
class CAPSTONE_API UCS_ActivateStateAndWait : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnStateEnded OnStateEnded;

	UFUNCTION(BlueprintCallable, meta= (BlueprintInternalUseOnly = "true"))
	static UCS_ActivateStateAndWait* ListenForStateEnd(UCS_StateMachineManagerBase* StateMachine, UCS_CombatStateBase* State);

	UFUNCTION()
	void StateEnd(UCS_CombatStateBase* State);

	UFUNCTION(BlueprintCallable)
	void EndTask();

	UPROPERTY()
	TWeakObjectPtr<UCS_StateMachineManagerBase> StateMachine;

	UPROPERTY()
	TWeakObjectPtr<UCS_CombatStateBase> StateToWaitFor; 
};
