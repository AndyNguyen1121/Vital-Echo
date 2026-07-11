// Fill out your copyright notice in the Description page of Project Settings.\

#include "Utils/CS_BlueprintLibrary.h"

#include "DefaultLevelSequenceInstanceData.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlayer.h"

void UCS_BlueprintLibrary::PlayLevelSequenceWithOriginTransform(AActor* ContextActor, ULevelSequence* LevelSequence,
	FTransform OriginTransform)
{
	UWorld* World = GEngine->GetWorldFromContextObject(ContextActor, EGetWorldErrorMode::LogAndReturnNull);
	if (!World) return;
	
	ALevelSequenceActor* SequenceActor = nullptr;
	FMovieSceneSequencePlaybackSettings Settings;
	ULevelSequencePlayer* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(World, LevelSequence, Settings, SequenceActor);
	
	if (!IsValid(SequenceActor)) return;

	SequenceActor->bOverrideInstanceData = true;

	if (UDefaultLevelSequenceInstanceData* InstanceData = Cast<UDefaultLevelSequenceInstanceData>(SequenceActor->DefaultInstanceData))
	{
		InstanceData->TransformOrigin = OriginTransform;
	}

	SequencePlayer->Play();
}
