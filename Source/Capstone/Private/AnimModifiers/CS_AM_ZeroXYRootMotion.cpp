// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimModifiers/CS_AM_ZeroXYRootMotion.h"

void UCS_AM_ZeroXYRootMotion::OnApply_Implementation(UAnimSequence* AnimationSequence)
{
	Super::OnApply_Implementation(AnimationSequence);

	IAnimationDataController& Controller = AnimationSequence->GetController();

	const bool bShouldTransact = false;

	Controller.OpenBracket(
		FText::FromString("ZeroXYRootMotion"),
		bShouldTransact
	);
	
	if (BoneName.IsNone())
	{
		BoneName =
			AnimationSequence->GetSkeleton()
			->GetReferenceSkeleton()
			.GetBoneName(0);
	}
	else
	{
		const bool bExists =
		AnimationSequence->GetSkeleton()
		->GetReferenceSkeleton()
		.FindBoneIndex(BoneName) != INDEX_NONE;

		if (!bExists)
		{
			return;
		}
	}

	
	const int32 NumKeys =
		AnimationSequence->GetDataModel()->GetNumberOfKeys();

	for (int32 Key = 0; Key < NumKeys; Key++)
	{
		const FInt32Range KeyRange(Key, Key + 1);

		// Get current transform from the data model
		const FTransform CurrentTransform =
			AnimationSequence->GetDataModel()
			->GetBoneTrackTransform(BoneName, Key);

		FVector Location = CurrentTransform.GetLocation();

		OriginalRootPositions.Add(Location);

		// Remove XY movement
		Location.X = 0.f;
		Location.Y = -1000;

		FTransform NewTransform = CurrentTransform;
		NewTransform.SetLocation(Location);

		Controller.UpdateBoneTrackKeys(
			BoneName,
			KeyRange,
			{ NewTransform.GetLocation() },
			{ NewTransform.GetRotation() },
			{ NewTransform.GetScale3D() },
			bShouldTransact
		);
	}

	Controller.CloseBracket(bShouldTransact);
}

void UCS_AM_ZeroXYRootMotion::OnRevert_Implementation(UAnimSequence* AnimationSequence)
{
	Super::OnRevert_Implementation(AnimationSequence);

	IAnimationDataController& Controller = AnimationSequence->GetController();

	const bool bShouldTransact = false;

	Controller.OpenBracket(
		FText::FromString("ZeroXYRootMotion"),
		bShouldTransact
	);

	if (BoneName.IsNone())
	{
		BoneName =
			AnimationSequence->GetSkeleton()
			->GetReferenceSkeleton()
			.GetBoneName(0);
	}
	else
	{
		const bool bExists =
		AnimationSequence->GetSkeleton()
		->GetReferenceSkeleton()
		.FindBoneIndex(BoneName) != INDEX_NONE;

		if (!bExists)
		{
			return;
		}
	}

	const int32 NumKeys =
		AnimationSequence->GetDataModel()->GetNumberOfKeys();

	if (OriginalRootPositions.Num() != NumKeys)
	{
		return;
	}

	for (int32 Key = 0; Key < NumKeys; Key++)
	{
		const FInt32Range KeyRange(Key, Key + 1);

		// Get current transform from the data model
		const FTransform CurrentTransform =
			AnimationSequence->GetDataModel()
			->GetBoneTrackTransform(BoneName, Key);
		
		FTransform NewTransform = CurrentTransform;
		NewTransform.SetLocation(OriginalRootPositions[Key]);

		Controller.UpdateBoneTrackKeys(
			BoneName,
			KeyRange,
			{ NewTransform.GetLocation() },
			{ NewTransform.GetRotation() },
			{ NewTransform.GetScale3D() },
			bShouldTransact
		);
	}

	Controller.CloseBracket(bShouldTransact);
}
