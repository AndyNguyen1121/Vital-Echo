// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayEffectCalculations/TestCalculation.h"

#include "AbilitySystem/CS_AttributeSet.h"

void UTestCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	FGameplayEffectAttributeCaptureDefinition DamageDef = FGameplayEffectAttributeCaptureDefinition(
		UCS_AttributeSet::GetDamageAttribute(),
		EGameplayEffectAttributeCaptureSource::Source,
		true
	);

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	float DamageOut;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageDef, EvaluateParameters, DamageOut);

	DamageOut += 50;

	if (DamageOut > 0)
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UCS_AttributeSet::GetDamageAttribute(), EGameplayModOp::Additive, DamageOut));
}
