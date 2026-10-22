// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CS_AttributeSet.h"
#include "CS_Player_AttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONE_API UCS_Player_AttributeSet : public UCS_AttributeSet
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData HeatGauge;

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData HeatGaugeLimit;

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData HeatGaugeRecoverRate;

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData HeatGaugeSuppressionDuration;
	
	ATTRIBUTE_ACCESSORS(ThisClass, HeatGauge);
	ATTRIBUTE_ACCESSORS(ThisClass, HeatGaugeLimit);
	ATTRIBUTE_ACCESSORS(ThisClass, HeatGaugeRecoverRate);
	ATTRIBUTE_ACCESSORS(ThisClass, HeatGaugeSuppressionDuration);

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
};
