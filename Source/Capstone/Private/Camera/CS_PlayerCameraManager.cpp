// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CS_PlayerCameraManager.h"

#include "Camera/CameraComponent.h"
#include "Capstone/DebugMacros.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameplayTags/CSTags.h"
#include "Utils/CS_BlueprintLibrary.h"

ACS_PlayerCameraManager::ACS_PlayerCameraManager()
{
	
}

void ACS_PlayerCameraManager::BeginPlay()
{
	Super::BeginPlay();
}

AActor* ACS_PlayerCameraManager::EnableLockOn(float SearchRadius, float MaxAngle)
{
	AActor* TargetActor = FindValidLockOnTarget(SearchRadius, MaxAngle);
	return TargetActor;
}

void ACS_PlayerCameraManager::DisableLockOn()
{
}

void ACS_PlayerCameraManager::SwitchLockOnTargets(const FVector2D ControllerInput)
{
}

AActor* ACS_PlayerCameraManager::FindValidLockOnTarget(float SearchRadius, float MaxAngle, FLinearColor DebugColor)
{
	AActor* CachedOwnerPawn = GetOwningPlayerController()->GetPawn();
	if (!IsValid(CachedOwnerPawn)) return nullptr;
	
	TArray<AActor*> ValidActors = UCS_BlueprintLibrary::SphereOverlapTest(
		GetOwningPlayerController()->GetPawn(),
		SearchRadius,
		FVector::ZeroVector,
		FGameplayTagContainer(CSTags::IDs::Enemy),
		bShowLockOnDebug,
		FColor::Purple);
	
	ValidActors.RemoveAll([MaxAngle, this](AActor* Actor)
	{
		FVector DirectionToEnemy = (Actor->GetActorLocation() - GetCameraLocation()).GetSafeNormal();
		float Dot = GetActorForwardVector().Dot(DirectionToEnemy); 
		float Angle = FMath::RadiansToDegrees(FMath::Acos(Dot));

		return Angle > MaxAngle;
	});

	if (ValidActors.Num() == 0) return nullptr;
	
	AActor* ClosestActor = ValidActors[0];
	float ClosestDistance = CachedOwnerPawn->GetDistanceTo(ClosestActor);
	for (int32 i = 1; i < ValidActors.Num(); ++i)
	{
		float DistanceTo = ValidActors[i]->GetDistanceTo(CachedOwnerPawn) < ClosestDistance;
		if (DistanceTo < ClosestDistance)
		{
			ClosestDistance = DistanceTo;
			ClosestActor = ValidActors[i];
		}
	}

	if (bShowLockOnDebug)
	{
		DrawDebugSphere(GetWorld(), ClosestActor->GetActorLocation() + (ClosestActor->GetActorUpVector() * 100), 10.f, 16, DebugColor.ToFColor(true), false, 0.5f);
	}
	
	return ClosestActor;
}
