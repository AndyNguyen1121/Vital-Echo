// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachine/CS_StateMachineManagerBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "DataAssets/Base/CS_CombatProfile.h"
#include "DataAssets/Base/CS_CombatStateBase.h"


// Sets default values for this component's properties
UCS_StateMachineManagerBase::UCS_StateMachineManagerBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCS_StateMachineManagerBase::BeginPlay()
{
	Super::BeginPlay();
	if (!CombatDefault) return;
	
	RetrieveStates(CombatDefault, CombatStateList);
	GiveStartupAbilities(CombatDefault);
	CurrentState = CombatDefault;
}

bool UCS_StateMachineManagerBase::SwitchState(UCS_PlayerCombatState* NextState)
{
	return true;
}

void UCS_StateMachineManagerBase::RetrieveStates(UCS_CombatStateBase* State, TSet<UCS_CombatStateBase*>& StateList)
{
	if (!State)
		return;
	
	StateList.Add(State);
	
	if (State->TransitionStates.Num() == 0)
		return;

	/*if (UCS_CombatProfile* CombatProfile = Cast<UCS_CombatProfile>(State))
	{
		for (UCS_CombatProfile* DataAsset : CombatProfile->AdditionalStates)
		{
			RetrieveStates(DataAsset, StateList);
		}
	}*/
	
	for (UCS_CombatStateBase* DataAsset : State->TransitionStates)
	{
		RetrieveStates(DataAsset, StateList);

		UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner());
		if (!DataAsset || !ASC || !DataAsset->GameplayAbility) continue;
			ASC->GiveAbility(DataAsset->GameplayAbility);
	}
}

void UCS_StateMachineManagerBase::GiveStartupAbilities(UCS_CombatProfile* DefaultState)
{
	if (!IsValid(GetOwner()) || !DefaultState) return;
	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner());
	if (!ASC) return;
	
	for (TSubclassOf<UGameplayAbility> Ability : DefaultState->StartUpAbilities)
	{
		if (!Ability) continue;
		
		ASC->GiveAbility(Ability);
	}
}


