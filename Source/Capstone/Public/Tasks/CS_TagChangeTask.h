// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "CS_TagChangeTask.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTagChanged, FGameplayTag, Tag, int32, NewCount);

UCLASS(BlueprintType, meta=(ExposedAsyncProxy = AsyncTask))
class CAPSTONE_API UCS_TagChangeTask : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnTagChanged OnTagChanged;
	
	UFUNCTION(BlueprintCallable, meta= (BlueprintInternalUseOnly = "true"))
	static UCS_TagChangeTask* ListenForTagChange(UAbilitySystemComponent* AbilitySystemComponent, FGameplayTag Tag);

	TWeakObjectPtr<UAbilitySystemComponent> ASC;
	FGameplayTag TagToListenFor;

	void AttributeChanged(FGameplayTag Tag, int32 NewCount);

	UFUNCTION(BlueprintCallable)
	void EndTask();
};
