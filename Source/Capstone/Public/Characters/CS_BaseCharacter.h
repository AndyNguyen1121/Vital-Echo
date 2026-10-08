// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagAssetInterface.h"
#include "GameFramework/Character.h"
#include "Structs/DamageAttributes.h"
#include "ContextualAnimSceneActorComponent.h"
#include "AbilitySystem/CS_AttributeSet.h"
#include "Enums/WeaponTypes.h"
#include "Interfaces/HitStopComponentInterface.h"
#include "Interfaces/TokenInterface.h"
#include "GameplayEffectTypes.h"
#include "Structs/WarpLocationInfo.h"
#include "CS_BaseCharacter.generated.h"

class ACS_WeaponBase;
class UCS_AttributeSet;
class UCS_HitStopComponent;
class UCS_TokenManager;
class UMotionWarpingComponent;
class UAttributeSet;
class UGameplayEffect;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FASCInitialized, UAbilitySystemComponent*, ASC, UAttributeSet*, AS);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponEquipped, ACS_WeaponBase*, NewWeapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponTypeChanged, EWeaponType, NewWeaponType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

UCLASS()
class CAPSTONE_API ACS_BaseCharacter : public ACharacter, public IAbilitySystemInterface, public IGameplayTagAssetInterface, public ITokenInterface, public IHitStopComponentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACS_BaseCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintPure)
	UMotionWarpingComponent* GetMotionWarpingComponent() const { return MotionWarpingComponent; }

	UFUNCTION(BlueprintPure)
	UContextualAnimSceneActorComponent* GetContextualAnimationComponent() const { return ContextualAnimSceneComponent; }

	virtual UCS_HitStopComponent* GetHitStopComponent_Implementation() override { return HitStopComponent; };

	virtual UCS_TokenManager* GetTokenManager_Implementation() override  { return TokenManager; }

	UFUNCTION(BlueprintPure)
	virtual ACS_WeaponBase* GetCurrentEquippedWeapon() { return CurrentEquippedWeapon.Get(); }

	UFUNCTION(BlueprintPure)
	float GetInitialGravityScale() const { return InitialGravityScale; }

	UFUNCTION(BlueprintCallable)
	void ResetGravityScale();

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	UFUNCTION(BlueprintPure)
	FVector2D GetStrafeDirection();

	UFUNCTION(BlueprintPure)
	virtual UCS_AttributeSet* GetAttributeSet() { return nullptr; };

	UFUNCTION(BlueprintCallable)
	virtual void SetGameplayState(bool CanMove, bool IsPerformingAction, bool CanRotate, bool ResetToDefaultState);

	UFUNCTION(BlueprintCallable)
	virtual void EquipWeapon(ACS_WeaponBase* NewWeapon);

	UFUNCTION(BlueprintCallable)
	void ChangeWeaponType(EWeaponType NewWeaponType);

	UFUNCTION(BlueprintPure)
	bool IsDead() const { return bIsDead; }
	
	
public:
	UPROPERTY(VisibleAnywhere)
	TSet<AActor*> ActorsHit;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FDamageAttributes CurrentDamageAttribute;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FDamageFeedback CurrentDamageVisuals;

	UPROPERTY(BlueprintReadOnly)
	float MoveAmount;

	UPROPERTY(VisibleAnywhere)
	FWarpLocationInfo WarpLocationInfo;
	
	UPROPERTY(BlueprintAssignable)
	FASCInitialized OnASCInitialized;

	UPROPERTY(BlueprintAssignable)
	FWeaponEquipped OnWeaponEquipped;

	UPROPERTY(BlueprintAssignable)
	FWeaponTypeChanged OnWeaponTypeChanged;

	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeath;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	EWeaponType CurrentEquippedWeaponType;

private:
	void InitializeAttributes();
	
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> AttributeInitializationEffect;

	float InitialGravityScale;

	bool bInitialGravitySet;

	UPROPERTY(VisibleAnywhere)
	bool bIsDead{false};

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMotionWarpingComponent> MotionWarpingComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UContextualAnimSceneActorComponent> ContextualAnimSceneComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UCS_TokenManager> TokenManager;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UCS_HitStopComponent> HitStopComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TWeakObjectPtr<ACS_WeaponBase> CurrentEquippedWeapon;

protected:
	void HandlePostureDepletion(float DeltaSeconds);
	
	void OnPostureChange(const FOnAttributeChangeData& AttributeChangeData);

	UFUNCTION()
	virtual void OnHealthChanged(float OldValue, float NewValue, AActor* EventInstigator);
};
