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
	if (AActor* Actor = FindLockOnTarget(InputDirection))
	{
		CachedPlayerCharacter->LockOnReticle->AttachToComponent(Actor->GetRootComponent(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false));
		CachedPlayerCharacter->LockOnReticle->SetVisibility(true);
	}
}

AActor* UCS_SoftLockOnComponent::FindLockOnTarget(FVector2D InputDirection)
{
	FVector CameraForwardDir = CachedPlayerCharacter->GetCSCameraManager()->GetActorForwardVector();
	FVector CameraRightDir = CachedPlayerCharacter->GetCSCameraManager()->GetActorRightVector();
	CameraForwardDir.Z = 0;
	CameraForwardDir.Normalize();

	FVector InputDirectionRelativeToCamera = (CameraForwardDir * InputDirection.Y) + (CameraRightDir * InputDirection.X);
	
	FVector StartLocation =
		CachedPlayerCharacter->GetActorLocation() +
			(InputDirectionRelativeToCamera * TraceOffset.X) +
				(InputDirectionRelativeToCamera.RightVector * TraceOffset.Y) + (FVector::UpVector * TraceOffset.Z);

	FVector EndLocation = StartLocation + (InputDirectionRelativeToCamera * TraceDistance);
	
	//////////////////////// Set Up Collisions ///////////////////////////////

	TArray<FHitResult> Hits;
	FCollisionShape Shape = FCollisionShape::MakeSphere(TraceRadius);
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());

	FCollisionResponseParams ResponseParams;
	ResponseParams.CollisionResponse.SetAllChannels(ECR_Ignore);
	ResponseParams.CollisionResponse.SetResponse(ECC_Pawn, ECR_Block);
	///////////////////////////////////////////////////////////////////////////
	
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

	return Hits.Num() > 0 ? Hits[0].GetActor() : nullptr;
}



