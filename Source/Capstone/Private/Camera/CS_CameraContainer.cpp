// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CS_CameraContainer.h"

#include "Camera/CS_CameraActor.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ACS_CameraContainer::ACS_CameraContainer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(SceneComponent);
	
	MainCameraChildActor = CreateDefaultSubobject<UChildActorComponent>("MainCamera");
	MainCameraChildActor->SetupAttachment(GetRootComponent());
	LockOnCameraChildActor = CreateDefaultSubobject<UChildActorComponent>("LockOnCamera");
	LockOnCameraChildActor->SetupAttachment(GetRootComponent());
}

void ACS_CameraContainer::BeginPlay()
{
	Super::BeginPlay();

	MainCamera = Cast<ACS_CameraActor>(MainCameraChildActor->GetChildActor());
	LockOnCamera = Cast<ACS_CameraActor>(LockOnCameraChildActor->GetChildActor());
	InitialLockOnSocketOffset = LockOnCamera->CameraBoom->SocketOffset;
}

void ACS_CameraContainer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (Controller.IsValid() && IsValid(Controller->GetPawn()))
	{
		if (GetActiveCamera() == LockOnCamera && CurrentLockOnTarget.IsValid())
		{
			FRotator DesiredRotation;
			DesiredRotation = UKismetMathLibrary::FindLookAtRotation(LockOnCamera->GetActorLocation() + LockOnCamera->CameraBoom->TargetOffset, CurrentLockOnTarget->GetActorLocation());

			float HeightDifference = CurrentLockOnTarget->GetActorLocation().Z - Controller->GetPawn()->GetActorLocation().Z;
			
			DesiredRotation.Pitch = FMath::Clamp(
					DesiredRotation.Pitch,
					MinLockOnPitch,
					MaxLockOnPitch);
			LockOnCamera->CameraBoom->SocketOffset.Z = (InitialLockOnSocketOffset.Z + (HeightDifference * 0.3f));
			Controller->SetControlRotation(DesiredRotation);
		}

		SetActorLocation(Controller->GetPawn()->GetActorLocation());
		SetActorRotation(Controller->GetControlRotation());
	}
}

void ACS_CameraContainer::InitializeContainer(APlayerController* PlayerController)
{
	Controller = PlayerController;

	if (MainCamera)
		MainCamera->InitializeCamera(PlayerController);

	if (LockOnCamera)
		LockOnCamera->InitializeCamera(PlayerController);

	Controller->SetViewTarget(MainCamera);
}

void ACS_CameraContainer::SwitchToMainCamera()
{
	if (!Controller.IsValid()) return;
	
	Controller->SetViewTargetWithBlend(MainCamera, BlendTime, BlendFunction,0, true);
	CurrentLockOnTarget = nullptr;
}

void ACS_CameraContainer::SwitchToLockOnCamera()
{
	if (!Controller.IsValid()) return;
	
	Controller->SetViewTargetWithBlend(LockOnCamera, BlendTime, BlendFunction, 0, true);
}

void ACS_CameraContainer::SetLockOnTarget(AActor* Target)
{
	CurrentLockOnTarget = Target;

	if (!CurrentLockOnTarget.IsValid())
	{
		SwitchToMainCamera();
	}
	
}

ACS_CameraActor* ACS_CameraContainer::GetActiveCamera()
{
	return CurrentLockOnTarget.IsValid() ? LockOnCamera : MainCamera;
}


