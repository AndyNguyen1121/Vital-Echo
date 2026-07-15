// Fill out your copyright notice in the Description page of Project Settings.\

#include "Utils/CS_BlueprintLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "DefaultLevelSequenceInstanceData.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlayer.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"
#include "Engine/OverlapResult.h"
#include "GameplayTags/CSTags.h"

void UCS_BlueprintLibrary::PlayLevelSequenceWithOriginTransform(AActor* ContextActor, ULevelSequence* LevelSequence,
                                                                FTransform OriginTransform)
{
	UWorld* World = GEngine->GetWorldFromContextObject(ContextActor, EGetWorldErrorMode::LogAndReturnNull);
	if (!World) return;
	
	ALevelSequenceActor* SequenceActor = nullptr;
	FMovieSceneSequencePlaybackSettings Settings;
	ULevelSequencePlayer* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(World, LevelSequence, Settings, SequenceActor);
	
	if (!IsValid(SequenceActor)) return;

	SequenceActor->bOverrideInstanceData = true;

	if (UDefaultLevelSequenceInstanceData* InstanceData = Cast<UDefaultLevelSequenceInstanceData>(SequenceActor->DefaultInstanceData))
	{
		InstanceData->TransformOrigin = OriginTransform;
	}

	SequencePlayer->Play();
}

TArray<AActor*> UCS_BlueprintLibrary::SphereOverlapTest(AActor* AvatarActor, float Radius, FVector Offset,
                                                        FGameplayTagContainer TagsToFilter, bool DrawDebug, FLinearColor DebugColor)
{
	UWorld* World = GEngine->GetWorldFromContextObject(AvatarActor, EGetWorldErrorMode::LogAndReturnNull);
	if (!World) return TArray<AActor*>();

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(AvatarActor);

	FCollisionResponseParams ResponseParams;
	ResponseParams.CollisionResponse.SetAllChannels(ECR_Ignore);
	ResponseParams.CollisionResponse.SetResponse(ECollisionChannel::ECC_Pawn, ECR_Block);

	FVector DesiredOffset = AvatarActor->GetActorLocation() +
		(AvatarActor->GetActorForwardVector() * Offset.X) +
			(AvatarActor->GetActorRightVector() * Offset.Y) +
				(AvatarActor->GetActorUpVector() * Offset.Z);
	
	World->OverlapMultiByChannel(OverlapResults, DesiredOffset, FQuat::Identity, ECollisionChannel::ECC_Visibility, FCollisionShape::MakeSphere(Radius), QueryParams, ResponseParams);

	TArray<AActor*> ActorsHit;
	for (const FOverlapResult& OverlapResult : OverlapResults)
	{
		if (IsValid(OverlapResult.GetActor()))
		{
			ActorsHit.AddUnique(OverlapResult.GetActor());
		}
	}

	FilterGameplayTags(TagsToFilter, ActorsHit);

	if (DrawDebug)
	{
		DrawSphereOverlapDebug(AvatarActor, ActorsHit, DesiredOffset, Radius, DebugColor);	
	}
	
	return ActorsHit;
}

void UCS_BlueprintLibrary::DrawSphereOverlapDebug(UObject* ContextObject, const TArray<AActor*>& Actors,
                                                  const FVector& HitBoxLocation, const float HitBoxRadius, FLinearColor DebugColor)
{
	UWorld* World = GEngine->GetWorldFromContextObject(ContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World) return;
	
	DrawDebugSphere(World, HitBoxLocation, HitBoxRadius, 16, DebugColor.ToFColor(true), false, 3.f);

	for (const AActor* Actor : Actors)
	{
		if (IsValid(Actor))
		{
			FVector DebugLocation = Actor->GetActorLocation();
			DrawDebugSphere(World, DebugLocation, 30.f, 10, FColor::Green, false, 3.f);
		}
	}
}

void UCS_BlueprintLibrary::FilterGameplayTags(const FGameplayTagContainer& TagsToCheck,
	TArray<AActor*>& ActorsToFilter)
{
	if (TagsToCheck.IsEmpty() || TagsToCheck.HasAllExact(FGameplayTagContainer(CSTags::None))) return;

	ActorsToFilter.RemoveAll([&TagsToCheck](AActor* Actor)
	{
		bool RemoveActor = true;

		// Check tag container first
		if (IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>(Actor))
		{
			if  (TagInterface->HasAnyMatchingGameplayTags(TagsToCheck))
			{
				RemoveActor = false;
			}
		}

		// Check ACS if not found in tag container
		if (RemoveActor)
		{
			UAbilitySystemComponent* ACS = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor);
			if (ACS && ACS->HasAnyMatchingGameplayTags(TagsToCheck))
			{
				RemoveActor = false;
			}
		}
		
		return RemoveActor; 
	});
}
