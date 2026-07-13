// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CS_BaseCharacter.h"
#include "CS_EnemyCharacter.generated.h"

UCLASS()
class CAPSTONE_API ACS_EnemyCharacter : public ACS_BaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACS_EnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
