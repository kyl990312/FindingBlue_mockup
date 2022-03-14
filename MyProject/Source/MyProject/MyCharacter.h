// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyProject.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

//DECLARE_MULTICAST_DELEGATE_OneParam(FOnHPChanged,float);

UENUM(BlueprintType)
enum class UEWeaponType : uint8 {
	None = 0 UMETA(DisplayName = "None"),
	Stick  = 1 UMETA(DisplayName = "Stick"),
	AR = 2 UMETA(DisplayName = "AR"),
	Sniper = 3 UMETA(DisplayName = "Sniper"),
};

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

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
		COMMON,
		AR_AIM,
		SNIPER_AIM,
		NPC,
	};

	// Called when the game starts or when spawned
	void SetControlMode(EControlMode ControlMode);
	EControlMode CurrentControlMode = EControlMode::COMMON;

	void InitializeWeapons();
	void SetEnableWeapon(class AMyWeapon* Target, bool Enable);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void PossessedBy(AController* NewContoller) override;


	bool GetIsRunning();
	int GetCurrentWeapon();
	void AddNewWeapon(UEWeaponType NewWeaponType);
	void GetAttackStartForwardVector(FVector& Start, FVector& Forward);

	UFUNCTION()
		bool GetCurrentWeaponObject(class AMyWeapon* Weapon);
	
	void Attack();
	void AttackEnd();
	FOnAttackEndDelegate OnAttackEnd;

	void RunStart();
	void RunEnd();

	UPROPERTY(VisibleAnyWhere, Category = Camera)
		USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnyWhere, Category = Camera)
		UCameraComponent* Camera;

	//FOnHPChanged OnHPChanged;

private:
	void MoveForward(float NewAxisValue);
	void MoveRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);  
	void ChangeWeapon(FKey key);
	void Aim();
	void Zoom(float NewAxisValue);

	UFUNCTION()
	void AttackEnd(UAnimMontage* Montage, bool bInterrupted);

	const float DefaultSpeedRate = 0.1f;
	const float RunSpeedRate = 0.3f;
	const float RotateRate = 500.0f;
	const float MinArmLength =100.0f;
	const float MaxArmLength = 300.0f;
	const float ArmLengthSpeed = 10.0f;

	float CurrentSpeedRate = 0.0f;
	bool JumpInput = false;
	bool IsAttacking = false;
	float MaxHp = 100.0f;
	float CurrentHp;
	bool Aimming = false;
	float CurrentArmLength = 0.0f;
	float ArmLengthTo = 0.0f;
	
	UEWeaponType CurrentWeapon = UEWeaponType::None;
	UPROPERTY(EditAnywhere, Category=Weapon)
	TArray<UEWeaponType> OwnWeapons;
	TArray<class AMyWeapon*> Weapons;

	UPROPERTY()
	class UMyAnimInstance* Anim;
};
