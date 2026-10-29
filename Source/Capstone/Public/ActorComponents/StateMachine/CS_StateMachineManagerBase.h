// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CS_StateMachineManagerBase.generated.h"


class UCS_CombatStateBase;
class UCS_CombatProfile;
class UCS_PlayerCombatState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateChanged, UCS_CombatStateBase*, State);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CAPSTONE_API UCS_StateMachineManagerBase : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCS_StateMachineManagerBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat States")
	TObjectPtr<UCS_CombatProfile> CombatProfile;

	UPROPERTY(BlueprintReadOnly, Category = "Combat States", VisibleAnywhere)
	TObjectPtr<UCS_CombatStateBase> CurrentState;

public:
	UFUNCTION(BlueprintCallable)
	virtual bool SwitchState(UCS_CombatStateBase* NextState);

	UFUNCTION(BlueprintCallable)
	void ResetToDefaultState();

	UPROPERTY()
	FOnStateChanged OnStateChanged;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void RetrieveStates(UCS_CombatStateBase* State, TSet<UCS_CombatStateBase*>& StateList);
	virtual void GiveStartupAbilities(UCS_CombatProfile* DefaultState);
	virtual void GiveStartupEffects(UCS_CombatProfile* DefaultState);
	void RemoveStartupAbilities();
	virtual void SwitchCombatProfile(UCS_CombatProfile* NewCombatProfile, TSet<UCS_CombatStateBase*>& StateList);

protected:
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "true"), Category="Combat States")
	TSet<UCS_CombatStateBase*> CombatStateList;

	UPROPERTY(VisibleAnywhere)
	TArray<UCS_CombatStateBase*> CurrentStateTransitionSet;

};
