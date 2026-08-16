#pragma once
#include "CoreMinimal.h"           
#include "Enums/InputTypes.h"     
#include "FBufferedInput.generated.h"

USTRUCT(BlueprintType)
struct FFBufferedInput
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EInputType Input;

	UPROPERTY(EditAnywhere)
	int FramesElapsedSincePressed;

	UPROPERTY(EditAnywhere)
	int FramesHeld;

	UPROPERTY(EditAnywhere)
	bool bIsBeingHeld;

	UPROPERTY(EditAnywhere)
	bool bIsConsumed;

	FFBufferedInput()
	{
		Input = EInputType::EIT_Neutral;
		FramesElapsedSincePressed = 0;
		FramesHeld = 0;
		bIsBeingHeld = false;
		bIsConsumed = false;
	}

	FFBufferedInput(EInputType InputType)
	{
		Input = InputType;
		FramesElapsedSincePressed = 0;
		FramesHeld = 0;
		bIsBeingHeld = false;
		bIsConsumed = false;

	}
};