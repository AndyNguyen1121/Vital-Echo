// Fill out your copyright notice in the Description page of Project Settings.


#include "Capstone/Public/Characters/CS_BaseCharacter.h"

#include "MotionWarpingComponent.h"
#include "AbilitySystem/CS_AttributeSet.h"
#include "ActorComponents/CombatSystem/CS_HitStopComponent.h"
#include "ActorComponents/TokenManager/CS_TokenManager.h"
#include "Capstone/Public/Player/CS_PlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayTags/CSTags.h"
#include "Utils/CS_BlueprintLibrary.h"
#include "Weapons/CS_WeaponBase.h"


// Sets default values
ACS_BaseCharacter::ACS_BaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>("MotionWarpingComponent");
	ContextualAnimSceneComponent = CreateDefaultSubobject<UContextualAnimSceneActorComponent>("ContextualAnimSceneComponent");
	TokenManager = CreateDefaultSubobject<UCS_TokenManager>("TokenManager");
	HitStopComponent = CreateDefaultSubobject<UCS_HitStopComponent>("HitStopComponent");
	ContextualAnimSceneComponent->SetupAttachment(GetRootComponent());
}

void ACS_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitialGravityScale = GetCharacterMovement()->GravityScale;
	bInitialGravitySet = true;
	
	InitializeAttributes();
}

UAbilitySystemComponent* ACS_BaseCharacter::GetAbilitySystemComponent() const
{
	return nullptr;
}


void ACS_BaseCharacter::ResetGravityScale()
{
	if (!bInitialGravitySet) return;
	
	GetCharacterMovement()->GravityScale = InitialGravityScale;
}

void ACS_BaseCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if (!GetAbilitySystemComponent()) return;

	TagContainer = GetAbilitySystemComponent()->GetOwnedGameplayTags();
}

FVector2D ACS_BaseCharacter::GetStrafeDirection()
{
	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (MovementComponent->Velocity.IsNearlyZero()) return FVector2D::ZeroVector;

	FVector CurrentVelocity = MovementComponent->Velocity;
	CurrentVelocity.Z = 0;

	FVector LocalVelocity = GetActorTransform().InverseTransformVectorNoScale(CurrentVelocity);
	LocalVelocity.Normalize();

	return FVector2D(LocalVelocity.X, LocalVelocity.Y);
}

void ACS_BaseCharacter::InitializeAttributes()
{
	if (!AttributeInitializationEffect) return;

	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(AttributeInitializationEffect, 1.f, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	OnASCInitialized.Broadcast(GetAbilitySystemComponent(), GetAttributeSet());
	
	if (GetAttributeSet())
		GetAttributeSet()->PostAttributesInitialized();
}

void ACS_BaseCharacter::SetGameplayState(bool CanMove, bool IsPerformingAction, bool CanRotate, bool ResetToDefaultState)
{
	UCS_BlueprintLibrary::SetLooseGameplayTagAsBool(this, CSTags::Status::GameplayStates::RestrictMove, !CanMove);
	UCS_BlueprintLibrary::SetLooseGameplayTagAsBool(this, CSTags::Status::GameplayStates::IsPerformingAction, IsPerformingAction);
	UCS_BlueprintLibrary::SetLooseGameplayTagAsBool(this, CSTags::Status::GameplayStates::RestrictRotate, !CanRotate);
}

void ACS_BaseCharacter::EquipWeapon(ACS_WeaponBase* NewWeapon)
{
	if (CurrentEquippedWeapon.IsValid())
	{
		CurrentEquippedWeapon->Unequip(this);
	}
	NewWeapon->Equip(this);
	CurrentEquippedWeapon = NewWeapon;
	OnWeaponEquipped.Broadcast(NewWeapon);
}

void ACS_BaseCharacter::ChangeWeaponType(EWeaponType NewWeaponType)
{
	if (NewWeaponType != CurrentEquippedWeaponType)
	{
		OnWeaponTypeChanged.Broadcast(NewWeaponType);
	}

	CurrentEquippedWeaponType = NewWeaponType;
}
