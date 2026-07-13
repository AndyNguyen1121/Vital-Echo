// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "CS_BaseCharacter.generated.h"

UCLASS()
class CAPSTONE_API ACS_BaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACS_BaseCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

public:
	UPROPERTY(VisibleAnywhere)
	TSet<AActor*> ActorsHit;

};
