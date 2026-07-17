// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CS_CameraActor.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class CAPSTONE_API ACS_CameraActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACS_CameraActor();

	virtual void BeginPlay() override;

	void InitializeCamera(APlayerController* PlayerController);

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<UChildActorComponent> MainCam;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> SceneComponent;

	UPROPERTY()
	TWeakObjectPtr<APlayerController> Controller;

	UPROPERTY()
	TWeakObjectPtr<APawn> PlayerPawn;

private:
	UPROPERTY(VisibleAnywhere)
	bool bIsInitialized{false};
};
