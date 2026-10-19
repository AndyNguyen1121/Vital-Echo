// Fill out your copyright notice in the Description page of Project Settings.


#include "Capstone/Public/Characters/CS_BaseCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "MotionWarpingComponent.h"
#include "AbilitySystem/AttributeSets/CS_AttributeSet.h"
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

void ACS_BaseCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	HandlePostureDepletion(DeltaSeconds);
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
	{
		GetAttributeSet()->OnDamaged.AddDynamic(this, &ThisClass::ACS_BaseCharacter::OnHealthChanged);
		GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(UCS_AttributeSet::GetPostureAttribute()).AddUObject(this, &ACS_BaseCharacter::OnPostureChange);
		GetAttributeSet()->PostAttributesInitialized();
	}
}

void ACS_BaseCharacter::HandlePostureDepletion(float DeltaSeconds)
{
	if (!GetAttributeSet() || GetAttributeSet()->GetPosture() == 0 || HasMatchingGameplayTag(CSTags::Status::GameplayStates::PostureRegenSupressed))
		return;

	UCS_AttributeSet* CachedAttributeSet = GetAttributeSet();
	CachedAttributeSet->SetPosture(CachedAttributeSet->GetPosture() - CachedAttributeSet->GetPostureRecoverRate() * DeltaSeconds);
}

void ACS_BaseCharacter::OnPostureChange(const FOnAttributeChangeData& AttributeChangeData)
{
	if (!GetAttributeSet()) return;
	
	if (AttributeChangeData.NewValue >= GetAttributeSet()->GetPostureThreshold())
	{
		GetAttributeSet()->SetPosture(0);
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, CSTags::Events::Gameplay::PostureBroken, FGameplayEventData());
	}
}

void ACS_BaseCharacter::OnHealthChanged(float OldValue, float NewValue, AActor* EventInstigator)
{
	FGameplayEventData Payload;
	Payload.Target = this;
	Payload.Instigator = EventInstigator;

	if (NewValue == 0)
	{
		// Dead
		OnDeath.Broadcast();
		bIsDead = true;
		return;
	}
	
	if (NewValue < OldValue && !HasAnyMatchingGameplayTags(FGameplayTagContainer(CSTags::Status::GameplayStates::IsBlocking)))
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, CSTags::Events::Gameplay::HitReact, Payload);
	}
	else if (HasAnyMatchingGameplayTags(FGameplayTagContainer(CSTags::Status::GameplayStates::IsBlocking)) && IsValid(EventInstigator))
	{
		// turn to face enemy
		UCS_BlueprintLibrary::TurnToFaceActor(this, EventInstigator);
	}
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
