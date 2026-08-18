// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TokenInterface.generated.h"

class UCS_TokenManager;
// This class does not need to be modified.
UINTERFACE()
class UTokenInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CAPSTONE_API ITokenInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UCS_TokenManager* GetTokenManager();
};
