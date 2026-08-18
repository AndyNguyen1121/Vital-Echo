// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CS_TokenManager.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CAPSTONE_API UCS_TokenManager : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCS_TokenManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ReturnTokensToActor(AActor* Actor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool ReserveTokensFromActor(AActor* Actor, int32 NumberOfTokens);

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 OwnedTokens;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxNumberOfTokens;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 TokensHeld;

private:
	void TakeTokens(int32 NumOfTokens);
	void GiveTokens(int32 NumOfTokens);
	
};
