// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DetourCrowdAIController.h"
#include "CS_AIControllerBase.generated.h"

class ACS_EnemyCharacter;

UCLASS()
class CAPSTONE_API ACS_AIControllerBase : public ADetourCrowdAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACS_AIControllerBase();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void SetAggro(bool bIsAggro);

public:
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<ACS_EnemyCharacter> EnemyCharacter;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
};
