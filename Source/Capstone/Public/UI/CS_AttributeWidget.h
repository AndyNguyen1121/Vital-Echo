// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Blueprint/UserWidget.h"
#include "CS_AttributeWidget.generated.h"

class UCS_AbilitySystemComponent;
class ACS_BaseCharacter;
class UCS_AttributeSet;
/**
 * 
 */
UCLASS()
class CAPSTONE_API UCS_AttributeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Crash|Attributes")
	FGameplayAttribute Attribute;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Crash|Attributes")
	FGameplayAttribute MaxAttribute;

	void BindWidget(UAbilitySystemComponent* ASC, UCS_AttributeSet* AttributeSet);
	void OnAttributeChange(UCS_AttributeSet* AttributeSet, float OldValue);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Attribute Changed"))
	void BP_OnAttributeChange(float NewValue, float NewMaxValue,  float OldValue);

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<AActor> AvatarActor;
};
