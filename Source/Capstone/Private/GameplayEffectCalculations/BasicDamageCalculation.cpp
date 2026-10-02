// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayEffectCalculations/BasicDamageCalculation.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/CS_AttributeSet.h"
#include "GameplayTags/CSTags.h"

void UBasicDamageCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                                     FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	FGameplayEffectAttributeCaptureDefinition DamageDef = FGameplayEffectAttributeCaptureDefinition(
		UCS_AttributeSet::GetDamageAttribute(),
		EGameplayEffectAttributeCaptureSource::Source,
		true
	);

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();

	float DamageOut = Spec.GetSetByCallerMagnitude(CSTags::Effects::SetByCaller::Damage);
	float PostureDamageOut = Spec.GetSetByCallerMagnitude(CSTags::Effects::SetByCaller::PostureDamage);

	FGameplayEventData Payload;
	Payload.Target = TargetASC->GetAvatarActor();
	Payload.Instigator = SourceASC->GetAvatarActor();
	
	if (TargetASC->HasAnyMatchingGameplayTags(FGameplayTagContainer(CSTags::Status::GameplayStates::PlayerStates::ParryWindowActive)))
	{
		DamageOut = 0.f;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetASC->GetAvatarActor(), CSTags::Events::Gameplay::ParryActivate, Payload);
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(SourceASC->GetAvatarActor(), CSTags::Events::Gameplay::ParryReceive, Payload);
	}

	if (TargetASC->HasAnyMatchingGameplayTags(FGameplayTagContainer(CSTags::Status::GameplayStates::IsBlocking)))
	{
		PostureDamageOut *= 0.5f;
		DamageOut *= 0.5f;
	}
	
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UCS_AttributeSet::GetDamageAttribute(), EGameplayModOp::Override, DamageOut));
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UCS_AttributeSet::GetPostureDamageAttribute(), EGameplayModOp::Override, PostureDamageOut));
}
