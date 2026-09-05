// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/StateMachine/CS_PlayerStateMachineManager.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Capstone/DebugMacros.h"
#include "Characters/CS_PlayerCharacter.h"
#include "DataAssets/Base/CS_CombatStateBase.h"
#include "DataAssets/Player/CS_PlayerCombatState.h"
#include "DataAssets/Base/CS_CombatProfile.h"
#include "DataAssets/Player/CS_PlayerCombatProfile.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayTags/CSTags.h"
#include "Player/CS_PlayerController.h"
#include "Player/CS_PlayerState.h"
#include "Utils/CS_BlueprintLibrary.h"
#include "Weapons/CS_WeaponBase.h"


// Sets default values for this component's properties
UCS_PlayerStateMachineManager::UCS_PlayerStateMachineManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UCS_PlayerStateMachineManager::BeginPlay()
{
	PlayerCharacter = Cast<ACS_PlayerCharacter>(GetOwner());
	if (!IsValid(PlayerCharacter)) return;
	PlayerCharacter->OnWeaponEquipped.AddDynamic(this, &ThisClass::UCS_PlayerStateMachineManager::ChangePlayerWeaponCombatProfile);
	
	Super::BeginPlay();
}

void UCS_PlayerStateMachineManager::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckForStateTransitions();
}

bool UCS_PlayerStateMachineManager::SwitchState(UCS_CombatStateBase* NextState)
{
	if (!NextState || !IsValid(PlayerCharacter)) return false;
	if (!CombatStateList.Contains(NextState)) return false;

	if (NextState->GameplayAbility)
	{
		bool bCanActivate = PlayerCharacter->GetAbilitySystemComponent()->TryActivateAbilityByClass(NextState->GameplayAbility);
		if (!bCanActivate)
		{
			return false;
		}
	}

	PlayerCharacter->ResetGravityScale();
	CurrentState = NextState;
	

	if (UCS_PlayerCombatState* CombatState = Cast<UCS_PlayerCombatState>(NextState))
	{
		PlayerCharacter->CurrentDamageAttribute = CombatState->DamageAttributes;
		PlayerCharacter->CurrentDamageVisuals = CombatState->DamageFeedback;
		PlayerCharacter->SetGameplayState(CombatState->StateInfo.bCanMove, CombatState->StateInfo.bIsPerformingAction, CombatState->StateInfo.bCanRotate, false);

		if (CombatState->StateInfo.GravityScale > -1)
		{
			PlayerCharacter->GetCharacterMovement()->StopMovementImmediately();
			PlayerCharacter->GetCharacterMovement()->GravityScale = CombatState->StateInfo.GravityScale;
		}
		else
		{
			PlayerCharacter->ResetGravityScale();
		}

		CurrentStateTransitionSet = CombatState->SortedTransitionStates;
	}
	else
	{
		PlayerCharacter->CurrentDamageAttribute = FDamageAttributes();
		PlayerCharacter->CurrentDamageVisuals = FDamageFeedback();

		if (NextState == CombatProfile)
		{
			if (UCS_PlayerCombatProfile* PlayerCombatProfile = Cast<UCS_PlayerCombatProfile>(CombatProfile))
			{
				CurrentStateTransitionSet = PlayerCombatProfile->SortedTransitionStates;
			}
		}
	}
	
	PlayerCharacter->ActorsHit.Empty();
	return true;
}

void UCS_PlayerStateMachineManager::ResetToDefaultState()
{
	SwitchState(CombatProfile);
}

void UCS_PlayerStateMachineManager::RetrieveStates(UCS_CombatStateBase* State, TSet<UCS_CombatStateBase*>& StateList)
{
	Super::RetrieveStates(State, StateList);

	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner());
	if (UCS_PlayerCombatProfile* PlayerCombatProfile = Cast<UCS_PlayerCombatProfile>(State))
	{
		for (UCS_CombatStateBase* AnyState : PlayerCombatProfile->AnyStateTransitions)
		{
			StateList.Add(AnyState);
			ASC->GiveAbility(AnyState->GameplayAbility);
		}
	}
}


void UCS_PlayerStateMachineManager::SwitchCombatProfile(UCS_CombatProfile* NewCombatProfile,
	TSet<UCS_CombatStateBase*>& StateList)
{
	RemoveStartupAbilities();
	CombatStateList.Empty();
	CurrentStateTransitionSet.Empty();

	if (!NewCombatProfile) return;
	
	CombatProfile = DuplicateObject<UCS_CombatProfile>(NewCombatProfile, this,  FName(*(NewCombatProfile->GetName() + TEXT("_Copy"))));
	
	if (UCS_PlayerCombatProfile* PlayerCombatProfile = Cast<UCS_PlayerCombatProfile>(CombatProfile))
	{
		PlayerCombatProfile->SortStates();
	}
	
	RetrieveStates(CombatProfile, CombatStateList);

	if (CombatProfile)
	{
		GiveStartupAbilities(CombatProfile);
	}
	
	SwitchState(CombatProfile);
}

void UCS_PlayerStateMachineManager::CheckForStateTransitions()
{
	if (CurrentStateTransitionSet.Num() == 0) return;

	// Store pointer to array because CurrentStateTransitionSet will get changed if SwitchState is true
	TArray<UCS_CombatStateBase*> TransitionsToCheck = CurrentStateTransitionSet;

	for (UCS_CombatStateBase* State : TransitionsToCheck)   
	{
		if (!CheckIfCanTransitionState(State) || !SwitchState(State)) continue;

		// On successful state switch
		UCS_BlueprintLibrary::SetLooseGameplayTagAsBool(PlayerCharacter, CSTags::Status::GameplayStates::PlayerStates::InputWindowActive, false);

		if (UCS_PlayerCombatState* CombatState = Cast<UCS_PlayerCombatState>(State))
		{
			PlayerCharacter->GetCSPlayerController()->ConsumeInput(CombatState->RequiredInputs);
		}

		return;
	}
}

bool UCS_PlayerStateMachineManager::CheckIfCanTransitionState(UCS_CombatStateBase* State)
{
	if (!IsValid(PlayerCharacter) || !IsValid(PlayerCharacter->GetCSPlayerState())) return false;
	
	UCS_PlayerCombatState* CombatState = Cast<UCS_PlayerCombatState>(State);
	if (!CombatState) return false;
	
	if (PlayerCharacter->HasMatchingGameplayTag(CSTags::Status::GameplayStates::IsPerformingAction) && !PlayerCharacter->HasMatchingGameplayTag(CSTags::Status::GameplayStates::PlayerStates::InputWindowActive))
	{
		return false;
	}

	if (!PlayerCharacter->GetCSPlayerState()->bIsLockedOn && CombatState->bRequireLockOn)
	{
		return false;
	}

	if (!(PlayerCharacter->GetCharacterMovement()->IsFalling() || PlayerCharacter->GetCharacterMovement()->IsFlying()) != CombatState->Grounded)
	{
		return false;
	}

	// INPUT SHOULD BE CHECKED LAST TO AVOID CONSUMPTION UPON FAILING
	if (!PlayerCharacter->GetCSPlayerController()->CheckInputCriteria(CombatState))
	{
		return false;
	}
	
	return true;
}

void UCS_PlayerStateMachineManager::ChangePlayerWeaponCombatProfile(ACS_WeaponBase* NewWeapon)
{
	SwitchCombatProfile(NewWeapon->CombatProfile, CombatStateList);
}

