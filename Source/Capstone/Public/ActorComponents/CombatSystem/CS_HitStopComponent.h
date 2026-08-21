// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CS_HitStopComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable, BlueprintType)
class CAPSTONE_API UCS_HitStopComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BeginHitStop(float Duration, float TimeDilation);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void EndHitStop();
};
