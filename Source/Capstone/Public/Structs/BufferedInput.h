#pragma once
#include "CoreMinimal.h"           
#include "Enums/InputTypes.h"     
#include "BufferedInput.generated.h"

USTRUCT(BlueprintType)
struct FBufferedInput
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

	FBufferedInput()
	{
		Input = EInputType::EIT_Neutral;
		FramesElapsedSincePressed = 0;
		FramesHeld = 0;
		bIsBeingHeld = false;
		bIsConsumed = false;
	}

	FBufferedInput(EInputType InputType)
	{
		Input = InputType;
		FramesElapsedSincePressed = 0;
		FramesHeld = 0;
		bIsBeingHeld = false;
		bIsConsumed = false;

	}
};