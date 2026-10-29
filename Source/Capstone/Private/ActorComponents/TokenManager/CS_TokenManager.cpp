// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/TokenManager/CS_TokenManager.h"

#include "Interfaces/TokenInterface.h"


// Sets default values for this component's properties
UCS_TokenManager::UCS_TokenManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCS_TokenManager::BeginPlay()
{
	Super::BeginPlay();

	OwnedTokens = MaxNumberOfTokens;
}

void UCS_TokenManager::TakeTokens(int32 NumOfTokens)
{
	OwnedTokens -= NumOfTokens;
}

void UCS_TokenManager::GiveTokens(int32 NumOfTokens)
{
	OwnedTokens += NumOfTokens;
}

bool UCS_TokenManager::ReserveTokensFromActor_Implementation(AActor* Actor, int32 NumberOfTokens)
{
	if (!IsValid(Actor)) return false;

	if (Actor->Implements<UTokenInterface>())
	{
		if (UCS_TokenManager* ActorTokenManager = ITokenInterface::Execute_GetTokenManager(Actor))
		{
			if (ActorTokenManager->OwnedTokens < NumberOfTokens)
			{
				return false;
			}

			ActorTokenManager->TakeTokens(NumberOfTokens);
			TokensHeld += NumberOfTokens;
			return true;
		}
	}
	
	return false;
}

void UCS_TokenManager::ReturnTokensToActor_Implementation(AActor* Actor)
{
	if (!IsValid(Actor)) return;


	if (Actor->Implements<UTokenInterface>())
	{
		if (UCS_TokenManager* ActorTokenManager = ITokenInterface::Execute_GetTokenManager(Actor))
		{
			ActorTokenManager->GiveTokens(TokensHeld);
			TokensHeld = 0;
		}
	}
}





