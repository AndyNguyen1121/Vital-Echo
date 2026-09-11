// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CS_SoftLockOnComponent.generated.h"


class ACS_PlayerController;
class ACS_PlayerCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CAPSTONE_API UCS_SoftLockOnComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	float TraceRadius;

	UPROPERTY(EditDefaultsOnly)
	float TraceDistance;

	UPROPERTY(EditDefaultsOnly)
	FVector TraceOffset;

	UPROPERTY(EditAnywhere)
	bool bShowDebug{false};
	
public:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void TrySetLockOnTarget(FVector2D InputDirection);

private:
	UPROPERTY()
	TWeakObjectPtr<ACS_PlayerCharacter> CachedPlayerCharacter;

	UPROPERTY()
	TWeakObjectPtr<ACS_PlayerController> CachedPlayerController;

private:
	UFUNCTION()
	AActor* FindLockOnTarget(FVector2D InputDirection);
};
