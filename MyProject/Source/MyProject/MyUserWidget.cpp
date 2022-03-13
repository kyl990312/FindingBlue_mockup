// Fill out your copyright notice in the Description page of Project Settings.


#include "MyUserWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/ProgressBar.h"
#include "MyCharacter.h"


UMyUserWidget::UMyUserWidget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer) {
	//HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("CharacterHPBar"));

	//static ConstructorHelpers::FClassFinder<UUserWidget> HPBAR_WIDGET(TEXT("/Game/UI/CharacterUI.CharacterUI_C"));
	//if (HPBAR_WIDGET.Succeeded()) {
	//	HPBarWidget->SetWidgetClass(HPBAR_WIDGET.Class);
	//}
}

void UMyUserWidget::SetHPRatio(float NewHP)
{
	if (HPProgressBar != nullptr) {
		HPProgressBar->SetPercent(NewHP);
	}
}

void UMyUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (HPBarWidget != nullptr) {
		HPBarWidget->InitWidget();
		auto WidgetObject = HPBarWidget->GetUserWidgetObject();
		if (WidgetObject != nullptr) {
			HPProgressBar = Cast<UProgressBar>(WidgetObject->GetWidgetFromName(
				TEXT("HPProgressBar")
			));
		}
	}

	auto Controller = GetWorld()->GetFirstPlayerController();
	if (Controller != nullptr) {
		ABLOG(Log, TEXT("[Widget] GetContorller: %s"), *Controller->GetName());
		auto NewPlayer = Cast<AMyCharacter>(Controller->GetPawn());
		if (NewPlayer != nullptr) {
			BindPlayer(NewPlayer);
		}
	}
}

void UMyUserWidget::BindPlayer(AMyCharacter* NewPlayer)
{
	if (NewPlayer != nullptr) {
		Player = NewPlayer;
		ABLOG(Warning, TEXT("[Widget] BindPlayer : %s"), *Player->GetName());
		//Player->OnHPChanged.AddUObject(this,&UMyUserWidget::SetHPRatio);
	}
}

