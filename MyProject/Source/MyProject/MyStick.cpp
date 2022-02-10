// Fill out your copyright notice in the Description page of Project Settings.


#include "MyStick.h"

AMyStick::AMyStick() {
	AttackPower = 5.0f;
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("STICK"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_STICK(TEXT("/Game/Weapons/Stick.Stick"));
	if (SK_STICK.Succeeded()) {
		Weapon->SetSkeletalMesh(SK_STICK.Object);
	}
}

void AMyStick::BeginPlay()
{
	Super::BeginPlay();
}

void AMyStick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
