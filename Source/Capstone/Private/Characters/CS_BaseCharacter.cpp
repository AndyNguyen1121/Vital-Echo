// Fill out your copyright notice in the Description page of Project Settings.


#include "Capstone/Public/Characters/CS_BaseCharacter.h"

#include "MotionWarpingComponent.h"
#include "AbilitySystem/CS_AttributeSet.h"
#include "Capstone/Public/Player/CS_PlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ACS_BaseCharacter::ACS_BaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>("MotionWarpingComponent");
}

void ACS_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitialGravityScale = GetCharacterMovement()->GravityScale;
	bInitialGravitySet = true;
	
	InitializeAttributes();
}

UAbilitySystemComponent* ACS_BaseCharacter::GetAbilitySystemComponent() const
{
	return nullptr;
}


void ACS_BaseCharacter::ResetGravityScale()
{
	if (!bInitialGravitySet) return;
	
	GetCharacterMovement()->GravityScale = InitialGravityScale;
}

void ACS_BaseCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if (!GetAbilitySystemComponent()) return;

	TagContainer = GetAbilitySystemComponent()->GetOwnedGameplayTags();
}

void ACS_BaseCharacter::InitializeAttributes()
{
	if (!AttributeInitializationEffect) return;

	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(AttributeInitializationEffect, 1.f, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

