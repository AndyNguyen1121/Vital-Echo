// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CS_AttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)\

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttributeInitialized);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPostureIncreased);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDamaged, float, OldValue, float, NewValue, AActor*, Instigator);

UCLASS()
class CAPSTONE_API UCS_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	void PostAttributesInitialized();
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	/** 
	 * Adds posture while safely clamping value.
	 */ 
	void AddPostureSafe(float Value);
	void AddPostureWithPayload(float Value, AActor* Instigator);
public:
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData Health;
	
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData Damage;

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData ShieldDamage;

	// Posture
	
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData Posture;

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData PostureThreshold;

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData PostureRecoverRate;

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData PostureDamage;

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData BrokenPostureRecoverRate;

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData BasePostureDmg;

	ATTRIBUTE_ACCESSORS(ThisClass, Health);
	ATTRIBUTE_ACCESSORS(ThisClass, MaxHealth);
	ATTRIBUTE_ACCESSORS(ThisClass, Damage);
	ATTRIBUTE_ACCESSORS(ThisClass, ShieldDamage);
	ATTRIBUTE_ACCESSORS(ThisClass, Posture);
	ATTRIBUTE_ACCESSORS(ThisClass, PostureThreshold);
	ATTRIBUTE_ACCESSORS(ThisClass, PostureDamage);
	ATTRIBUTE_ACCESSORS(ThisClass, PostureRecoverRate);
	ATTRIBUTE_ACCESSORS(ThisClass, BrokenPostureRecoverRate);

	// TODO: WILL PROBABLY REPLACE
	ATTRIBUTE_ACCESSORS(ThisClass, BasePostureDmg);

	UPROPERTY(VisibleAnywhere)
	bool bAttributesInitialized;

	UPROPERTY()
	FAttributeInitialized OnAttributesInitialized;

	UPROPERTY()
	FOnDamaged OnDamaged;
};
