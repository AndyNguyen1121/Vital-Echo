// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "CS_CheatManager.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONE_API UCS_CheatManager : public UCheatManager
{
	GENERATED_BODY()

	virtual void InitCheatManager() override;

public:
	UFUNCTION(Exec)
	void ToggleDebugHitBoxes();

	UFUNCTION(Exec)
	void UnlimitedDash();
};
