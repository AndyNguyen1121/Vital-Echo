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
	}

	namespace IDs
	{
		UE_DEFINE_GAMEPLAY_TAG(Enemy, "CSTags.IDs.Enemy");
		UE_DEFINE_GAMEPLAY_TAG(Player, "CSTags.IDs.Player");
	}

	namespace Status
	{
		UE_DEFINE_GAMEPLAY_TAG(KnockedUp, "CSTags.Status.KnockedUp");
	}
}
