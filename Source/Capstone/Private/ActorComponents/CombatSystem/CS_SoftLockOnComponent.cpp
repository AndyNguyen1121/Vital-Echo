// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/CombatSystem/CS_SoftLockOnComponent.h"

#include "KismetTraceUtils.h"
#include "Camera/CS_PlayerCameraManager.h"
#include "Capstone/DebugMacros.h"
#include "Characters/CS_PlayerCharacter.h"
#include "Components/WidgetComponent.h"
#include "Player/CS_PlayerController.h"
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

	CachedPlayerController = CachedPlayerCharacter->GetCSPlayerController();
	CachedPlayerController->OnInputDirectionChanged.AddDynamic(this, &ThisClass::TrySetLockOnTarget);
}

void UCS_SoftLockOnComponent::TrySetLockOnTarget(FVector2D InputDirection)
{
	ACharacter* TargetCharacter = nullptr;
	if (!InputDirection.IsNearlyZero())
	{
		 TargetCharacter = Cast<ACharacter>(FindLockOnTarget(InputDirection));
	}

	if (IsValid(TargetCharacter))
	{
		CachedPlayerCharacter->SetLockOnTarget(TargetCharacter);
		CachedPlayerCharacter->LockOnReticle->AttachToComponent(TargetCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false), FName("LockOnTarget"));
		CachedPlayerCharacter->LockOnReticle->SetVisibility(true);
	}
	else
	{
		CachedPlayerCharacter->SetLockOnTarget(nullptr);
		CachedPlayerCharacter->LockOnReticle->SetVisibility(false);
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
	
	AActor* ClosestActor = Hits[0].GetActor();
	float ClosestDistance = FLT_MAX;

	for (FHitResult HitResult : Hits)
	{
		float Distance = ClosestActor->GetDistanceTo(HitResult.GetActor());
		if (ClosestActor->GetDistanceTo(HitResult.GetActor()) < ClosestDistance)
		{
			ClosestActor = HitResult.GetActor();
			ClosestDistance = Distance;
		}
	}

	return ClosestActor;
}



