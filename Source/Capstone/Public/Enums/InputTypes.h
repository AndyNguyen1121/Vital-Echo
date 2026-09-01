#pragma once

UENUM(BlueprintType)
enum class EInputType : uint8
{
	EIT_Primary UMETA(DisplayName = "Primary"),
	EIT_Jump UMETA(DisplayName = "Jump"),
	EIT_Block UMETA(DisplayName = "Block"),

	EIT_Pause UMETA(DisplayName = "Pause"),
	
	EIT_Forward UMETA(DisplayName = "Forward"),
	EIT_Back UMETA(DisplayName = "Back"),
	EIT_Neutral UMETA(DisplayName = "Neutral"),
	
	EIT_Left UMETA(Hidden, DisplayName = "Left"),
	EIT_Right UMETA(Hidden, DisplayName = "Right"),
	EIT_Up UMETA(Hidden, DisplayName = "Up"),
	EIT_Down UMETA(Hidden, DisplayName = "Down"),
};
