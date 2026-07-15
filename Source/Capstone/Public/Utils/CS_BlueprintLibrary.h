// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
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

	UFUNCTION(BlueprintCallable)
	static TArray<AActor*> SphereOverlapTest(AActor* AvatarActor, float Radius, FVector Offset = FVector::ZeroVector,
	                                         FGameplayTagContainer TagsToFilter = FGameplayTagContainer(), bool DrawDebug = false, FLinearColor DebugColor =
		                                         FLinearColor::Red);

	static void DrawSphereOverlapDebug(UObject* ContextObject, const TArray<AActor*>& Actors, const FVector& HitBoxLocation, const float HitBoxRadius, FLinearColor
	                                   DebugColor);

	static void FilterGameplayTags(const FGameplayTagContainer& TagsToCheck, UPARAM(ref) TArray<AActor*>& ActorsToFilter);
};
