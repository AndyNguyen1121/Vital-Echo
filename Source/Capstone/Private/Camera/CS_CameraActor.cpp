// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CS_CameraActor.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ACS_CameraActor::ACS_CameraActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(SceneComponent);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = true;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>("Camera");
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void ACS_CameraActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACS_CameraActor::InitializeCamera(APlayerController* PlayerController)
{
	if (!IsValid(PlayerController)) return;
	
	bIsInitialized = true;
	Controller = PlayerController; 
	PlayerPawn = Controller->GetPawn();
	
}

