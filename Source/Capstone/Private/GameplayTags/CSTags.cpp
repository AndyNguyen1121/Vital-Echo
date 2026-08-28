#include "GameplayTags/CSTags.h"

namespace CSTags
{
	UE_DEFINE_GAMEPLAY_TAG(None, "CSTags.None");

	namespace Abilities
	{
		UE_DEFINE_GAMEPLAY_TAG(ActivateOnGiven, "CSTags.Abilities.ActivateOnGiven");

	}

	namespace Events
	{
		UE_DEFINE_GAMEPLAY_TAG(TraceAttack, "CSTags.Events.TraceAttack");
		UE_DEFINE_GAMEPLAY_TAG(HitReact, "CSTags.Events.HitReact");
		UE_DEFINE_GAMEPLAY_TAG(KnockedUp, "CSTags.Events.KnockedUp");
	}

	namespace IDs
	{
		UE_DEFINE_GAMEPLAY_TAG(Enemy, "CSTags.IDs.Enemy");
		
		namespace EnemyType
		{
			UE_DEFINE_GAMEPLAY_TAG(Basic, "CSTags.IDs.Enemy.Basic");
		}
		
		UE_DEFINE_GAMEPLAY_TAG(Player, "CSTags.IDs.Player");
	}

	namespace Status
	{
		namespace GameplayStates
		{
			UE_DEFINE_GAMEPLAY_TAG(RestrictMove, "CSTags.Status.GameplayStates.RestrictMove");
			UE_DEFINE_GAMEPLAY_TAG(RestrictRotate, "CSTags.Status.GameplayStates.RestrictRotate");
			UE_DEFINE_GAMEPLAY_TAG(IsPerformingAction, "CSTags.Status.GameplayStates.IsPerformingAction");
		}
		
		namespace Debuffs
		{
			UE_DEFINE_GAMEPLAY_TAG(KnockedUp, "CSTags.Status.Debuffs.KnockedUp");
			UE_DEFINE_GAMEPLAY_TAG(Stunned, "CSTags.Status.Debuffs.Stunned");
			UE_DEFINE_GAMEPLAY_TAG(Recovering, "CSTags.Status.Debuffs.Recovering");
		}
	}

	namespace Effects
	{
		UE_DEFINE_GAMEPLAY_TAG(Damage, "CSTags.Effects.Damage");
	}
}
