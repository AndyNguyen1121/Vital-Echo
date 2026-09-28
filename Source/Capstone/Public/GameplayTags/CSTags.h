#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace CSTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(None);

	namespace Abilities
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActivateOnGiven);
	}

	namespace Input
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(PauseInputWindowActive);
	}

	namespace Events
	{
		namespace Player
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(EndBlock);
		}

		namespace Gameplay
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(TraceAttack);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(HitReact);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(KnockedUp);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(ParryActivate);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(ParryReceive);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(PostureBroken);
		}
	}

	namespace IDs
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy);
		
		namespace EnemyType
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Basic);
		}
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player);
	}

	namespace Status
	{
		namespace GameplayStates
		{
			namespace PlayerStates
			{
				UE_DECLARE_GAMEPLAY_TAG_EXTERN(ParryWindowActive);
				UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputWindowActive);
			}

			namespace EnemyStates
			{
				UE_DECLARE_GAMEPLAY_TAG_EXTERN(CanBeExecuted);
			}
			
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(RestrictBlocking);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(RestrictMove);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(RestrictRotate);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(IsPerformingAction);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(IsInScriptedAnimation);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(PostureRegenSupressed);
		}

		namespace Debuffs
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(KnockedUp);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Stunned);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Recovering);
		}
	}

	namespace Effects
	{
		namespace SetByCaller
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(PostureSuppressDuration);
		}
		
	}

	
}
