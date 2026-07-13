// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CS_BaseCharacter.h"
#include "AbilitySystem/CS_AbilitySystemComponent.h"
#include "CS_EnemyCharacter.generated.h"

class UCS_AbilitySystemComponent;

UCLASS()
class CAPSTONE_API ACS_EnemyCharacter : public ACS_BaseCharacter
{
	GENERATED_BODY()

public:
	ACS_EnemyCharacter();
	virtual void BeginPlay() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; };
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCS_AbilitySystemComponent> AbilitySystemComponent;
};
