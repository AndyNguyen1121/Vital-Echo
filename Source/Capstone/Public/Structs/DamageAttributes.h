#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "DamageAttributes.generated.h"

class UGameplayEffect;

USTRUCT(Blueprintable)
struct FDamageAttributes
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FVector KnockBackForce = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category="Damage")
	float Damage;
	
	UPROPERTY(EditAnywhere, Category="Damage")
	TSubclassOf<UGameplayEffect> DamageEffect;

	UPROPERTY(EditAnywhere)
	FGameplayTagContainer IDsToIgnore;
};


USTRUCT(Blueprintable)
struct FDamageFeedback
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="HitStun")
	float HitStunDuration = 0.05f;

	UPROPERTY(EditAnywhere, Category="HitStun")
	float HitStunTimeDilation = 0.25f;

	UPROPERTY(EditAnywhere, Category="CameraShake")
	TSubclassOf<UCameraShakeBase> CameraShakeClass;

	UPROPERTY(EditAnywhere, Category="CameraShake")
	float CameraShakeScale = 1.f;

	UPROPERTY(EditAnywhere, Category="Vibrations")
	UForceFeedbackEffect* VibrationEffect;
};