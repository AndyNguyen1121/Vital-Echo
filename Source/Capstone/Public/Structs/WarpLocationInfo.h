#pragma once

#include "CoreMinimal.h"
#include "WarpLocationInfo.generated.h"

USTRUCT(Blueprintable)
struct FWarpLocationInfo
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	FName WarpTarget;

	UPROPERTY(VisibleAnywhere)
	float TimeElapsed;

	UPROPERTY(VisibleAnywhere)
	float Duration;

	UPROPERTY(VisibleAnywhere)
	FVector StartingLocation;

	UPROPERTY(VisibleAnywhere)
	FQuat StartingRotation;

	UPROPERTY(VisibleAnywhere)
	FVector TargetLocation;

	UPROPERTY(VisibleAnywhere)
	FQuat TargetRotation;
};
