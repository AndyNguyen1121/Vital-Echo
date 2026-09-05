// Fill out your copyright notice in the Description page of Project Settings.


#include "Tasks/CS_TagChangeTask.h"

#include "AbilitySystemComponent.h"

UCS_TagChangeTask* UCS_TagChangeTask::ListenForTagChange(UAbilitySystemComponent* AbilitySystemComponent,
                                                         FGameplayTag Tag)
{
	UCS_TagChangeTask* WaitForTagChangeTask = NewObject<UCS_TagChangeTask>(AbilitySystemComponent);
	AbilitySystemComponent->RegisterGameplayTagEvent(Tag, EGameplayTagEventType::AnyCountChange).AddUObject(WaitForTagChangeTask, &UCS_TagChangeTask::AttributeChanged);
	WaitForTagChangeTask->ASC = AbilitySystemComponent;
	WaitForTagChangeTask->TagToListenFor = Tag;
	
	return WaitForTagChangeTask;
}

void UCS_TagChangeTask::AttributeChanged(FGameplayTag Tag, int32 NewCount)
{
	OnTagChanged.Broadcast(Tag, NewCount);
}

void UCS_TagChangeTask::EndTask()
{
	if (ASC.IsValid())
	{
		ASC->RegisterGameplayTagEvent(TagToListenFor, EGameplayTagEventType::AnyCountChange).RemoveAll(this);
	}

	SetReadyToDestroy();
}
