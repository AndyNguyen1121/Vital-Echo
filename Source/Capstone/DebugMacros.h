#pragma once
#include "DrawDebugHelpers.h"

#define DEBUG_LOG(TEXT) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, TEXT);
#define DEBUG_ERR(TEXT) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT);
#define DEBUG_WARN(TEXT) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT);
#define DRAW_SPHERE(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 20, 12, FColor::Red, true);
#define CONSOLE_LOG(Str) UE_LOG(LogTemp, Log, TEXT("%s"), *Str)
#define CONSOLE_WARN(Str) UE_LOG(LogTemp, Warning, TEXT("%s"), *Str)
#define CONSOLE_ERR(Str) UE_LOG(LogTemp, Error, TEXT("%s"), *Str)