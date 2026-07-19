// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CS_CameraContainer.generated.h"

class ACS_CameraActor;

UCLASS()
class CAPSTONE_API ACS_CameraContainer : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACS_CameraContainer();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
	void InitializeContainer(APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable)
	void SwitchToMainCamera();

	UFUNCTION(BlueprintCallable)
	void SwitchToLockOnCamera();

	UFUNCTION(BlueprintCallable)
	void SetLockOnTarget(AActor* Target);

	UFUNCTION(BlueprintCallable)
	ACS_CameraActor* GetActiveCamera();

	UFUNCTION(BlueprintPure)
	AActor* GetLockOnActor() { return CurrentLockOnTarget.Get(); }
	
public:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ACS_CameraActor> MainCamera;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ACS_CameraActor> LockOnCamera;

	UPROPERTY(EditDefaultsOnly)
	float BlendTime{0.25f};
	
	UPROPERTY(EditDefaultsOnly)
	float MinLockOnPitch{-20.f};
	
	UPROPERTY(EditDefaultsOnly)
	float MaxLockOnPitch{20.f};

	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<EViewTargetBlendFunction> BlendFunction = EViewTargetBlendFunction::VTBlend_Cubic;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bFollowPlayer{true};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bRotateWithController{true};

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UChildActorComponent> MainCameraChildActor;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UChildActorComponent> LockOnCameraChildActor;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> SceneComponent;
	
private:
	UPROPERTY()
	TWeakObjectPtr<APlayerController> Controller;

	UPROPERTY()
	TWeakObjectPtr<AActor> CurrentLockOnTarget;

	FVector InitialLockOnSocketOffset;
	
};
