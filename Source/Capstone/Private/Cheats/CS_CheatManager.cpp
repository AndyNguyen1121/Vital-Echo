// Fill out your copyright notice in the Description page of Project Settings.


#include "Cheats/CS_CheatManager.h"

#include "Notifies/NotifyStates/ANS_ActivateAttackWindow.h"

void UCS_CheatManager::InitCheatManager()
{
	Super::InitCheatManager();
}

void UCS_CheatManager::ToggleDebugHitBoxes()
{
	UANS_ActivateAttackWindow::bDebug = !UANS_ActivateAttackWindow::bDebug;
}
