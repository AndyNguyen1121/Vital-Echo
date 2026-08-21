// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HitStopComponentInterface.generated.h"

class UCS_HitStopComponent;
// This class does not need to be modified.
UINTERFACE()
class UHitStopComponentInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CAPSTONE_API IHitStopComponentInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UCS_HitStopComponent* GetHitStopComponent();
	
};
