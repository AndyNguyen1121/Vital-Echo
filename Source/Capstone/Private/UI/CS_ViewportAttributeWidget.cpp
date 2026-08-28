// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CS_ViewportAttributeWidget.h"

#include "Characters/CS_BaseCharacter.h"
#include "Kismet/GameplayStatics.h"

void UCS_ViewportAttributeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	if (PlayerController)
	{
		if (ACS_BaseCharacter* Character = Cast<ACS_BaseCharacter>(PlayerController->GetCharacter()))
		{
			if (Character->GetAbilitySystemComponent() && Character->GetAttributeSet())
			{
				BindWidget(Character->GetAbilitySystemComponent(), Character->GetAttributeSet());
			}
		}
	}
}
