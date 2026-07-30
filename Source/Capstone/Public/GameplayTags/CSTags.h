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
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TraceAttack);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(HitReact);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(KnockedUp);
	}

	namespace IDs
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player);
	}

	namespace Status
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(KnockedUp);
	}
}
