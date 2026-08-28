// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CS_WidgetComponent.h"

#include "AbilitySystem/CS_AbilitySystemComponent.h"
#include "AbilitySystem/CS_AttributeSet.h"
#include "Blueprint/WidgetTree.h"
#include "Capstone/DebugMacros.h"
#include "Characters/CS_BaseCharacter.h"
#include "UI/CS_AttributeWidget.h"

void UCS_WidgetComponent::BeginPlay()
{
	Super::BeginPlay();
	
	InitAbilitySystemData();

	if (!IsASCInitialized())
	{
		Character->OnASCInitialized.AddDynamic(this, &ThisClass::OnASCInitialized);
	}

	InitializeAttributeDelegate();
}

void UCS_WidgetComponent::InitAbilitySystemData()
{
	Character = Cast<ACS_BaseCharacter>(GetOwner());
    AttributeSet = Cast<UCS_AttributeSet>(Character->GetAttributeSet());
    AbilitySystemComponent = Cast<UCS_AbilitySystemComponent>(Character->GetAbilitySystemComponent());
}

bool UCS_WidgetComponent::IsASCInitialized() const
{
	return AbilitySystemComponent.IsValid() && AttributeSet.IsValid();
}

void UCS_WidgetComponent::InitializeAttributeDelegate()
{
	if (!AttributeSet.IsValid()) return;
	
	if (!AttributeSet->bAttributesInitialized)
	{
		AttributeSet->OnAttributesInitialized.AddDynamic(this, &ThisClass::BindToAttributeChanges);
	}
	else
	{
		BindToAttributeChanges();
	}
}

void UCS_WidgetComponent::BindWidgetToAttributeChanges(UWidget* WidgetObject)
{
	UCS_AttributeWidget* AttributeWidget = Cast<UCS_AttributeWidget>(WidgetObject);
	if (!IsValid(AttributeWidget)) return;
	
	AttributeWidget->AvatarActor = AbilitySystemComponent->GetAvatarActor();
	AttributeWidget->BindWidget(AbilitySystemComponent.Get(), AttributeSet.Get());
}

void UCS_WidgetComponent::OnASCInitialized(UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	AttributeSet = Cast<UCS_AttributeSet>(AS);
	AbilitySystemComponent = Cast<UCS_AbilitySystemComponent>(ASC);
	if (!IsASCInitialized()) return;
	InitializeAttributeDelegate();
}

void UCS_WidgetComponent::BindToAttributeChanges()
{
	BindWidgetToAttributeChanges(GetUserWidgetObject());
	GetUserWidgetObject()->WidgetTree->ForEachWidget([this](UWidget* ChildWidget)
	{
		BindWidgetToAttributeChanges(ChildWidget);
	});
}
