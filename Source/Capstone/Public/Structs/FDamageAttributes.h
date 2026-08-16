#pragma once
#include "CoreMinimal.h"
#include "FDamageAttributes.generated.h"

USTRUCT(Blueprintable)
struct FFDamageAttributes
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FVector KnockBackForce = FVector::ZeroVector;
};
