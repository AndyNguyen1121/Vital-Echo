// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/NotifyStates/ANS_WarpActorLocation.h"

#include "MotionWarpingComponent.h"
#include "RootMotionModifier.h"
#include "Capstone/DebugMacros.h"
#include "Characters/CS_BaseCharacter.h"
#include "Utils/CS_BlueprintLibrary.h"

void UANS_WarpActorLocation::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                         float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!IsValid(MeshComp)) return;

	ACS_BaseCharacter* Character = Cast<ACS_BaseCharacter>(MeshComp->GetOwner());
	if (!IsValid(Character)) return;

	UMotionWarpingComponent* MotionWarpingComponent = Character->GetMotionWarpingComponent();
	if (!MotionWarpingComponent) return;
	

	if (const FMotionWarpingTarget* Target = MotionWarpingComponent->FindWarpTarget(WarpTarget))
	{
		Character->WarpLocationInfo.StartingLocation = Character->GetActorLocation();
		Character->WarpLocationInfo.StartingRotation = Character->GetActorRotation().Quaternion();
		Character->WarpLocationInfo.TargetLocation = Target->GetLocation();
		
		if (bMaintainZPosition) Character->WarpLocationInfo.TargetLocation.Z = Character->GetActorLocation().Z;
		
		Character->WarpLocationInfo.TargetRotation = Target->GetRotation();
		Character->WarpLocationInfo.Duration = TotalDuration;
		Character->WarpLocationInfo.TimeElapsed = 0;
		Character->WarpLocationInfo.WarpTarget = WarpTarget;

		UWorld* World = GEngine->GetWorldFromContextObject(Character, EGetWorldErrorMode::LogAndReturnNull);

		if (!World) return;

		DrawDebugSphere(World,  Target->GetLocation(), 5.f, 15.f, FColor::Yellow, false, 10);
	}
}

void UANS_WarpActorLocation::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (!IsValid(MeshComp)) return;

	ACS_BaseCharacter* Character = Cast<ACS_BaseCharacter>(MeshComp->GetOwner());
	if (!IsValid(Character)) return;

	UMotionWarpingComponent* MotionWarpingComponent = Character->GetMotionWarpingComponent();
	if (!MotionWarpingComponent) return;

	FWarpLocationInfo* CharacterWarpLocationInfo = &Character->WarpLocationInfo;

	CharacterWarpLocationInfo->TimeElapsed += FrameDeltaTime;
	FVector LerpedLocation =
		FMath::Lerp(
			CharacterWarpLocationInfo->StartingLocation,
			CharacterWarpLocationInfo->TargetLocation,
			CharacterWarpLocationInfo->TimeElapsed / CharacterWarpLocationInfo->Duration);

	FQuat SlerpedRotation =
		FQuat::Slerp(
			CharacterWarpLocationInfo->StartingRotation,
			CharacterWarpLocationInfo->TargetRotation,
			CharacterWarpLocationInfo->TimeElapsed / CharacterWarpLocationInfo->Duration);

	Character->SetActorLocationAndRotation(LerpedLocation, SlerpedRotation, true);

	DEBUG_ERR(FString::SanitizeFloat(CharacterWarpLocationInfo->TimeElapsed))
}
