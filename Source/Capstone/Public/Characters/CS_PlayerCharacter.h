
#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "CS_BaseCharacter.h"
#include "CS_PlayerCharacter.generated.h"
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class CAPSTONE_API ACS_PlayerCharacter : public ACS_BaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACS_PlayerCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;

};
