// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CS_AIControllerBase.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemy/CS_EnemyCharacter.h"


// Sets default values
ACS_AIControllerBase::ACS_AIControllerBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACS_AIControllerBase::BeginPlay()
{
	EnemyCharacter = Cast<ACS_EnemyCharacter>(GetPawn());
	if (!EnemyCharacter.IsValid()) return;

	if (!BehaviorTree) return;
	RunBehaviorTree(BehaviorTree);

	Super::BeginPlay();
}

void ACS_AIControllerBase::SetAggro(bool bIsAggro)
{
	if (!GetBlackboardComponent()) return;

	GetBlackboardComponent()->SetValueAsBool(FName("IsAggro"), bIsAggro);
}

