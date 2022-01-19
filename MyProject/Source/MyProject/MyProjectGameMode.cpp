// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProjectGameMode.h"
#include "MyCharacter.h"
#include "MyPlayerController.h"
#include "UObject/ConstructorHelpers.h"

AMyProjectGameMode::AMyProjectGameMode()
{
	DefaultPawnClass = AMyCharacter::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass();
}


void AMyProjectGameMode::PostLogin(APlayerController* NewPlayer) {
	Super::PostLogin(NewPlayer);
}