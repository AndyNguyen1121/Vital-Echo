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
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bUsePawnControlRotation = true;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>("Camera");
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void ACS_CameraActor::BeginPlay()
{
	Super::BeginPlay();

	Controller =  UGameplayStatics::GetPlayerController(this, 0);
	PlayerPawn = UGameplayStatics::GetPlayerController(this, 0)->GetPawn();
	
}

void ACS_CameraActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (PlayerPawn.IsValid())
	{
		if (bFollowPlayer)
			SetActorLocation(PlayerPawn->GetActorLocation());

		if (bRotateWithController)
			SetActorRotation(Controller->GetControlRotation());
	}
}

