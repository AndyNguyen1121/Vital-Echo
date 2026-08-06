// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/Notify/AN_WarpToGround.h"

#include "MotionWarpingComponent.h"
#include "Capstone/DebugMacros.h"
#include "Characters/CS_BaseCharacter.h"
#include "Kismet/KismetMathLibrary.h"

void UAN_WarpToGround::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                              const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!IsValid(MeshComp))
		return;

	ACS_BaseCharacter* Character = Cast<ACS_BaseCharacter>(MeshComp->GetOwner());

	if (!IsValid(Character))
		return;

	UWorld* World = GEngine->GetWorldFromContextObject(Character, EGetWorldErrorMode::LogAndReturnNull);

	if (!World)
		return;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Character);

	FCollisionResponseParams ResponseParams;
	ResponseParams.CollisionResponse.SetAllChannels(ECR_Ignore);
	ResponseParams.CollisionResponse.SetResponse(ECC_WorldStatic, ECR_Block);
	ResponseParams.CollisionResponse.SetResponse(ECC_WorldDynamic, ECR_Block);

	FVector EndLocation = Character->GetActorLocation() + (-Character->GetActorUpVector() * TraceDistance);
	
	FHitResult HitResult;
	World->LineTraceSingleByChannel(
		HitResult,
		Character->GetActorLocation(),
		EndLocation,
		ECC_Visibility,
		Params,
		ResponseParams);

	DrawDebugLine(
	  World,
	  Character->GetActorLocation(),
	  EndLocation,
	  HitResult.GetActor() ? FColor::Green : FColor::Red,
	  false,
	  5.0f,
	  0,
	  1.5f);

	if (HitResult.GetActor())
	{
		Character->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(FName("Ground"), HitResult.ImpactPoint);
		DEBUG_ERR(HitResult.GetActor()->GetName())
		
	}
}
