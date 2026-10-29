// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "CS_AbilitySystemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CAPSTONE_API UCS_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Abilities")
	bool CanActivateAbilityByClass(TSubclassOf<UGameplayAbility> AbilityClass) const;

protected:
	virtual void OnGiveAbility(FGameplayAbilitySpec& AbilitySpec) override;

private:
	void HandleAutoActivatedAbilities(const FGameplayAbilitySpec& AbilitySpec);

	TSet<FGameplayAbilitySpecHandle> AutoActivatedAbilities;
};
