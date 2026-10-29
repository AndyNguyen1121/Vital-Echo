// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CS_EnemyCharacter.h"
#include "CS_Parry_Enemy.generated.h"

UCLASS()
class CAPSTONE_API ACS_Parry_Enemy : public ACS_EnemyCharacter
{
	GENERATED_BODY()

public:
	ACS_Parry_Enemy();
	virtual void Tick(float DeltaTime) override;

public:
	

protected:
	virtual void BeginPlay() override;

};
	
