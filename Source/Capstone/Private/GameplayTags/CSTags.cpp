#include "GameplayTags/CSTags.h"

namespace CSTags
{
	UE_DEFINE_GAMEPLAY_TAG(None, "CSTags.None");

	namespace Abilities
	{
		UE_DEFINE_GAMEPLAY_TAG(ActivateOnGiven, "CSTags.Abilities.ActivateOnGiven");

	}

	namespace Input
	{
		UE_DEFINE_GAMEPLAY_TAG(PauseInputWindowActive, "CSTags.Input.PauseInputWindowActive");
	}

	namespace Events
	{
		namespace Player
		{
			UE_DEFINE_GAMEPLAY_TAG(EndBlock, "CSTags.Events.Player.EndBlock");
		}

		namespace Gameplay
		{
			UE_DEFINE_GAMEPLAY_TAG(TraceAttack, "CSTags.Events.Gameplay.TraceAttack");
			UE_DEFINE_GAMEPLAY_TAG(HitReact, "CSTags.Events.Gameplay.HitReact");
			UE_DEFINE_GAMEPLAY_TAG(KnockedUp, "CSTags.Events.Gameplay.KnockedUp");
			UE_DEFINE_GAMEPLAY_TAG(ParryActivate, "CSTags.Events.Gameplay.ParryActivate");
			UE_DEFINE_GAMEPLAY_TAG(ParryReceive, "CSTags.Events.Gameplay.ParryReceive");
			UE_DEFINE_GAMEPLAY_TAG(PostureBroken, "CSTags.Events.Gameplay.PostureBroken");
		}
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
			namespace PlayerStates
			{
				UE_DEFINE_GAMEPLAY_TAG(ParryWindowActive, "CSTags.Status.GameplayStates.PlayerStates.ParryWindowActive");
				UE_DEFINE_GAMEPLAY_TAG(InputWindowActive, "CSTags.Status.GameplayStates.PlayerStates.InputWindowActive");
				
			}

			namespace EnemyStates
			{
				UE_DEFINE_GAMEPLAY_TAG(CanBeExecuted, "CSTags.Status.GameplayStates.EnemyStates.CanBeExecuted");
			}

			UE_DEFINE_GAMEPLAY_TAG(IsBlocking, "CSTags.Status.GameplayStates.IsBlocking");
			UE_DEFINE_GAMEPLAY_TAG(RestrictBlocking, "CSTags.Status.GameplayStates.RestrictBlocking");
			UE_DEFINE_GAMEPLAY_TAG(RestrictMove, "CSTags.Status.GameplayStates.RestrictMove");
			UE_DEFINE_GAMEPLAY_TAG(RestrictRotate, "CSTags.Status.GameplayStates.RestrictRotate");
			UE_DEFINE_GAMEPLAY_TAG(IsPerformingAction, "CSTags.Status.GameplayStates.IsPerformingAction");
			UE_DEFINE_GAMEPLAY_TAG(IsInScriptedAnimation, "CSTags.Status.GameplayStates.IsInScriptedAnimation");
			UE_DEFINE_GAMEPLAY_TAG(PostureRegenSupressed, "CSTags.Status.GameplayStates.PostureRegenSuppressed");
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
		namespace SetByCaller
		{
			UE_DEFINE_GAMEPLAY_TAG(Damage, "CSTags.Effects.SetByCaller.Damage");
			UE_DEFINE_GAMEPLAY_TAG(PostureDamage, "CSTags.Effects.SetByCaller.PostureDamage");
		}
	}
}
