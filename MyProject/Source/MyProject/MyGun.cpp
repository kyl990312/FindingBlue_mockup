// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGun.h"

AMyGun::AMyGun() {
	AttackPower = 5.0f;
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("STICK"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_STICK(TEXT("/Game/Weapons/Gun_1.Gun_1"));
	if (SK_STICK.Succeeded()) {
		Weapon->SetSkeletalMesh(SK_STICK.Object);
	}
}

void AMyGun::BeginPlay()
{
	Super::BeginPlay();
}

void AMyGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
