// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTags/CSTags.h"
#include "UObject/Object.h"
#include "Utils/CS_BlueprintLibrary.h"
#include "CS_SequenceStopCallback.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONE_API UCS_SequenceStopCallback : public UObject
{
	GENERATED_BODY()

public:

	void Initialize(AActor* InPrimaryActor, AActor* InSecondaryActor)
	{
		PrimaryActor = InPrimaryActor;
		SecondaryActor = InSecondaryActor;
	}

	UFUNCTION()
	void OnSequenceStopped()
	{
		UCS_BlueprintLibrary::SetLooseGameplayTagAsBool(
			PrimaryActor,
			CSTags::Status::GameplayStates::IsInScriptedAnimation,
			false
		);

		UCS_BlueprintLibrary::SetLooseGameplayTagAsBool(
			SecondaryActor,
			CSTags::Status::GameplayStates::IsInScriptedAnimation,
			false
		);
	}

private:

	UPROPERTY()
	AActor* PrimaryActor;

	UPROPERTY()
	AActor* SecondaryActor;
};
