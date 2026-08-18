// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CS_StateMachineManagerBase.h"
#include "CS_EnemyStateMachineManager.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CAPSTONE_API UCS_EnemyStateMachineManager : public UCS_StateMachineManagerBase
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCS_EnemyStateMachineManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
