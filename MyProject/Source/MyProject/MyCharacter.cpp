// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "MyStick.h"
#include "MyGun.h"
#include "MyAnimInstance.h"
#include "MyEnemyAIController.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// set camera
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SRPINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	GetCapsuleComponent()->SetCapsuleHalfHeight(43.5f);
	GetCapsuleComponent()->SetCapsuleRadius(15.0f);	
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -45.0f), FRotator(0.0f, -90.0f, 0.0f));

	SpringArm->TargetArmLength = 00.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));
	CurrentArmLength = 0.0f;

	// set mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_PLAYER(TEXT("/Game/Character/Character_Rig.Character_Rig"));
	if (SK_PLAYER.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SK_PLAYER.Object);
	}
	
	//set animation
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> PLAYER_ANIM(TEXT("/Game/Character/Player_Blueprint.Player_Blueprint_C"));
	if (PLAYER_ANIM.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(PLAYER_ANIM.Class);
	}

	SetControlMode(EControlMode::COMMON);

	CurrentSpeedRate = DefaultSpeedRate;

	AIControllerClass = AMyEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	CurrentWeapon = UEWeaponType::None;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	InitializeWeapons();

	if (GetController()->IsPlayerController()) {
		AddNewWeapon(UEWeaponType::Stick);
		//test
		AddNewWeapon(UEWeaponType::AR);
		AddNewWeapon(UEWeaponType::Sniper);

	}

	if (OwnWeapons.Num() > 0) {
		CurrentWeapon = OwnWeapons[0];
		ABLOG(Warning, TEXT("Set Weapon: %s"), *GetName());
	}

	if(CurrentWeapon != UEWeaponType::None)
		SetEnableWeapon(Weapons[(int32)CurrentWeapon - 1], true);
}

void AMyCharacter::SetControlMode(EControlMode ControlMode)
{
	CurrentControlMode = ControlMode;
	SpringArm->TargetArmLength = CurrentArmLength;
	SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bDoCollisionTest = true;
	bUseControllerRotationYaw = false;

	switch (CurrentControlMode)
	{
	case EControlMode::COMMON:
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, RotateRate, 0.0f);
		CurrentArmLength = 300.0f;
		break;
	case EControlMode::AR_AIM:
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, RotateRate, 0.0f);
		bUseControllerRotationYaw = true;
		CurrentArmLength = -200.0f;
		break;
	case EControlMode::SNIPER_AIM:
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, RotateRate, 0.0f);
		bUseControllerRotationYaw = true;
		CurrentArmLength = -200.0f;
		break;
	case EControlMode::NPC:
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
		break;
	}
	SpringArm->TargetArmLength = CurrentArmLength;
	ArmLengthTo = CurrentArmLength;
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CurrentArmLength = FMath::FInterpTo(CurrentArmLength, ArmLengthTo, DeltaTime, ArmLengthSpeed);
	SpringArm->TargetArmLength = CurrentArmLength;
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// bind input
	PlayerInputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Pressed, this, &AMyCharacter::RunStart);
	PlayerInputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Released, this, &AMyCharacter::RunEnd);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMyCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Weapon"), EInputEvent::IE_Pressed, this, &AMyCharacter::ChangeWeapon);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMyCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Released, Anim, &UMyAnimInstance::StopAttackMontage);
	PlayerInputComponent->BindAction(TEXT("Aim"), EInputEvent::IE_Pressed, this, &AMyCharacter::Aim);


	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMyCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMyCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AMyCharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("Zoom"), this, &AMyCharacter::Zoom);
}

void AMyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Anim = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
	if (Anim != nullptr) {
		Anim->OnMontageEnded.AddDynamic(this, &AMyCharacter::AttackEnd);
		Anim->OnHitCheck.AddLambda([this]()->void {
			if (CurrentWeapon != UEWeaponType::None) {
				Weapons[(int32)CurrentWeapon-1]->Attack();
			}
		});
	}
}

float AMyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = DamageAmount;
	// if damaged at head(only gun), finalDamage = CurrentHp
	if (CurrentHp > 0) {
		CurrentHp -= FinalDamage;
		//OnHPChanged.Broadcast(MaxHp < KINDA_SMALL_NUMBER ? 0.0f : (CurrentHp / MaxHp));
		ABLOG(Warning, TEXT("Damaged: %f"), FinalDamage);
		ABLOG(Warning, TEXT("HP: %f / %f"), CurrentHp, MaxHp);
	}
	else {
		SetActorEnableCollision(false);
		Anim->SetDead(true);
	}

	return FinalDamage;
}

void AMyCharacter::PossessedBy(AController* NewContoller)
{
	Super::PossessedBy(NewContoller);

	if (IsPlayerControlled()) {
		SetControlMode(EControlMode::COMMON);
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
		MaxHp = 500.0f;
	}
	else {
		SetControlMode(EControlMode::NPC);
		GetCharacterMovement()->MaxWalkSpeed = 60.0f;
	}

	CurrentHp = MaxHp;
}

void AMyCharacter::MoveForward(float NewAxisValue)
{
	//if (IsAttacking) return;
	if (CurrentHp < 0) return;
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue * CurrentSpeedRate);
}

void AMyCharacter::MoveRight(float NewAxisValue)
{
	//if (IsAttacking) return;
	if (CurrentHp < 0) return;
	switch (CurrentControlMode)
	{
	case EControlMode::COMMON:
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue * CurrentSpeedRate);
		break;
	case EControlMode::AR_AIM:
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue * CurrentSpeedRate);
		break;
	case EControlMode::SNIPER_AIM:
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue * CurrentSpeedRate);
		break;
	}
}

void AMyCharacter::LookUp(float NewAxisValue)
{
	if (CurrentHp < 0) return;
	AddControllerPitchInput(NewAxisValue);
}

void AMyCharacter::Turn(float NewAxisValue)
{
	if (CurrentHp < 0) return;
	AddControllerYawInput(NewAxisValue);
}

void AMyCharacter::RunStart()
{
	CurrentSpeedRate = RunSpeedRate;
}

void AMyCharacter::RunEnd()
{
	CurrentSpeedRate = DefaultSpeedRate;
}

void AMyCharacter::ChangeWeapon(FKey key)
{
	// hide current weapon
	ABLOG(Warning,TEXT("Pre Weapon(int) : %d"), (int32)CurrentWeapon)
	if (Weapons[(int32)CurrentWeapon-1] != nullptr) {
		SetEnableWeapon(Weapons[(int32)CurrentWeapon - 1], false);
	}

	// set current weapon type
	FName keyName = key.GetFName();
	if (keyName == FName(TEXT("One"))) {
		CurrentWeapon = OwnWeapons[0];
	}
	else if (keyName == FName(TEXT("Two")) && OwnWeapons.Num() >= 2) {
		CurrentWeapon = OwnWeapons[1];
	}
	else if (keyName == FName(TEXT("Three")) && OwnWeapons.Num() >= 3) {
		CurrentWeapon = OwnWeapons[2];
	}

	if (Aimming && CurrentWeapon == UEWeaponType::Stick) {
		Aim();
	}

	//// show new weapon
	if (Weapons[(int32)CurrentWeapon-1] != nullptr) {
		SetEnableWeapon(Weapons[(int32)CurrentWeapon - 1], true);
	}
	ABLOG(Warning, TEXT("Pre Weapon(int) : %d"), (int32)CurrentWeapon)
}

bool AMyCharacter::GetCurrentWeaponObject(AMyWeapon* Weapon)
{
	if (CurrentWeapon != UEWeaponType::None) {
		Weapon = Weapons[(int32)CurrentWeapon - 1];
		return true;
	}
	return false;
}

void AMyCharacter::Attack()
{
	if (CurrentHp < 0) return;
	if (!IsAttacking) {
		IsAttacking = true;
		Anim->PlayAttackMontage((int32)CurrentWeapon);
	}
}

void AMyCharacter::Aim()
{
	if (CurrentWeapon == UEWeaponType::Stick || CurrentWeapon == UEWeaponType::None) return;
	Aimming = !Aimming;
	if (Aimming) {
		switch (CurrentWeapon)
		{
		case UEWeaponType::AR:
			SetControlMode(EControlMode::AR_AIM);
			break;
		case UEWeaponType::Sniper:
			SetControlMode(EControlMode::SNIPER_AIM);
			break;
		default:
			break;
		}
	}
	else {
		SetControlMode(EControlMode::COMMON);
	}
	Anim->AimmingOnOff(Aimming);
}

void AMyCharacter::Zoom(float NewAxisValue)
{
	if (Aimming) return;
	ArmLengthTo = FMath::Clamp(ArmLengthTo + NewAxisValue * ArmLengthTo, MinArmLength, MaxArmLength);
}


void AMyCharacter::AttackEnd(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttacking = false;
	OnAttackEnd.Broadcast();
}

void AMyCharacter::InitializeWeapons()
{
	// create new weapons and set weapons to socekt
	for (int i = 1; i < 4; ++i) {
		AMyWeapon* NewWeapon = nullptr;
		FName Socket(TEXT(""));
		TSubclassOf<class AMyWeapon> WeaponClass;

		switch ((UEWeaponType)i)
		{
		case UEWeaponType::Stick:

			WeaponClass = AMyStick::StaticClass();
			NewWeapon = GetWorld()->SpawnActor<AMyStick>(WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator);
			Socket = TEXT("Stick-rSocket");
			break;
		case UEWeaponType::Sniper:
		case UEWeaponType::AR:
			WeaponClass = AMyGun::StaticClass();
			NewWeapon = GetWorld()->SpawnActor<AMyGun>(WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator);
			Socket = TEXT("Gun-rSocket");
			break;
		default:
			break;
		}

		if (NewWeapon != nullptr) {
			NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, Socket);
			NewWeapon->SetOwner(this);
			SetEnableWeapon(NewWeapon,false);
			Weapons.Add(NewWeapon);
		}
	}
}

void AMyCharacter::SetEnableWeapon(AMyWeapon* Target, bool Enable)
{
	Target->SetActorEnableCollision(Enable);
	Target->SetActorHiddenInGame(!Enable);
	if (Anim != nullptr && Enable) {
		Anim->AimmingOnOff(false);
	}
}


bool AMyCharacter::GetIsRunning()
{
	return RunSpeedRate == CurrentSpeedRate;
}

int AMyCharacter::GetCurrentWeapon()
{
	if (CurrentWeapon == UEWeaponType::Stick) {
		return 1;
	}
	else if (CurrentWeapon == UEWeaponType::AR || CurrentWeapon == UEWeaponType::Sniper) {
		return 2;
	}
	return 0;
}

void AMyCharacter::AddNewWeapon(UEWeaponType NewWeaponType)
{
	OwnWeapons.Add(NewWeaponType);
}

void AMyCharacter::GetAttackStartForwardVector(FVector& Start, FVector& Forward)
{
	if (Aimming) {
		Start = Camera->GetComponentLocation();
		Forward = Camera->GetForwardVector();
	}
	else {
		Start = GetActorLocation();
		Forward = GetActorForwardVector();
	}
}







