// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../CS_BaseCharacter.h"
#include "AbilitySystem/CS_AbilitySystemComponent.h"
#include "AbilitySystem/AttributeSets/CS_AttributeSet.h"
#include "CS_EnemyCharacter.generated.h"

class UCS_AttributeSet;
class UCS_EnemyStateMachineManager;
class UCS_AbilitySystemComponent;

UCLASS()
class CAPSTONE_API ACS_EnemyCharacter : public ACS_BaseCharacter
{
	GENERATED_BODY()

public:
	ACS_EnemyCharacter();
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; };

	UFUNCTION(BlueprintPure)
	UCS_EnemyStateMachineManager* GetEnemyStateMachineManager() { return EnemyStateMachineManager; }

	virtual UCS_AttributeSet* GetAttributeSet() const override { return AttributeSet; };

	void CheckConsecutiveHitCooldown();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="CustomProperties")
	float MinRetreatDistance;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="CustomProperties")
	float MaxRetreatDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CustomProperties")
	bool bIsAggroOnStart;

	// Number of times player hit enemy and is successfully blocked
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Capstone")
	int32 ConsecutiveHits;

	// How many hits to activate parry
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Capstone")
	int32 ConsecutiveHitThreshold;

	// How long to wait before resetting ConsecutiveHits
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Capstone")
	int32 ConsecutiveHitCooldown;

	// Time elapsed since the player has hit enemy
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float TimeSinceLastConsecutiveAttack;
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCS_AbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCS_AttributeSet> AttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UCS_EnemyStateMachineManager> EnemyStateMachineManager;

protected:
	void UpdateMoveAmount();

	virtual void DestroyCharacter() override;
};
