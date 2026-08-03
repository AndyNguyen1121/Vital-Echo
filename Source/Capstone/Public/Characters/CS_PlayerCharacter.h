
#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "CS_BaseCharacter.h"
#include "Structs/DamageAttributes.h"
#include "CS_PlayerCharacter.generated.h"
class ACS_PlayerCameraManager;
class ACS_PlayerController;
class ACS_PlayerState;
class UCS_PlayerStateMachineManager;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class CAPSTONE_API ACS_PlayerCharacter : public ACS_BaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACS_PlayerCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;

	UFUNCTION(BlueprintPure)
	UCS_PlayerStateMachineManager* GetStateMachineManager() {return PlayerStateMachineManager;}

	UFUNCTION(BlueprintPure)
	ACS_PlayerState* GetCSPlayerState() const {return CachedPlayerState;}

	UFUNCTION(BlueprintPure)
	ACS_PlayerController* GetCSPlayerController() const {return CachedPlayerController;}

	UFUNCTION(BlueprintPure)
	ACS_PlayerCameraManager* GetCSCameraManager() const;

	UPROPERTY(EditDefaultsOnly, Category="CS|LockOn")
	float LockOnSearchRadius{500.f};

	UPROPERTY(EditDefaultsOnly, Category="CS|LockOn")
	float LockOnMaxAngle{45};

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void RotatePlayerToTargetRotation(FRotator TargetRotation, float Duration);

	UFUNCTION(BlueprintCallable)
	void ToggleLockOn();

	UFUNCTION(BlueprintCallable)
	void WarpToLockOnTarget(float MinRange, float MaxRange, FVector DistanceOffset);

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UCS_PlayerStateMachineManager> PlayerStateMachineManager;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	ACS_PlayerState* CachedPlayerState;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	ACS_PlayerController* CachedPlayerController;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	TSoftObjectPtr<AActor> LockOnTarget;

	FVector FrozenLocation;
	
};
