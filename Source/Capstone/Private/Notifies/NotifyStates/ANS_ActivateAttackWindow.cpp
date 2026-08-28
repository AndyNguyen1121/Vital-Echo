// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/NotifyStates/ANS_ActivateAttackWindow.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "KismetTraceUtils.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Capstone/DebugMacros.h"
#include "Characters/CS_PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayTags/CSTags.h"
#include "Kismet/GameplayStatics.h"
#include "Utils/CS_BlueprintLibrary.h"


void UANS_ActivateAttackWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                            float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!IsValid(MeshComp)) return;

	if (ACS_BaseCharacter* SourceCharacter = Cast<ACS_BaseCharacter>(MeshComp->GetOwner()))
	{
		SourceCharacter->ActorsHit.Empty();
	}
}

void UANS_ActivateAttackWindow::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                           float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (!IsValid(MeshComp)) return;

	FTransform SocketTransform = MeshComp->GetSocketTransform(SocketName);

	FVector AdjustedOffset = SocketTransform.GetRotation().GetForwardVector() * PositionOffset.X;
	AdjustedOffset += SocketTransform.GetRotation().GetRightVector() * PositionOffset.Y;
	AdjustedOffset += SocketTransform.GetRotation().GetUpVector() * PositionOffset.Z;
	
	FVector StartLocation = SocketTransform.GetLocation() + AdjustedOffset;
	FVector EndLocation = StartLocation - (SocketTransform.GetRotation().GetForwardVector());
	FRotator DesiredRotation = SocketTransform.Rotator();
	DesiredRotation.Pitch += RotationOffset.Pitch;
	DesiredRotation.Yaw += RotationOffset.Yaw;
	DesiredRotation.Roll += RotationOffset.Roll;

	//////////////////////// Set Up Collisions ///////////////////////////////

	TArray<FHitResult> Hits;
	FCollisionShape Shape = FCollisionShape::MakeCapsule(TraceRadius, TraceHalfHeight);
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(MeshComp->GetOwner());

	FCollisionResponseParams ResponseParams;
	ResponseParams.CollisionResponse.SetAllChannels(ECR_Ignore);
	ResponseParams.CollisionResponse.SetResponse(ECC_Pawn, ECR_Block);
	UWorld* World = GEngine->GetWorldFromContextObject(MeshComp, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World)) return;

	///////////////////////////////////////////////////////////////////////////
	
	const bool bHit = World->SweepMultiByChannel(Hits, StartLocation, EndLocation, DesiredRotation.Quaternion(), ECC_Visibility, Shape, QueryParams, ResponseParams);
	
#if WITH_EDITOR

if (World && (bDebug || (World->WorldType == EWorldType::EditorPreview && bShowDebugInPreview)))
{
	DrawDebugCapsuleTraceMulti(
		World,
		StartLocation,
		EndLocation,
		TraceRadius,
		TraceHalfHeight,
		DesiredRotation,
		EDrawDebugTrace::ForDuration,
		bHit,
		Hits,         
		FColor::Green, 
		FColor::Red,  
		0.25f          
	);
}

#endif

	if (Hits.Num() > 0)
	{
		
		if (ACS_BaseCharacter* SourceCharacter = Cast<ACS_BaseCharacter>(MeshComp->GetOwner()))
		{
			FDamageAttributes CharacterDamageAttributes = SourceCharacter->CurrentDamageAttribute;
			FDamageFeedback CharacterDamageVisuals = SourceCharacter->CurrentDamageVisuals;
			
			for (const FHitResult& HitResult : Hits)
			{
				if (ACS_BaseCharacter* TargetCharacter = Cast<ACS_BaseCharacter>(HitResult.GetActor()))
				{
					if (TargetCharacter->HasAnyMatchingGameplayTags(CharacterDamageAttributes.IDsToIgnore)) continue;
					
					if (!SourceCharacter->ActorsHit.Contains(TargetCharacter))
					{
						SourceCharacter->ActorsHit.Add(TargetCharacter);
						FVector DesiredLaunchVelocity;

						if (!bOverrideKnockback)
						{
							DesiredLaunchVelocity =
							(SourceCharacter->GetActorForwardVector() * CharacterDamageAttributes.KnockBackForce.X) +
								(SourceCharacter->GetActorRightVector() * CharacterDamageAttributes.KnockBackForce.Y) +
									(SourceCharacter->GetActorUpVector() * CharacterDamageAttributes.KnockBackForce.Z);
						}
						else
						{
							DesiredLaunchVelocity =
							(SourceCharacter->GetActorForwardVector() * NewKnockbackForce.X) +
								(SourceCharacter->GetActorRightVector() * NewKnockbackForce.Y) +
									(SourceCharacter->GetActorUpVector() * NewKnockbackForce.Z);
						}

						CharacterDamageAttributes.KnockBackForce = DesiredLaunchVelocity;
						UCS_BlueprintLibrary::ApplyDamageAttributes(TargetCharacter, SourceCharacter, CharacterDamageAttributes);
						UCS_BlueprintLibrary::ApplyDamageFeedback(TargetCharacter, SourceCharacter, CharacterDamageVisuals);
					}
				}
			}
		}
	}
}
