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

	void ChangeWidget(TSubclassOf<class UUserWidget> NewWidgetClass);

	virtual void PostLogin(APlayerController* NewPlayer) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
		TSubclassOf<class UUserWidget> StartingWidgetClass;
	UPROPERTY(VisibleAnywhere)
		class UUserWidget* CurrentWidget = nullptr;
	
};



