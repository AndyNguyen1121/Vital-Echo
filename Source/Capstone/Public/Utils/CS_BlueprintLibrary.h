// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CS_BlueprintLibrary.generated.h"

class ULevelSequence;
/**
 * 
 */
UCLASS()
class CAPSTONE_API UCS_BlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static void PlayLevelSequenceWithOriginTransform(AActor* ContextActor, ULevelSequence* LevelSequence, FTransform OriginTransform);
};
