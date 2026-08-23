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
		UE_DEFINE_GAMEPLAY_TAG(Player, "CSTags.IDs.Player");
	}

	namespace Status
	{
		UE_DEFINE_GAMEPLAY_TAG(KnockedUp, "CSTags.Status.KnockedUp");
		UE_DEFINE_GAMEPLAY_TAG(Stunned, "CSTags.Status.Stunned");
		UE_DEFINE_GAMEPLAY_TAG(Recovering, "CSTags.Status.Recovering");
	}

	namespace Effects
	{
		UE_DEFINE_GAMEPLAY_TAG(Damage, "CSTags.Effects.Damage");
	}
}
