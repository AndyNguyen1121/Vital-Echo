// Fill out your copyright notice in the Description page of Project Settings.


#include "Cheats/CS_CheatManager.h"

#include "AbilitySystem/AttributeSets/CS_Player_AttributeSet.h"
#include "Characters/CS_PlayerCharacter.h"
#include "Notifies/NotifyStates/ANS_ActivateAttackWindow.h"

void UCS_CheatManager::InitCheatManager()
{
	Super::InitCheatManager();
}

void UCS_CheatManager::ToggleDebugHitBoxes()
{
	UANS_ActivateAttackWindow::bDebug = !UANS_ActivateAttackWindow::bDebug;
}

void UCS_CheatManager::UnlimitedDash()
{
	if (!GetPlayerController() || !GetPlayerController()->GetPawn())
		return;

	ACS_PlayerCharacter* PlayerCharacter = Cast<ACS_PlayerCharacter>(GetPlayerController()->GetPawn());
	UCS_Player_AttributeSet* PlayerAttributeSet = Cast<UCS_Player_AttributeSet>(PlayerCharacter->GetAttributeSet());

	if (!PlayerAttributeSet)
		return;

	PlayerAttributeSet->SetHeatGaugeLimit(1000000);
	PlayerAttributeSet->SetHeatGauge(1000000);
}

void UCS_CheatManager::UnlimitedHealth()
{
	if (!GetPlayerController() || !GetPlayerController()->GetPawn())
		return;

	ACS_PlayerCharacter* PlayerCharacter = Cast<ACS_PlayerCharacter>(GetPlayerController()->GetPawn());
	UCS_Player_AttributeSet* PlayerAttributeSet = Cast<UCS_Player_AttributeSet>(PlayerCharacter->GetAttributeSet());

	if (!PlayerAttributeSet)
		return;

	PlayerAttributeSet->SetMaxHealth(1000000);
	PlayerAttributeSet->SetHealth(1000000);
}
