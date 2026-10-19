// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CS_AttributeWidget.h"

#include "AbilitySystem/AttributeSets/CS_AttributeSet.h"

void UCS_AttributeWidget::OnAttributeChange(UCS_AttributeSet* AttributeSet, float OldValue)
{
	const float AttributeValue = Attribute.GetNumericValue(AttributeSet);
	const float MaxAttributeValue = MaxAttribute.GetNumericValue(AttributeSet);

	BP_OnAttributeChange(AttributeValue, MaxAttributeValue, OldValue);
}

void UCS_AttributeWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCS_AttributeWidget::BindWidget(UAbilitySystemComponent* ASC, UCS_AttributeSet* AttributeSet)
{
	ASC->GetGameplayAttributeValueChangeDelegate(Attribute).AddLambda([this, ASC, AttributeSet](const FOnAttributeChangeData& AttributeChangeData)
	{
		OnAttributeChange(AttributeSet, AttributeChangeData.OldValue);
	});

	OnAttributeChange(AttributeSet, 0);
}
