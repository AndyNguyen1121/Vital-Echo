// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CS_EnemyCharacter.h"

#include "AbilitySystem/CS_AbilitySystemComponent.h"
#include "Capstone/DebugMacros.h"
#include "GameplayTags/CSTags.h"


// Sets default values
ACS_EnemyCharacter::ACS_EnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GameplayTags.AddTag(CSTags::IDs::Enemy);
	AbilitySystemComponent = CreateDefaultSubobject<UCS_AbilitySystemComponent>("AbilitySystemComponent");
}

void ACS_EnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}



