// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/NotifyStates/BaseScript/ANS_CS_Base.h"

void UANS_CS_Base::BindMontageEnd(UObject* InUserObject, UAnimInstance* AnimInstance, UAnimMontage* Montage,
	TFunction<void(UAnimMontage*, bool)> Callback)
{
	if (!AnimInstance || !Montage || !InUserObject)
		return;

	if (AnimInstance->Montage_IsPlaying(Montage))
	{
		FOnMontageEnded EmptyDelegate;
		AnimInstance->Montage_SetEndDelegate(EmptyDelegate, Montage);
	}
	
	FOnMontageEnded Delegate = FOnMontageEnded::CreateWeakLambda(
		this,
		[InUserObject, Callback](UAnimMontage* EndedMontage, bool bInterrupted)
		{
			if (InUserObject)
			{
				Callback(EndedMontage, bInterrupted);
			}
		});

	AnimInstance->Montage_SetEndDelegate(Delegate, Montage);
}
