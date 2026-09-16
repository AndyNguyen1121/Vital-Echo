// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CS_EnemyCharacter.h"

#include "RootMotionModifier.h"
#include "AbilitySystem/CS_AbilitySystemComponent.h"
#include "Capstone/DebugMacros.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayTags/CSTags.h"
#include "MotionWarpingComponent.h"
#include "ActorComponents/StateMachine/CS_EnemyStateMachineManager.h"
#include "Utils/CS_BlueprintLibrary.h"


// Sets default values
ACS_EnemyCharacter::ACS_EnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	AbilitySystemComponent = CreateDefaultSubobject<UCS_AbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->AddLooseGameplayTag(CSTags::IDs::Enemy);
	EnemyStateMachineManager = CreateDefaultSubobject<UCS_EnemyStateMachineManager>("EnemyStateMachineManager");
	AttributeSet = CreateDefaultSubobject<UCS_AttributeSet>("AttributeSet");
}

void ACS_EnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

void ACS_EnemyCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateMoveAmount();
}


void ACS_EnemyCharacter::UpdateMoveAmount()
{
	MoveAmount = ((GetVelocity().Size()) / GetCharacterMovement()->GetMaxSpeed());
}



