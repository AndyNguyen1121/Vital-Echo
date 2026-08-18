// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/NotifyStates/ANS_DragHitActors.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Capstone/DebugMacros.h"
#include "Characters/CS_BaseCharacter.h"
#include "Characters/CS_PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayTags/CSTags.h"
#include "Utils/CS_BlueprintLibrary.h"

void UANS_DragHitActors::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!IsValid(MeshComp)) return;

	ACS_BaseCharacter* Character = Cast<ACS_BaseCharacter>(MeshComp->GetOwner());
	if (!IsValid(Character)) return;

	CachedCharacter = Character;

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	if (!AnimInstance) return;
	
	UAnimMontage* Montage = Cast<UAnimMontage>(Animation);
	TWeakObjectPtr<ACS_BaseCharacter> WeakCharacter = Character;

	BindMontageEnd(WeakCharacter.Get(), AnimInstance, Montage, [this, WeakCharacter](UAnimMontage*, bool)
	{
		if (WeakCharacter.IsValid())
		{
			for (AActor* Actor : WeakCharacter->ActorsHit)
			{
				DEBUG_ERR("BITCH")
				if (ACharacter* EnemyCharacter = Cast<ACharacter>(Actor))
				{
					EnemyCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
					
				}
			}
		}
	});
} 

void UANS_DragHitActors::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                    float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (!IsValid(MeshComp)) return;

	ACS_BaseCharacter* Character = Cast<ACS_BaseCharacter>(MeshComp->GetOwner());
	if (!IsValid(Character)) return;

	for (AActor* Actor : Character->ActorsHit)
	{
		if (ACS_BaseCharacter* EnemyCharacter = Cast<ACS_BaseCharacter>(Actor))
		{
			EnemyCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
			FVector Velocity = Character->GetVelocity();

			FVector ModifiedVelocity =
				Character->GetActorForwardVector() * (FVector::DotProduct(Velocity, Character->GetActorForwardVector()) * MovementMultiplier.X) +
				Character->GetActorRightVector() * (FVector::DotProduct(Velocity, Character->GetActorRightVector()) * MovementMultiplier.Y) +
				Character->GetActorUpVector() * (FVector::DotProduct(Velocity, Character->GetActorUpVector()) * MovementMultiplier.Z);
			
			EnemyCharacter->AddActorWorldOffset(ModifiedVelocity * FrameDeltaTime, true);
		}
	}
}

void UANS_DragHitActors::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!IsValid(MeshComp)) return;

	ACS_BaseCharacter* Character = Cast<ACS_BaseCharacter>(MeshComp->GetOwner());
	if (!IsValid(Character)) return;

	for (AActor* Actor : Character->ActorsHit)
	{
		if (ACS_BaseCharacter* EnemyCharacter = Cast<ACS_BaseCharacter>(Actor))
		{
			FVector DesiredLaunchVelocity =
				(Character->GetActorForwardVector() * EndLaunchVelocity.X) +
					(Character->GetActorRightVector() * EndLaunchVelocity.Y) +
						(Character->GetActorUpVector() * EndLaunchVelocity.Z);

			UCS_BlueprintLibrary::KnockUpActor(EnemyCharacter, Character, DesiredLaunchVelocity);
			
			EnemyCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
		}
	}
}