// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CS_WeaponBase.h"
#include "Enums/WeaponTypes.h"
#include "CS_TrickWeapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponTransformType : uint8
{
	EWTS_None,
	EWTS_Primary,
	EWTS_Secondary
};


UCLASS()
class CAPSTONE_API ACS_TrickWeapon : public ACS_WeaponBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void PlayWeaponTransformAnimation(EWeaponTransformType TransformType, AActor* ParentActor);
	
	UFUNCTION(BlueprintPure)
	UAnimMontage* GetPrimaryWeaponAnimation() { return PrimaryWeaponAnimation; }

	UFUNCTION(BlueprintPure)
	UAnimMontage* GetSecondaryWeaponAnimation() { return SecondaryWeaponAnimation; }

	
private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> PrimaryWeaponAnimation;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> SecondaryWeaponAnimation;

	UPROPERTY(EditDefaultsOnly)
	EWeaponType SecondaryWeaponType;
	
};
