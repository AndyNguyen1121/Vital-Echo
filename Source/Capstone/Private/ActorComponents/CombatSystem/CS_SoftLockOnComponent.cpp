// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/CombatSystem/CS_SoftLockOnComponent.h"

#include "KismetTraceUtils.h"
#include "Camera/CS_PlayerCameraManager.h"
#include "Capstone/DebugMacros.h"
#include "Characters/CS_PlayerCharacter.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/CS_PlayerController.h"
#include "Player/CS_PlayerState.h"
#include "Utils/CS_BlueprintLibrary.h"

// Called when the game starts
void UCS_SoftLockOnComponent::BeginPlay()
{
	Super::BeginPlay();

	CachedPlayerCharacter = Cast<ACS_PlayerCharacter>(GetOwner());

	if (!CachedPlayerCharacter.IsValid())
	{
		DEBUG_ERR("SoftLockOnComponent: No Character found");
		return;
	}

	if (CachedPlayerController.IsValid())
	{
		CachedPlayerController = CachedPlayerCharacter->GetCSPlayerController();
		CachedPlayerController->OnInputDirectionChanged.AddDynamic(this, &ThisClass::TrySetLockOnTarget);
	}
}

void UCS_SoftLockOnComponent::TrySetLockOnTarget(FVector2D InputDirection)
{
	ACS_BaseCharacter* TargetCharacter = nullptr;
	if (!CachedPlayerCharacter->GetCSPlayerState()->bIsHardLockedOn &&!InputDirection.IsNearlyZero())
	{
		 TargetCharacter = Cast<ACS_BaseCharacter>(FindLockOnTarget(InputDirection));
	}

	if (IsValid(TargetCharacter) && !TargetCharacter->IsDead())
	{
		CachedPlayerCharacter->SetLockOnTarget(TargetCharacter);
	}
}

AActor* UCS_SoftLockOnComponent::FindLockOnTarget(FVector2D InputDirection)
{
	FVector CameraForwardDir = CachedPlayerCharacter->GetCSCameraManager()->GetActorForwardVector();
	FVector CameraRightDir = CachedPlayerCharacter->GetCSCameraManager()->GetActorRightVector();
	CameraForwardDir.Z = 0;
	CameraForwardDir.Normalize();

	FVector InputDirectionRelativeToCamera = (CameraForwardDir * InputDirection.Y) + (CameraRightDir * InputDirection.X);
	InputDirectionRelativeToCamera.Normalize();
	
	FVector StartLocation =
		CachedPlayerCharacter->GetActorLocation() +
			(InputDirectionRelativeToCamera * TraceOffset.X) +
				(InputDirectionRelativeToCamera.RightVector * TraceOffset.Y) + (FVector::UpVector * TraceOffset.Z);

	FVector EndLocation = StartLocation + (InputDirectionRelativeToCamera * TraceDistance);
	
	// Set up collision detection
	TArray<FHitResult> Hits;
	FCollisionShape Shape = FCollisionShape::MakeSphere(TraceRadius);
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());

	FCollisionResponseParams ResponseParams;
	ResponseParams.CollisionResponse.SetAllChannels(ECR_Ignore);
	ResponseParams.CollisionResponse.SetResponse(ECC_Pawn, ECR_Block);

	// Execute trace
	const bool bHit = GetWorld()->SweepMultiByChannel(Hits, StartLocation, EndLocation, FQuat::Identity, ECC_Visibility, Shape, QueryParams, ResponseParams);

#if WITH_EDITOR

	if (bShowDebug)
	{
		DrawDebugSphereTraceMulti(
			GetWorld(),
			StartLocation,
			EndLocation,
			TraceRadius,
			EDrawDebugTrace::ForOneFrame,
			bHit,
			Hits,
			FColor::Green,
			FColor::Red,
			1);
	}

#endif

	if (Hits.IsEmpty()) return nullptr;
	
	return PickBestTarget(Hits, CachedPlayerCharacter->GetLockOnTarget());
}

float UCS_SoftLockOnComponent::ScoreTarget(AActor* Candidate, FVector2D ScreenCenter, FVector PlayerLoc)
{
	FVector2D ScreenPos;
	CachedPlayerController->ProjectWorldLocationToScreen(Candidate->GetActorLocation(), ScreenPos);

	const float ScreenDist = FVector2D::Distance(ScreenPos, ScreenCenter);
	const float WorldDist  = FVector::Dist(Candidate->GetActorLocation(), PlayerLoc);
	
	return ScreenDist * 1.0f + WorldDist * 0.15f;
}

AActor* UCS_SoftLockOnComponent::PickBestTarget(const TArray<FHitResult>& Candidates, AActor* CurrentTarget)
{
	AActor* Best = nullptr;
	float BestScore = TNumericLimits<float>::Max();

	int32 ViewportX = 0, ViewportY = 0;
	CachedPlayerController->GetViewportSize(ViewportX, ViewportY);
	FVector2D ScreenCenter(ViewportX * 0.5f, ViewportY * 0.5f);

	for (FHitResult Candidate : Candidates)
	{
		float Score = ScoreTarget(Candidate.GetActor(), ScreenCenter, CachedPlayerCharacter->GetActorLocation());

		if (Candidate.GetActor() == CurrentTarget)
		{
			Score *= 0.7f;
		}

		if (Score < BestScore)
		{
			BestScore = Score;
			Best = Candidate.GetActor();
		}
	}
	return Best;
}



