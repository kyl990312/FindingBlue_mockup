// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProjectGameMode.h"
#include "MyCharacter.h"
#include "MyPlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

AMyProjectGameMode::AMyProjectGameMode()
{
	DefaultPawnClass = AMyCharacter::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass();
}


void AMyProjectGameMode::ChangeWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (CurrentWidget != nullptr) {
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}

	if (NewWidgetClass != nullptr) {
		CurrentWidget = CreateWidget(GetWorld(),NewWidgetClass);
		if (CurrentWidget != nullptr) {
			CurrentWidget->AddToViewport();
		}
	}
}

void AMyProjectGameMode::PostLogin(APlayerController* NewPlayer) {
	Super::PostLogin(NewPlayer);

	//if (NewPlayer != nullptr)
	//	ABLOG(Log,TEXT("Login New Player : %s"),*NewPlayer->GetName());
}

void AMyProjectGameMode::BeginPlay()
{
	Super::BeginPlay();
	ChangeWidget(StartingWidgetClass);
}
