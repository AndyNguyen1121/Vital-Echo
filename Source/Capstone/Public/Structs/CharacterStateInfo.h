#pragma once

#pragma once
#include "CoreMinimal.h"
#include "CharacterStateInfo.generated.h"

USTRUCT(Blueprintable)
struct FCharacterStateInfo
{
	GENERATED_BODY()

	/*UPROPERTY(EditAnywhere)
	bool bIsPerformingAction = true;

	UPROPERTY(EditAnywhere)
	bool bCanMove = false;

	UPROPERTY(EditAnywhere)
	bool bCanRotate = false;*/

	UPROPERTY(EditAnywhere)
	float GravityScale = -1;
};