// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "MyStick.h"
#include "MyGun.h"
#include "MyAnimInstance.h"

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

	GetCapsuleComponent()->SetCapsuleHalfHeight(45.0f);
	GetCapsuleComponent()->SetCapsuleRadius(15.0f);	
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -45.0f), FRotator(0.0f, -90.0f, 0.0f));

	SpringArm->TargetArmLength = 0.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

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

	SetControlMode(EControlMode::GTA);

	CurrentSpeedRate = DefaultSpeedRate;
	CurrentHp = MaxHp;

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{

	Super::BeginPlay();
	InitializeWeapons();

	AddNewWeapon(UEWeaponType::Stick);
	CurrentWeapon = OwnWeapons[0];
	//test
	AddNewWeapon(UEWeaponType::AR);
	AddNewWeapon(UEWeaponType::Sniper);
}

void AMyCharacter::SetControlMode(EControlMode ControlMode)
{
	CurrentControlMode = ControlMode;
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		SpringArm->TargetArmLength = 300.0f;
		SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bInheritPitch = true;
		SpringArm->bInheritRoll = true;
		SpringArm->bInheritYaw = true;
		SpringArm->bDoCollisionTest = true;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, RotateRate, 0.0f);
		break;
	}
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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


	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMyCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMyCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AMyCharacter::Turn);
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
	if (CurrentHp > 0) {
		CurrentHp -= FinalDamage;
	}
	else {
		SetActorEnableCollision(false);
	}

	return FinalDamage;
}

void AMyCharacter::MoveForward(float NewAxisValue)
{
	if (IsAttacking) return;
	if (CurrentHp < 0) return;
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue * CurrentSpeedRate);
}

void AMyCharacter::MoveRight(float NewAxisValue)
{
	if (IsAttacking) return;
	if (CurrentHp < 0) return;
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue * CurrentSpeedRate);
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

	//// show new weapon
	if (Weapons[(int32)CurrentWeapon-1] != nullptr) {
		SetEnableWeapon(Weapons[(int32)CurrentWeapon - 1], true);
	}
	ABLOG(Warning, TEXT("Pre Weapon(int) : %d"), (int32)CurrentWeapon)
}

void AMyCharacter::Attack()
{
	if (CurrentHp < 0) return;
	if (!IsAttacking) {
		IsAttacking = true;
		Anim->PlayAttackMontage((int32)CurrentWeapon-1);
	}
}

void AMyCharacter::AttackEnd(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttacking = false;
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

	SetEnableWeapon(Weapons[0], true);
}

void AMyCharacter::SetEnableWeapon(AMyWeapon* Target, bool Enable)
{
	Target->SetActorEnableCollision(Enable);
	Target->SetActorHiddenInGame(!Enable);
}


bool AMyCharacter::GetIsRunning()
{
	return RunSpeedRate == CurrentSpeedRate;
}

int AMyCharacter::GetCurrentWeapon()
{
	if (CurrentWeapon == UEWeaponType::Stick) {
		return 0;
	}
	else if (CurrentWeapon == UEWeaponType::AR || CurrentWeapon == UEWeaponType::Sniper) {
		return 1;
	}
	return -1;
}

void AMyCharacter::AddNewWeapon(UEWeaponType NewWeaponType)
{
	OwnWeapons.Add(NewWeaponType);
}

bool AMyCharacter::GetDead()
{
	return CurrentHp<=0;
}



