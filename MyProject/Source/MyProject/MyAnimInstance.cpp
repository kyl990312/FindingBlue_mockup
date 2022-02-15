// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include"MyCharacter.h"

UMyAnimInstance::UMyAnimInstance() {
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;
	IsRunning = false;
	CurrentWeapon =0;

	static ConstructorHelpers::FObjectFinder<UAnimMontage>ATTACK_MONTAGE(TEXT("/Game/Character/AttackMontage.AttackMontage"));
	if (ATTACK_MONTAGE.Succeeded()) {
		AttackMontage = ATTACK_MONTAGE.Object;
	}
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
			Dead = Character->GetDead();
		}
	}
}

void UMyAnimInstance::PlayAttackMontage(int32 Weapon)
{
	Montage_Play(AttackMontage, 1.0f);
	switch (Weapon)
	{
	case 0:
		Montage_JumpToSection(FName(TEXT("StickAttack")), AttackMontage);
		break;
	case 1:
		Montage_JumpToSection(FName(TEXT("GunAttackSlow")), AttackMontage);
		break;
	}

}

void UMyAnimInstance::AnimNotify_HitCheck()
{
	OnHitCheck.Broadcast();
}
