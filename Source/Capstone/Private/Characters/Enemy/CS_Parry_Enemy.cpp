// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/CS_Parry_Enemy.h"


// Sets default values
ACS_Parry_Enemy::ACS_Parry_Enemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConsecutiveHitThreshold = 2;
	ConsecutiveHitCooldown = 1.f;
}

// Called when the game starts or when spawned
void ACS_Parry_Enemy::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACS_Parry_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


