// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagAssetInterface.h"
#include "GameFramework/Character.h"
#include "Structs/DamageAttributes.h"
#include "ContextualAnimSceneActorComponent.h"
#include "Interfaces/HitStopComponentInterface.h"
#include "Interfaces/TokenInterface.h"
#include "Structs/WarpLocationInfo.h"
#include "CS_BaseCharacter.generated.h"

class UCS_HitStopComponent;
class UCS_TokenManager;
class UMotionWarpingComponent;
class UAttributeSet;
class UGameplayEffect;

UCLASS()
class CAPSTONE_API ACS_BaseCharacter : public ACharacter, public IAbilitySystemInterface, public IGameplayTagAssetInterface, public ITokenInterface, public IHitStopComponentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACS_BaseCharacter();
	virtual void BeginPlay() override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintPure)
	UMotionWarpingComponent* GetMotionWarpingComponent() const { return MotionWarpingComponent; }

	UFUNCTION(BlueprintPure)
	UContextualAnimSceneActorComponent* GetContextualAnimationComponent() const { return ContextualAnimSceneComponent; }

	virtual UCS_HitStopComponent* GetHitStopComponent_Implementation() override { return HitStopComponent; };

	virtual UCS_TokenManager* GetTokenManager_Implementation() override  { return TokenManager; }

	UFUNCTION(BlueprintPure)
	float GetInitialGravityScale() const { return InitialGravityScale; }

	UFUNCTION(BlueprintCallable)
	void ResetGravityScale();

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	UFUNCTION(BlueprintPure)
	FVector2D GetStrafeDirection();
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

	UPROPERTY(VisibleAnywhere)
	bool bIsDead{false};

private:
	void InitializeAttributes();
	
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> AttributeInitializationEffect;

	float InitialGravityScale;

	bool bInitialGravitySet;

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMotionWarpingComponent> MotionWarpingComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UContextualAnimSceneActorComponent> ContextualAnimSceneComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UCS_TokenManager> TokenManager;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UCS_HitStopComponent> HitStopComponent;
};
