// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include"MyCharacter.h"

UMyAnimInstance::UMyAnimInstance() {
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;
	IsRunning = false;
	CurrentWeapon =0;
	Dead = false;
	Aimming = false;

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
		}
	}
}

void UMyAnimInstance::PlayAttackMontage(int32 Weapon)
{
	Montage_Play(AttackMontage, 1.0f);
}

void UMyAnimInstance::StopAttackMontage()
{
	Montage_Stop(0.1f,AttackMontage);
}

void UMyAnimInstance::AimmingOnOff(bool value)
{
	Aimming = value;
}

void UMyAnimInstance::AnimNotify_HitCheck()
{
	OnHitCheck.Broadcast();
}

void UMyAnimInstance::AnimNotify_EndAttack()
{
	auto Character = Cast<AMyCharacter>(TryGetPawnOwner());
	if (Character != nullptr) Character->OnAttackEnd.Broadcast();
}

void UMyAnimInstance::SetDead(bool val)
{
	Dead = val;
}
