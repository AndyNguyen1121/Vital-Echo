#pragma once
#include "CoreMinimal.h"
#include "DamageAttributes.generated.h"

USTRUCT(Blueprintable)
struct FDamageAttributes
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FVector KnockBackForce = FVector::ZeroVector;
};
