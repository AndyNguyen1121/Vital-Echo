// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CS_CombatStateBase.h"
#include "Engine/DataAsset.h"
#include "CS_CombatProfile.generated.h"

class UGameplayAbility;
/**
 * 
 */
UCLASS()
class CAPSTONE_API UCS_CombatProfile : public UCS_CombatStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UGameplayAbility>> StartUpAbilities;
};
