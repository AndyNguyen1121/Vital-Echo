// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CS_WeaponBase.generated.h"

class UCS_CombatProfile;
class UCS_CombatStateBase;

UCLASS()
class CAPSTONE_API ACS_WeaponBase : public AActor
{
	GENERATED_BODY()

public:
	ACS_WeaponBase();

	UFUNCTION(BlueprintCallable)
	void Equip(AActor* ParentActor);

	UFUNCTION(BlueprintCallable)
	void Unequip(AActor* ParentActor);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_Equip(AActor* ParentActor);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_Unequip(AActor* ParentActor);
	
	UFUNCTION(BlueprintPure)
	USkeletalMeshComponent* GetWeaponMesh() { return WeaponMesh; }

	UPROPERTY(EditDefaultsOnly, Category="Initialization")
	TObjectPtr<UCS_CombatProfile> CombatProfile;
	
private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> SceneComponent;
};
