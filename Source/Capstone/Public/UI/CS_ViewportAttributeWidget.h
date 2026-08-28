// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CS_AttributeWidget.h"
#include "CS_ViewportAttributeWidget.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONE_API UCS_ViewportAttributeWidget : public UCS_AttributeWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
};
