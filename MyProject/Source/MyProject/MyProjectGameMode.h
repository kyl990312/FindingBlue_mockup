// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MyProject.h"
#include "GameFramework/GameModeBase.h"
#include "MyProjectGameMode.generated.h"

UCLASS(minimalapi)
class AMyProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMyProjectGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;
};



