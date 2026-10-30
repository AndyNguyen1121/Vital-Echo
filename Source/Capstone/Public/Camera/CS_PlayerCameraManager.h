// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "CS_PlayerCameraManager.generated.h"

class UPostProcessComponent;
class ACS_PlayerCharacter;
class ACS_CameraContainer;
class UCameraComponent;
class USpringArmComponent;
/**
 * 
 */
UCLASS()
class CAPSTONE_API ACS_PlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
protected:
	ACS_PlayerCameraManager();
	virtual void BeginPlay() override;

	virtual void ApplyCameraModifiers(float DeltaTime, FMinimalViewInfo& InOutPOV) override;

public:
	AActor* EnableLockOn(float SearchRadius, float MaxAngle);
	void DisableLockOn();

	UFUNCTION(BlueprintPure)
	ACS_CameraContainer* GetCameraContainer() const;

	void SwitchLockOnTargets(const FVector2D ControllerInput, ACS_PlayerCharacter* PlayerCharacter);

	// Camera effects
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void BlurScreen(UCurveFloat* Curve);
	
private:
	AActor* FindBestLockOnTarget(float SearchRadius, float MaxAngle, FLinearColor DebugColor = FLinearColor::Blue);

	TArray<AActor*> FindValidLockOnTargets(float SearchRadius, float MaxAngle, FLinearColor DebugColor = FLinearColor::Blue);
	
	float ScoreTarget(AActor* Candidate, FVector2D ScreenCenter, FVector PlayerLoc);

private:
	UPROPERTY(EditAnywhere)
	bool bShowLockOnDebug{true};

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UPostProcessComponent> PostProcessComponent;
	
};
