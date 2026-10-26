// Fill out your copyright notice in the Description page of Project Settings.


#include "Capstone/Public/Characters/CS_PlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "AnalyticsProviderETEventCache.h"
#include "MotionWarpingComponent.h"
#include "AbilitySystem/AttributeSets/CS_AttributeSet.h"
#include "AbilitySystem/AttributeSets/CS_Player_AttributeSet.h"
#include "ActorComponents/CombatSystem/CS_SoftLockOnComponent.h"
#include "Camera/CameraComponent.h"
#include "Camera/CS_CameraActor.h"
#include "Camera/CS_CameraContainer.h"
#include "Camera/CS_PlayerCameraManager.h"
#include "Capstone/DebugMacros.h"
#include "Capstone/Public/Player/CS_PlayerState.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameplayTags/CSTags.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/CS_PlayerController.h"
#include "ActorComponents/StateMachine/CS_PlayerStateMachineManager.h"
#include "ActorComponents/TokenManager/CS_TokenManager.h"
#include "Components/Widget.h"
#include "Components/WidgetComponent.h"
#include "Tasks/CS_TagChangeTask.h"
#include "Weapons/CS_WeaponBase.h"


// Sets default values
ACS_PlayerCharacter::ACS_PlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PlayerStateMachineManager = CreateDefaultSubobject<UCS_PlayerStateMachineManager>("StateMachine");
	SoftLockOnComponent = CreateDefaultSubobject<UCS_SoftLockOnComponent>("SoftLockOnComponent");
	LockOnReticle = CreateDefaultSubobject<UWidgetComponent>("LockOnWidget");
	LockOnReticle->SetupAttachment(GetCapsuleComponent());
	LockOnReticle->SetVisibility(false);

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 540, 0);
	GetCharacterMovement()->JumpZVelocity = 500;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.f;

	WalkSpeed = 600.f;
	SprintSpeed = 900.f;
	
}

void ACS_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetAbilitySystemComponent()->AddLooseGameplayTag(CSTags::IDs::Player);

	// Listen for IsPerformingAction
	PerformingActionTask = UCS_TagChangeTask::ListenForTagChange(
		GetAbilitySystemComponent(),
		CSTags::Status::GameplayStates::IsPerformingAction
	);

	PerformingActionTask->OnTagChanged.AddDynamic(
		this,
		&ThisClass::ACS_PlayerCharacter::CheckIfActionStopped
	);

	InputWindowTask = UCS_TagChangeTask::ListenForTagChange(
		GetAbilitySystemComponent(),
		CSTags::Status::GameplayStates::PlayerStates::InputWindowActive
	);

	InputWindowTask->OnTagChanged.AddDynamic(
		this,
		&ThisClass::ACS_PlayerCharacter::CheckIfActionStopped
	);

	OnActionStopped.AddDynamic(this, &ThisClass::CheckWeaponSwitchRequest);
}

void ACS_PlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	TryReplenishHeatGauge(DeltaSeconds);
}

UAbilitySystemComponent* ACS_PlayerCharacter::GetAbilitySystemComponent() const
{
	
	if (ACS_PlayerState* CurrentPlayerState = Cast<ACS_PlayerState>(GetPlayerState()))
	{
		return CurrentPlayerState->GetAbilitySystemComponent();
	}

	return nullptr;
}

void ACS_PlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!GetAbilitySystemComponent()) return;

	GetAbilitySystemComponent()->InitAbilityActorInfo(GetPlayerState(), this);

	CachedPlayerState = Cast<ACS_PlayerState>(GetPlayerState());
	CachedPlayerController = Cast<ACS_PlayerController>(NewController);
}

#pragma region Lock On
ACS_PlayerCameraManager* ACS_PlayerCharacter::GetCSCameraManager() const
{
	return Cast<ACS_PlayerCameraManager>(UGameplayStatics::GetPlayerCameraManager(this, 0));
}

UCS_Player_AttributeSet* ACS_PlayerCharacter::GetPlayerAttributeSet() const
{
	return Cast<UCS_Player_AttributeSet>(GetAttributeSet());
}

void ACS_PlayerCharacter::ToggleLockOn()
{
	ACS_PlayerCameraManager* CachedCamera = GetCSCameraManager();
	if (!IsValid(CachedCamera)) return;
	
	if (!GetCSPlayerState()->bIsHardLockedOn)
	{
		LockOnTarget = CachedCamera->EnableLockOn(LockOnSearchRadius, LockOnMaxAngle);
		SetLockOnTarget(LockOnTarget.Get());
	}
	else
	{
		DisableLockOn();
	}
}

void ACS_PlayerCharacter::WarpToLockOnTarget(float MinRange, float MaxRange, FVector DistanceOffset)
{
	MotionWarpingComponent->RemoveAllWarpTargets();
	if (!LockOnTarget.IsValid()) return;
	
	float DistanceToEnemySqr = LockOnTarget->GetSquaredDistanceTo(this);
	if (DistanceToEnemySqr < FMath::Pow(MinRange, 2) || DistanceToEnemySqr > FMath::Pow(MaxRange, 2)) return;

	FVector DirFromLockOnTarget = LockOnTarget->GetActorLocation() - GetActorLocation();
	DirFromLockOnTarget.Z = 0;
	DirFromLockOnTarget.Normalize();
	
	FVector RightVectorFromDirFromLockOnTarget = FVector::CrossProduct(-DirFromLockOnTarget, FVector::UpVector);
	FVector Offset = (RightVectorFromDirFromLockOnTarget * DistanceOffset.Y) + (GetActorUpVector() * DistanceOffset.Z) + (DirFromLockOnTarget * DistanceOffset.X);
	FRotator RotationOffset = DirFromLockOnTarget.Rotation() - LockOnTarget->GetRootComponent()->GetComponentRotation();
	RotationOffset.Pitch = 0;
	RotationOffset.Roll = 0;
	
	MotionWarpingComponent->AddOrUpdateWarpTargetFromComponent
		(FName("Warp"),
			LockOnTarget->GetRootComponent(),
			NAME_None,
			true,
			EWarpTargetLocationOffsetDirection::WorldSpace,
			Offset,
			RotationOffset);
}

void ACS_PlayerCharacter::SetLockOnTarget(AActor* Target, FName SocketName)
{
	// Remove binding from previous target
	if (IsValid(GetLockOnTarget()))
	{
		if (ACS_BaseCharacter* PrevLockOnTarget = Cast<ACS_BaseCharacter>(LockOnTarget.Get()))
		{
			PrevLockOnTarget->OnDeath.RemoveDynamic(this, &ThisClass::DisableLockOn);
		}
	}

	LockOnTarget = Target;
	if (!IsValid(Target))
	{
		LockOnReticle->SetVisibility(false);
		return;
	}
	ACS_BaseCharacter* TargetCharacter = Cast<ACS_BaseCharacter>(Target);

	if (IsValid(TargetCharacter))
	{
		LockOnReticle->AttachToComponent(TargetCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false), SocketName);
		LockOnReticle->SetVisibility(true);
		TargetCharacter->OnDeath.AddDynamic(this, &ThisClass::DisableLockOn);
	}
}

#pragma endregion 

UCS_AttributeSet* ACS_PlayerCharacter::GetAttributeSet() const
{
	if (!GetCSPlayerState()) return nullptr;
	
	return GetCSPlayerState()->GetAttributeSet();
}

void ACS_PlayerCharacter::SetGameplayState(bool CanMove, bool IsPerformingAction, bool CanRotate,
	bool ResetToDefaultState)
{
	Super::SetGameplayState(CanMove, IsPerformingAction, CanRotate, ResetToDefaultState);

	if (ResetToDefaultState && PlayerStateMachineManager)
	{
		PlayerStateMachineManager->ResetToDefaultState();
	}
}

void ACS_PlayerCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	if (GetCharacterMovement()->MovementMode == MOVE_Falling || GetCharacterMovement()->MovementMode == MOVE_Flying)
	{
		GetCapsuleComponent()->SetCollisionProfileName(FName("AirbornePawn"));
	}

	if (GetCharacterMovement()->MovementMode == MOVE_Walking)
	{
		GetCapsuleComponent()->SetCollisionProfileName(FName("Pawn"));
	}
}

void ACS_PlayerCharacter::CheckIfActionStopped(FGameplayTag Tag, int32 NewCount)
{
	if ((Tag == CSTags::Status::GameplayStates::IsPerformingAction && NewCount == 0)
		|| (Tag == CSTags::Status::GameplayStates::PlayerStates::InputWindowActive && NewCount == 1))
	{
		OnActionStopped.Broadcast();
	}
}

void ACS_PlayerCharacter::SendWeaponSwitchRequest(ACS_WeaponBase* Weapon)
{
	RequestedWeapon = Weapon;
	CheckWeaponSwitchRequest();
}

void ACS_PlayerCharacter::DisableLockOn()
{
	ACS_PlayerCameraManager* CachedCamera = GetCSCameraManager();
	if (!IsValid(CachedCamera)) return;
	
	SetLockOnTarget(nullptr);
	CachedCamera->DisableLockOn();
}


void ACS_PlayerCharacter::CheckWeaponSwitchRequest()
{
	if (HasMatchingGameplayTag(CSTags::Status::Debuffs::Stunned))
	{
		return;
	}
	
	if (HasMatchingGameplayTag(CSTags::Status::GameplayStates::IsPerformingAction) &&
		!HasMatchingGameplayTag(CSTags::Status::GameplayStates::PlayerStates::InputWindowActive))
	{
		return;
	}
	
	if (RequestedWeapon.IsValid())
	{
		EquipWeapon(RequestedWeapon.Get());
		RequestedWeapon = nullptr;
	}
}

void ACS_PlayerCharacter::TryReplenishHeatGauge(float DeltaSeconds)
{
	if (!GetPlayerAttributeSet()  ||
		HasMatchingGameplayTag(CSTags::Status::GameplayStates::PlayerStates::HeatGaugeRegenSuppressed))
		return;

	TimeSinceLastReposition += DeltaSeconds;

	// If cooldown is over and heat gauge value is 0
	if (TimeSinceLastReposition > GetPlayerAttributeSet()->GetHeatGaugeRecoverRate() && GetPlayerAttributeSet()->GetHeatGauge() != GetPlayerAttributeSet()->GetHeatGaugeLimit())
	{
		// Will replace hardcoded value with max num of repositions
		float BarRefillAmount = GetPlayerAttributeSet()->GetHeatGaugeLimit() / 5.f;
		UCS_Player_AttributeSet* CachedAttributeSet = GetPlayerAttributeSet();
		CachedAttributeSet->SetHeatGauge(CachedAttributeSet->GetHeatGauge() + BarRefillAmount);
		TimeSinceLastReposition = 0;
	}
}





