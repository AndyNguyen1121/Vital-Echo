// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/StateMachine/CS_StateMachineManagerBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Capstone/DebugMacros.h"
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
	SwitchCombatProfile(CombatProfile, CombatStateList);
}

bool UCS_StateMachineManagerBase::SwitchState(UCS_CombatStateBase* NextState)
{
	return true;
}

void UCS_StateMachineManagerBase::ResetToDefaultState()
{
	SwitchState(CombatProfile);
}

void UCS_StateMachineManagerBase::RetrieveStates(UCS_CombatStateBase* State, TSet<UCS_CombatStateBase*>& StateList)
{
	if (!State)
		return;
	
	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner());
	
	if (!ASC) return;
	
	if (!StateList.Contains(State))
		StateList.Add(State);
	
	if (State->TransitionStates.Num() == 0)
		return;
	
	for (UCS_CombatStateBase* DataAsset : State->TransitionStates)
	{
		if (!DataAsset) continue;
		RetrieveStates(DataAsset, StateList);
		
		if (!DataAsset->GameplayAbility) continue;
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

void UCS_StateMachineManagerBase::GiveStartupEffects(UCS_CombatProfile* DefaultState)
{
	if (!IsValid(GetOwner()) || !DefaultState) return;
	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner());
	if (!ASC) return;
	
	for (TSubclassOf<UGameplayEffect> Effect : DefaultState->StartupEffects)
	{
		if (!Effect) continue;
		FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(Effect, 1.f, FGameplayEffectContextHandle());
		ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data);

		DEBUG_LOG("Effect applied")
	}
}

void UCS_StateMachineManagerBase::RemoveStartupAbilities()
{
	if (!IsValid(GetOwner())) return;
	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner());
	if (!ASC) return;

	ASC->ClearAllAbilities();
}

void UCS_StateMachineManagerBase::SwitchCombatProfile(UCS_CombatProfile* NewCombatProfile, TSet<UCS_CombatStateBase*>& StateList)
{
	RemoveStartupAbilities();
	CombatStateList.Empty();
	CurrentStateTransitionSet.Empty();

	if (!NewCombatProfile) return;
	
	CombatProfile = NewCombatProfile;
	RetrieveStates(NewCombatProfile, CombatStateList);

	if (NewCombatProfile)
	{
		GiveStartupAbilities(NewCombatProfile);
		GiveStartupEffects(NewCombatProfile);\
	}
	
	SwitchState(NewCombatProfile);
}


