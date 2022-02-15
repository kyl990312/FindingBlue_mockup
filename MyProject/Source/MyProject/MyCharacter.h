// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyProject.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UENUM(BlueprintType)
enum class UEWeaponType : uint8 {
	None = 0 UMETA(DisplayName = "None"),
	Stick  = 1 UMETA(DisplayName = "Stick"),
	AR = 2 UMETA(DisplayName = "AR"),
	Sniper = 3 UMETA(DisplayName = "Sniper"),
};

UCLASS()
class MYPROJECT_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	virtual void BeginPlay() override;

	enum class EControlMode {
		FPS,
		GTA,
	};

	// Called when the game starts or when spawned
	void SetControlMode(EControlMode ControlMode);
	EControlMode CurrentControlMode = EControlMode::GTA;

	void InitializeWeapons();
	void SetEnableWeapon(class AMyWeapon* Target, bool Enable);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;


	bool GetIsRunning();
	int GetCurrentWeapon();
	void AddNewWeapon(UEWeaponType NewWeaponType);
	bool GetDead();

	UPROPERTY(VisibleAnyWhere, Category = Camera)
		USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnyWhere, Category = Camera)
		UCameraComponent* Camera;

private:
	void MoveForward(float NewAxisValue);
	void MoveRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);
	void RunStart();
	void RunEnd();        
	void ChangeWeapon(FKey key);
	void Attack();

	UFUNCTION()
	void AttackEnd(UAnimMontage* Montage, bool bInterrupted);

	const float DefaultSpeedRate = 0.1f;
	const float RunSpeedRate = 0.3f;
	const float RotateRate = 200.0f;
	float CurrentSpeedRate = 0.0f;
	bool JumpInput = false;
	bool IsAttacking = false;
	const float MaxHp = 100.0f;
	float CurrentHp;
	
	UEWeaponType CurrentWeapon = UEWeaponType::None;
	UPROPERTY(VisibleAnyWhere, Category=Weapon)
	TArray<UEWeaponType> OwnWeapons;
	TArray<class AMyWeapon*> Weapons;

	UPROPERTY()
	class UMyAnimInstance* Anim;
};
