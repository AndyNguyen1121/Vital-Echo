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
	
	float Time = 0.f;
	int SampleIndex = 0;
	int SampleRate = 30;
	const float SampleInterval = 1.f / static_cast<float>(SampleRate);
	float AnimLength = AnimationSequence->GetPlayLength();
	
	for (int32 Key = 0; Key < NumKeys; Key++)
	{
		Time = FMath::Clamp(static_cast<float>(SampleIndex) * SampleInterval, 0.f, AnimLength);
		
		const FInt32Range KeyRange(Key, Key + 1);

		// Get current transform from the data model
		const FTransform CurrentTransform =
			AnimationSequence->GetDataModel()
			->GetBoneTrackTransform(BoneName, Key);

		FVector Location = CurrentTransform.GetLocation();
		OriginalTransformKeys.Add(CurrentTransform);
		OriginalRootTimes.Add(Time);
		SampleIndex++;

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

	FString BoneComponentX = BoneName.ToString() + TEXT("_X");
	FString BoneComponentY = BoneName.ToString() + TEXT("_Y");
	FString BoneComponentZ = BoneName.ToString() + TEXT("_Z");
	
	const FAnimationCurveIdentifier CurveIdX(FName(BoneComponentX), ERawCurveTrackTypes::RCT_Float);
	const FAnimationCurveIdentifier CurveIdY(FName(BoneComponentY), ERawCurveTrackTypes::RCT_Float);
	const FAnimationCurveIdentifier CurveIdZ(FName(BoneComponentZ), ERawCurveTrackTypes::RCT_Float);
	
	Controller.AddCurve(CurveIdX, AACF_Metadata);
	Controller.AddCurve(CurveIdY, AACF_Metadata);
	Controller.AddCurve(CurveIdZ, AACF_Metadata);
	
	TArray<FRichCurveKey> KeysX, KeysY, KeysZ;

	for (int32 i = 0; i < NumKeys; ++i)
	{
		float OriginalTime = OriginalRootTimes[i];
		FVector Location = OriginalTransformKeys[i].GetLocation();
		
		FRichCurveKey XPoint (OriginalTime, Location.X);
		FRichCurveKey YPoint (OriginalTime, Location.Y);
		FRichCurveKey ZPoint (OriginalTime, Location.Z);
		
		KeysX.Add(XPoint);
		KeysY.Add(YPoint);
		KeysZ.Add(ZPoint);
	}
	
	Controller.SetCurveKeys(CurveIdX, KeysX);
	Controller.SetCurveKeys(CurveIdY, KeysY);
	Controller.SetCurveKeys(CurveIdZ, KeysZ);
	
	Controller.CloseBracket(bShouldTransact);
	
	MarkPackageDirty();
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

	const IAnimationDataModel* DataModel = AnimationSequence->GetDataModel();
	
	const TArray<FFloatCurve> Curves = DataModel->GetFloatCurves();

	FFloatCurve CurveX, CurveY, CurveZ;

	for (FFloatCurve Curve : Curves)
	{
		FString Name = Curve.GetName().ToString();

		if (Name.Len() >= 2)
		{
			FString LastTwoCharacters = Name.Right(2);

			if (LastTwoCharacters == TEXT("_X"))
			{
				CurveX = Curve;
			}
			else if (LastTwoCharacters == TEXT("_Y"))
			{
				CurveY = Curve;
			}
			else if (LastTwoCharacters == TEXT("_Z"))
			{
				CurveZ = Curve;
			}
			
		}
	}


	float Time = 0.f;
	int SampleIndex = 0;
	int SampleRate = 30;
	const float SampleInterval = 1.f / static_cast<float>(SampleRate);
	float AnimLength = AnimationSequence->GetPlayLength();

	TArray<FQuat4f> RotKeys; RotKeys.Init(FQuat4f::Identity, NumKeys);
	TArray<FVector3f> ScaleKeys; ScaleKeys.Init(FVector3f::OneVector, NumKeys);

	for (int32 Key = 0; Key < NumKeys; Key++)
	{
		const FInt32Range KeyRange(Key, Key + 1);
		Time = FMath::Clamp(static_cast<float>(SampleIndex) * SampleInterval, 0.f, AnimLength);
		FVector Location(CurveX.Evaluate(Time), CurveY.Evaluate(Time), CurveZ.Evaluate(Time));

		Controller.UpdateBoneTrackKeys(
			BoneName,
			KeyRange,
			{ FVector3f(Location) },
			{ RotKeys[Key] },
			{ ScaleKeys[Key] },
			bShouldTransact
		);
		
		SampleIndex++;
	}

	for (const FFloatCurve& Curve : Curves)
	{
		FAnimationCurveIdentifier CurveId(
			Curve.GetName(),
			ERawCurveTrackTypes::RCT_Float
		);

		Controller.RemoveCurve(CurveId);
	}
	
	Controller.CloseBracket(bShouldTransact);
	MarkPackageDirty();
}

