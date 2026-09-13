
#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "CS_BaseCharacter.h"
#include "Structs/DamageAttributes.h"
#include "CS_PlayerCharacter.generated.h"
class UWidgetComponent;
class UWidget;
class UCS_WidgetComponent;
class UCS_SoftLockOnComponent;
class UCS_TagChangeTask;
class UCS_TokenManager;
class ACS_PlayerCameraManager;
class ACS_PlayerController;
class ACS_PlayerState;
class UCS_PlayerStateMachineManager;
class USpringArmComponent;
class UCameraComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActionStopped);

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
	UCS_SoftLockOnComponent* GetSoftLockOnComponent() const {return SoftLockOnComponent;}

	UFUNCTION(BlueprintPure)
	ACS_PlayerState* GetCSPlayerState() const {return CachedPlayerState;}

	UFUNCTION(BlueprintPure)
	ACS_PlayerController* GetCSPlayerController() const {return CachedPlayerController;}

	UFUNCTION(BlueprintPure)
	ACS_PlayerCameraManager* GetCSCameraManager() const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void RotatePlayerToTargetRotation(FRotator TargetRotation, float Duration);

	UFUNCTION(BlueprintCallable)
	void ToggleLockOn();

	UFUNCTION(BlueprintCallable)
	void WarpToLockOnTarget(float MinRange, float MaxRange, FVector DistanceOffset);

	UFUNCTION(BlueprintCallable)
	void SetLockOnTarget(AActor* Target);

	virtual UCS_AttributeSet* GetAttributeSet() override;

	virtual void SetGameplayState(bool CanMove, bool IsPerformingAction, bool CanRotate, bool ResetToDefaultState) override;

	UFUNCTION(BlueprintCallable)
	void SendWeaponSwitchRequest(ACS_WeaponBase* Weapon);

public:
	UPROPERTY(EditDefaultsOnly, Category="CS|LockOn")
	float LockOnSearchRadius{500.f};

	UPROPERTY(EditDefaultsOnly, Category="CS|LockOn")
	float LockOnMaxAngle{45};

	UPROPERTY(BlueprintAssignable)
	FOnActionStopped OnActionStopped;

	UPROPERTY(EditDefaultsOnly, Category="CS|LockOn")
	TObjectPtr<UWidgetComponent> LockOnReticle;
	
private:
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;

	UFUNCTION()
	void CheckIfActionStopped(FGameplayTag Tag, int32 NewCount);

	UFUNCTION()
	void CheckWeaponSwitchRequest();

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UCS_PlayerStateMachineManager> PlayerStateMachineManager;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UCS_SoftLockOnComponent> SoftLockOnComponent;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	ACS_PlayerState* CachedPlayerState;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	ACS_PlayerController* CachedPlayerController;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess), VisibleAnywhere)
	TSoftObjectPtr<AActor> LockOnTarget;

	FVector FrozenLocation;

	UPROPERTY(VisibleAnywhere)
	TWeakObjectPtr<ACS_WeaponBase> RequestedWeapon;

	UPROPERTY()
	TObjectPtr<UCS_TagChangeTask> PerformingActionTask;

	UPROPERTY()
	TObjectPtr<UCS_TagChangeTask> InputWindowTask;
	
};
