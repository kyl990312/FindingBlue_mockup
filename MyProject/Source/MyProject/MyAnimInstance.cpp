// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include"MyCharacter.h"

UMyAnimInstance::UMyAnimInstance() {
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;
	IsRunning = false;
	CurrentWeapon =0;
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn)) {
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		auto Character = Cast<AMyCharacter>(Pawn);
		if (Character) {
			IsInAir = Character->GetCharacterMovement()->IsFalling();
			IsRunning = Character->GetIsRunning();
			CurrentWeapon = Character->GetCurrentWeapon();
		}
	}
}
