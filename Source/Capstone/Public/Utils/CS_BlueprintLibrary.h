// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ContextualAnimSceneActorComponent.h"
#include "ContextualAnimSceneAsset.h"
#include "Structs/DamageAttributes.h"
#include "CS_BlueprintLibrary.generated.h"

class ULevelSequence;
/**
 * 
 */

UENUM(BlueprintType)
enum class EHitDirection : uint8
{
	Left,
	Right,
	Back,
	Forward
};

UENUM(BlueprintType)
enum class EFilterType : uint8
{
	Remove,
	Keep
};

UCLASS()
class CAPSTONE_API UCS_BlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static void PlayFinisherWithOriginTransform(AActor* ContextActor, ULevelSequence* LevelSequence, FTransform OriginTransform, AActor* SecondaryActor = nullptr);

	UFUNCTION(BlueprintCallable)
	static TArray<AActor*> SphereOverlapTest(AActor* AvatarActor, float Radius, FVector Offset = FVector::ZeroVector,
	                                         FGameplayTagContainer TagsToSearch = FGameplayTagContainer(), EFilterType FilterMode = EFilterType::Keep, bool DrawDebug = false, FLinearColor DebugColor =
		                                         FLinearColor::Red);

	static void DrawSphereOverlapDebug(UObject* ContextObject, const TArray<AActor*>& Actors, const FVector& HitBoxLocation, const float HitBoxRadius, FLinearColor
	                                   DebugColor);

	static void FilterGameplayTags(const FGameplayTagContainer& TagsToCheck, UPARAM(ref) TArray<AActor*>& ActorsToFilter, EFilterType FilterMode);

	UFUNCTION(BlueprintCallable)
	static EHitDirection GetHitDirection(const FVector& TargetForward, const FVector& ToInstigator);

	UFUNCTION(BlueprintCallable)
	static void SetLooseGameplayTagsCount(AActor* Actor, FGameplayTagContainer Tags, int32 Count = 1);

	UFUNCTION(BlueprintCallable)
	static void KnockUpActor(AActor* TargetActor, AActor* SourceActor, FVector Velocity, bool UseSourceRotation);

	UFUNCTION(BlueprintPure)
	static AActor* GetClosestActorToLocation(const TArray<AActor*> Actors, FVector Location);

	UFUNCTION(BlueprintCallable)
	static void PlayContextualAnimation(UContextualAnimSceneActorComponent* ContextualAnimSceneActorComponent, AActor* PrimaryActor, AActor* SecondaryActor, UContextualAnimSceneAsset* SceneAsset);
	
	UFUNCTION(BlueprintCallable)
	static void ApplyHitStun(AActor* TargetActor, float Duration, float TimeDilation);

	UFUNCTION(BlueprintCallable)
	static void ApplyDamageAttributes(AActor* TargetActor, AActor* SourceActor, FDamageAttributes DamageAttributes, bool UseSourceActorRotationForKnockback = false);

	UFUNCTION(BlueprintCallable)
	static void ApplyDamageFeedback(ACharacter* TargetCharacter, ACharacter* SourceCharacter, FDamageFeedback DamageFeedback);

	UFUNCTION(BlueprintCallable)
	static void SetLooseGameplayTagAsBool(AActor* Actor, FGameplayTag GameplayTag, bool Active);

	UFUNCTION(BlueprintCallable)
	static void TurnToFaceActor(AActor* FromActor, AActor* ToActor);

private:
	static void OnSequenceStarted(AActor* PrimaryActor, AActor* SecondaryActor);
};
