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

	namespace Events
	{
		namespace Player
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(EndBlock);
		}
		
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TraceAttack);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(HitReact);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(KnockedUp);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(ParryActivate);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(ParryReceive);
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
			
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(RestrictMove);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(RestrictRotate);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(IsPerformingAction);
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
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage);
	}

	
}
