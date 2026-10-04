// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CS_PlayerCameraManager.h"

#include "Camera/CameraComponent.h"
#include "Camera/CS_CameraContainer.h"
#include "Capstone/DebugMacros.h"
#include "Characters/CS_PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameplayTags/CSTags.h"
#include "Player/CS_PlayerController.h"
#include "Player/CS_PlayerState.h"
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
	AActor* TargetActor = FindBestLockOnTarget(SearchRadius, MaxAngle);
	ACS_PlayerController* PlayerController = Cast<ACS_PlayerController>(GetOwningPlayerController());
	if (IsValid(PlayerController) && IsValid(TargetActor))
	{
		PlayerController->GetCSPlayerState()->bIsLockedOn = true;
		GetCameraContainer()->SetLockOnTarget(TargetActor);
		GetCameraContainer()->SwitchToLockOnCamera();
	}
		
	return TargetActor;
}

void ACS_PlayerCameraManager::DisableLockOn()
{
	ACS_PlayerController* PlayerController = Cast<ACS_PlayerController>(GetOwningPlayerController());
	if (IsValid(PlayerController))
	{
		PlayerController->GetCSPlayerState()->bIsLockedOn = false;
		GetCameraContainer()->SetLockOnTarget(nullptr);
		GetCameraContainer()->SwitchToMainCamera();
	}
}

ACS_CameraContainer* ACS_PlayerCameraManager::GetCameraContainer() const
{
	ACS_PlayerController* PlayerController = Cast<ACS_PlayerController>(GetOwningPlayerController());
	if (IsValid(PlayerController))
	{
		return PlayerController->GetCameraContainer();
	}

	return nullptr;
}

void ACS_PlayerCameraManager::SwitchLockOnTargets(const FVector2D ControllerInput, ACS_PlayerCharacter* PlayerCharacter)
{
	ACS_PlayerController* PlayerController = Cast<ACS_PlayerController>(GetOwningPlayerController());
	if (!IsValid(PlayerController) || !IsValid(PlayerCharacter)) return;
	if (!PlayerController->GetCSPlayerState()->bIsLockedOn || ControllerInput == FVector2D::ZeroVector) return;

	TArray<AActor*> ValidActors = FindValidLockOnTargets(PlayerCharacter->LockOnSearchRadius, PlayerCharacter->LockOnMaxAngle);
	if (ValidActors.Num() == 0) return;
	
	AActor* ClosestTarget = ValidActors[0];
	float ClosestDistance = GetDistanceTo(ClosestTarget);

	float Xinput = ControllerInput.X;
	float Yinput = ControllerInput.Y;
	
	for (AActor* Actor : ValidActors)
	{
		if (Actor == GetCameraContainer()->GetLockOnActor()) continue;

		FVector DirectionRelativeToCamera = (Actor->GetActorLocation() - GetCameraLocation()).GetSafeNormal();
		float Distance = GetDistanceTo(Actor);
		
		if (Distance < ClosestDistance)
		{
			if ((Xinput < 0 && GetCameraContainer()->LockOnCamera->GetActorRightVector().Dot(DirectionRelativeToCamera) < 0) || // Check Left OR
				(Xinput > 0 && GetCameraContainer()->LockOnCamera->GetActorRightVector().Dot(DirectionRelativeToCamera) > 0))   // Check Right depending on xInput
			{
				ClosestTarget = Actor;
				ClosestDistance = Distance;
			}
		}
	}

	if (IsValid(ClosestTarget))
	{
		if (ACS_PlayerCharacter* Player = Cast<ACS_PlayerCharacter>(PlayerController->GetPawn()))
		{
			Player->SetLockOnTarget(ClosestTarget);
		}
		GetCameraContainer()->SetLockOnTarget(ClosestTarget);
	}

}

AActor* ACS_PlayerCameraManager::FindBestLockOnTarget(float SearchRadius, float MaxAngle, FLinearColor DebugColor)
{
	TArray<AActor*> ValidActors = FindValidLockOnTargets(SearchRadius, MaxAngle);
	ACS_PlayerController* PlayerController = Cast<ACS_PlayerController>(GetOwningPlayerController());
	ACS_PlayerCharacter* Player = Cast<ACS_PlayerCharacter>(PlayerController->GetPawn());
	if (!IsValid(Player))
	{
		return nullptr;
	}
	
	AActor* Best = nullptr;
	float BestScore = TNumericLimits<float>::Max();

	int32 ViewportX = 0, ViewportY = 0;
	PlayerController->GetViewportSize(ViewportX, ViewportY);
	FVector2D ScreenCenter(ViewportX * 0.5f, ViewportY * 0.5f);

	for (AActor* Candidate : ValidActors)
	{
		float Score = ScoreTarget(Candidate, ScreenCenter, Player->GetActorLocation());

		if (Candidate == Player->GetLockOnTarget())
		{
			Score *= 0.7f;
		}

		if (Score < BestScore)
		{
			BestScore = Score;
			Best = Candidate;
		}
	}
	return Best;
}

TArray<AActor*> ACS_PlayerCameraManager::FindValidLockOnTargets(float SearchRadius, float MaxAngle,
	FLinearColor DebugColor)
{
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

		if (ACS_BaseCharacter* BaseCharacter = Cast<ACS_BaseCharacter>(Actor))
		{
			// Remove if dead
			if (BaseCharacter->IsDead())
			{
				return true;
			}
		}

		return Angle > MaxAngle;
	});

	return ValidActors;
	
}

float ACS_PlayerCameraManager::ScoreTarget(AActor* Candidate, FVector2D ScreenCenter, FVector PlayerLoc)
{
	FVector2D ScreenPos;
	GetOwningPlayerController()->ProjectWorldLocationToScreen(Candidate->GetActorLocation(), ScreenPos);

	const float ScreenDist = FVector2D::Distance(ScreenPos, ScreenCenter);
	const float WorldDist  = FVector::Dist(Candidate->GetActorLocation(), PlayerLoc);
	
	return ScreenDist * 1.0f + WorldDist * 0.15f;
}
