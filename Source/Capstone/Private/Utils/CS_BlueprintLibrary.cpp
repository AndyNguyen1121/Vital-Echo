// Fill out your copyright notice in the Description page of Project Settings.

#include "Utils/CS_BlueprintLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "ContextualAnimUtilities.h"
#include "DefaultLevelSequenceInstanceData.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlayer.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"
#include "ActorComponents/CombatSystem/CS_HitStopComponent.h"
#include "Characters/CS_BaseCharacter.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayTags/CSTags.h"
#include "Interfaces/HitStopComponentInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/CS_SequenceStopCallback.h"

void UCS_BlueprintLibrary::PlayFinisherWithOriginTransform(AActor* ContextActor, ULevelSequence* LevelSequence,
                                                           FTransform OriginTransform, AActor* SecondaryActor)
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

	UCS_SequenceStopCallback* Callback = NewObject<UCS_SequenceStopCallback>(SequenceActor);
	Callback->Initialize(ContextActor, SecondaryActor);

	OnSequenceStarted(ContextActor, SecondaryActor);
	SequencePlayer->OnStop.AddDynamic(Callback, &UCS_SequenceStopCallback::OnSequenceStopped);
	SequencePlayer->Play();
}

TArray<AActor*> UCS_BlueprintLibrary::SphereOverlapTest(AActor* AvatarActor, float Radius, FVector Offset,
                                                        FGameplayTagContainer TagsToSearch, EFilterType FilterMode, bool DrawDebug, FLinearColor DebugColor)
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

	FilterGameplayTags(TagsToSearch, ActorsHit, FilterMode);

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
                                              TArray<AActor*>& ActorsToFilter, EFilterType FilterMode)
{
	if (TagsToCheck.IsEmpty() || TagsToCheck.HasAllExact(FGameplayTagContainer(CSTags::None))) return;

	ActorsToFilter.RemoveAll([&TagsToCheck, FilterMode](AActor* Actor)
	{
		bool RemoveActor = true;

		// Check tag container first
		if (IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>(Actor))
		{
			if  (TagInterface->HasAnyMatchingGameplayTags(TagsToCheck) && FilterMode == EFilterType::Keep ||
				 !TagInterface->HasAnyMatchingGameplayTags(TagsToCheck) && FilterMode == EFilterType::Remove)
			{
				RemoveActor = false;
			}
		}

		// Check ACS if not found in tag container
		if (RemoveActor)
		{
			UAbilitySystemComponent* ACS = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor);
			if (ACS && (ACS->HasAnyMatchingGameplayTags(TagsToCheck) && FilterMode == EFilterType::Keep ||
				 !ACS->HasAnyMatchingGameplayTags(TagsToCheck) && FilterMode == EFilterType::Remove))
			{
				RemoveActor = false;
			}
		}
		
		return RemoveActor; 
	});
}

EHitDirection UCS_BlueprintLibrary::GetHitDirection(const FVector& TargetForward, const FVector& ToInstigator)
{
	const float Dot = FVector::DotProduct(TargetForward, ToInstigator);
	if (Dot < -0.5f)
	{
		return EHitDirection::Back;
	}

	if (Dot < 0.5f)
	{
		const FVector Cross = FVector::CrossProduct(TargetForward, ToInstigator);

		if (Cross.Z < 0)
		{
			return EHitDirection::Left;
		}

		return EHitDirection::Right;
	}

	return EHitDirection::Forward;
}

void UCS_BlueprintLibrary::SetLooseGameplayTagsCount(AActor* Actor, FGameplayTagContainer Tags, int32 Count)
{
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor);
	if (!ASC) return;
	
	for (const FGameplayTag& Tag : Tags)
	{
		ASC->SetLooseGameplayTagCount(Tag, Count);
	}
}

void UCS_BlueprintLibrary::KnockUpActor(AActor* TargetActor, AActor* SourceActor, FVector Velocity, bool UseSourceRotation)
{
	if (!IsValid(TargetActor) || !IsValid(SourceActor)) return;
	if (Velocity.IsNearlyZero()) return;

	ACharacter* TargetCharacter = Cast<ACharacter>(TargetActor);
	if (!IsValid(TargetCharacter)) return;

	FGameplayEventData Payload;
	Payload.Instigator = SourceActor;
	Payload.Target = TargetActor;

	FVector DirectionToInstigator = SourceActor->GetActorLocation() - TargetActor->GetActorLocation();
	DirectionToInstigator.Z = 0;
	DirectionToInstigator.Normalize();
	
	FVector DesiredVelocity;

	if (UseSourceRotation)
	{
		DesiredVelocity = SourceActor->GetActorForwardVector() * Velocity.X;
		DesiredVelocity += SourceActor->GetActorRightVector() * Velocity.Y;
		DesiredVelocity += SourceActor->GetActorUpVector() * Velocity.Z;
	}
	else
	{
		DesiredVelocity = DirectionToInstigator * Velocity.X;
		DesiredVelocity += (FVector::CrossProduct(FVector::UpVector, DirectionToInstigator) * Velocity.Y);
		DesiredVelocity.Z = Velocity.Z;
	}
	
	if (Velocity.Z > 0)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetActor, FGameplayTag(CSTags::Events::Gameplay::KnockedUp), Payload);
	}

	TargetCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	TargetCharacter->GetCharacterMovement()->StopMovementImmediately();
	TargetCharacter->GetCharacterMovement()->AddImpulse(DesiredVelocity, true);
}

AActor* UCS_BlueprintLibrary::GetClosestActorToLocation(const TArray<AActor*> Actors, FVector Location)
{
	AActor* ClosestActor = nullptr;
	float ClosestDistanceSq = FLT_MAX;

	for (AActor* Actor : Actors)
	{
		float DistanceSq = FVector::DistSquared(Location, Actor->GetActorLocation());

		if (DistanceSq < ClosestDistanceSq)
		{
			ClosestActor = Actor;
			ClosestDistanceSq = DistanceSq;
		}
	}

	return ClosestActor;
}

void UCS_BlueprintLibrary::PlayContextualAnimation(
	UContextualAnimSceneActorComponent* ContextualAnimSceneActorComponent, AActor* PrimaryActor, AActor* SecondaryActor,
	UContextualAnimSceneAsset* SceneAsset)
{
	if (!IsValid(ContextualAnimSceneActorComponent) || !IsValid(PrimaryActor) || !IsValid(SecondaryActor) || !IsValid(SceneAsset))
	{
		return;
	}

	FContextualAnimSceneBindingContext Primary =
	 UContextualAnimUtilities::BP_SceneBindingContext_MakeFromActor(PrimaryActor);

	FContextualAnimSceneBindingContext Secondary =
		UContextualAnimUtilities::BP_SceneBindingContext_MakeFromActor(SecondaryActor);

	FContextualAnimSceneBindings OutBindings;
	
	if (UContextualAnimUtilities::BP_CreateContextualAnimSceneBindingsForTwoActors(SceneAsset, Primary, Secondary, OutBindings))
	{
		ContextualAnimSceneActorComponent->StartContextualAnimScene(OutBindings);
	}
}

void UCS_BlueprintLibrary::ApplyHitStun(AActor* TargetActor, float Duration, float TimeDilation)
{
	if (!IsValid(TargetActor) || !TargetActor->Implements<UHitStopComponentInterface>()) return;

	UCS_HitStopComponent* HitStopComponent = IHitStopComponentInterface::Execute_GetHitStopComponent(TargetActor);

	HitStopComponent->BeginHitStop(Duration, TimeDilation);
}

void UCS_BlueprintLibrary::ApplyDamageAttributes(AActor* TargetActor, AActor* SourceActor,
                                                 FDamageAttributes DamageAttributes, bool UseSourceActorRotationForKnockback)
{
	KnockUpActor(TargetActor, SourceActor, DamageAttributes.KnockBackForce, UseSourceActorRotationForKnockback);

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (ASC && DamageAttributes.DamageEffect)
	{
		FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
		ContextHandle.AddInstigator(SourceActor, SourceActor);
		FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(DamageAttributes.DamageEffect, 1.f, ContextHandle);

		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(Spec, CSTags::Effects::SetByCaller::Damage, DamageAttributes.Damage);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(Spec, CSTags::Effects::SetByCaller::PostureDamage, DamageAttributes.PostureDamage);
		ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data);
	}
	
}

void UCS_BlueprintLibrary::ApplyDamageFeedback(ACharacter* TargetCharacter, ACharacter* SourceCharacter,
	FDamageFeedback DamageFeedback)
{
	ApplyHitStun(TargetCharacter, DamageFeedback.HitStunDuration, DamageFeedback.HitStunTimeDilation);
	ApplyHitStun(SourceCharacter, DamageFeedback.HitStunDuration, DamageFeedback.HitStunTimeDilation);

	if (!UGameplayStatics::GetPlayerController(TargetCharacter, 0)) return;

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(TargetCharacter, 0);
	PlayerController->ClientStartCameraShake(DamageFeedback.CameraShakeClass, DamageFeedback.CameraShakeScale);
	PlayerController->ClientPlayForceFeedback(DamageFeedback.VibrationEffect);
}

void UCS_BlueprintLibrary::SetLooseGameplayTagAsBool(AActor* Actor, FGameplayTag GameplayTag, bool Active)
{
	if (!IsValid(Actor)) return;

	if (Active)
	{
		SetLooseGameplayTagsCount(Actor, FGameplayTagContainer(GameplayTag), 1);
	}
	else 
	{
		UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(Actor, FGameplayTagContainer(GameplayTag));
	}
}

void UCS_BlueprintLibrary::TurnToFaceActor(AActor* FromActor, AActor* ToActor)
{
	if (!IsValid(FromActor) || !IsValid(ToActor)) return;

	FVector DirToActor = ToActor->GetActorLocation() - FromActor->GetActorLocation();
	DirToActor.Z = 0;
	DirToActor.Normalize();

	FromActor->SetActorRotation(DirToActor.Rotation());
}

void UCS_BlueprintLibrary::OnSequenceStarted(AActor* PrimaryActor, AActor* SecondaryActor)
{
	SetLooseGameplayTagAsBool(PrimaryActor, CSTags::Status::GameplayStates::IsInScriptedAnimation, true);
	SetLooseGameplayTagAsBool(SecondaryActor, CSTags::Status::GameplayStates::IsInScriptedAnimation, true);
}




