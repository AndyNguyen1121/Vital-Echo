#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "DamageAttributes.generated.h"

class UGameplayEffect;

USTRUCT(Blueprintable, BlueprintType)
struct FDamageAttributes
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector KnockBackForce = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
	float PostureDamage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
	TSubclassOf<UGameplayEffect> DamageEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer IDsToIgnore;
};


USTRUCT(Blueprintable, BlueprintType)
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