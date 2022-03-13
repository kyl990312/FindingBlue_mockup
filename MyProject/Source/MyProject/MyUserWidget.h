// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyProject.h"
#include "Blueprint/UserWidget.h"
#include "MyUserWidget.generated.h"

UCLASS()
class MYPROJECT_API UMyUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UMyUserWidget(const FObjectInitializer& ObjectInitializer);
private:
	void SetHPRatio(float NewHP);

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(VisibleAnywhere, Category = UI)
		class UWidgetComponent* HPBarWidget;
	UPROPERTY(VisibleAnywhere)
		class UProgressBar* HPProgressBar;

	void BindPlayer(class AMyCharacter* NewPlayer);

private:
	UPROPERTY(EditAnywhere)
		class AMyCharacter* Player;
};
