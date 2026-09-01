// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Engine/SkeletalMeshSocket.h"
#include "ANS_ActivateAttackWindow.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONE_API UANS_ActivateAttackWindow : public UAnimNotifyState
{
	GENERATED_BODY()

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	UFUNCTION()
	TArray<FString> GetBoneNames()
	{
		TArray<FString> BoneNames;
		
		if (Skeleton)
		{
			const FReferenceSkeleton& ReferenceSkeleton = Skeleton->GetRefSkeleton();
			
			for (int32 i = 0; i < ReferenceSkeleton.GetNum(); ++i)
			{
				BoneNames.Add(ReferenceSkeleton.GetBoneName(i).ToString());
			}

			const TArray<USkeletalMeshSocket*> Sockets = Skeleton->GetActiveSocketList();

			for (auto Socket : Sockets)
			{
				BoneNames.Add(Socket->SocketName.ToString());
			}
		}
		
		return BoneNames;
	}


public:
	UPROPERTY(EditAnywhere, Transient)
	USkeletalMesh* Skeleton;

	UPROPERTY(EditAnywhere, meta=(GetOptions="GetBoneNames"))
	FName SocketName;

	UPROPERTY(EditAnywhere)
	float TraceRadius{30.f};

	UPROPERTY(EditAnywhere)
	float TraceHalfHeight{90.f};

	UPROPERTY(EditAnywhere)
	FVector PositionOffset = FVector(0, 0, 100);

	UPROPERTY(EditAnywhere)
	FRotator RotationOffset;

	UPROPERTY(EditAnywhere)
	bool bOverrideKnockback;

	UPROPERTY(EditAnywhere, meta=(EditCondition = "bOverrideKnockback == true"))
	FVector NewKnockbackForce;

	UPROPERTY(EditAnywhere, meta=(EditCondition = "bOverrideKnockback == true"))
	bool bUseSourceActorRotation{false};

	UPROPERTY(EditAnywhere)
	bool bShowDebugInPreview = true;
	
	inline static bool bDebug = false;
};
