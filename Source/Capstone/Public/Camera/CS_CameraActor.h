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
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly, Instanced, meta=(AllowPrivateAccess), VisibleAnywhere)

	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(BlueprintReadOnly, Instanced, meta=(AllowPrivateAccess), VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(BlueprintReadOnly, Instanced, meta=(AllowPrivateAccess), VisibleAnywhere)
	TObjectPtr<USceneComponent> SceneComponent;

	UPROPERTY()
	TWeakObjectPtr<APlayerController> Controller;

	UPROPERTY()
	TWeakObjectPtr<APawn> PlayerPawn;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bFollowPlayer{true};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bRotateWithController{true};
};
