// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"

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
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_PLAYER(TEXT("/Game/Character/Models/Character_Rig.Character_Rig"));
	if (SK_PLAYER.Succeeded()) {
		ABLOG(Log, TEXT("find Character_Rig skeletal mesh"));
		GetMesh()->SetSkeletalMesh(SK_PLAYER.Object);
	}

	//set animation
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> PLAYER_ANIM(TEXT("/Game/Character/Animations_Sequences/Player_Blueprint.Player_Blueprint_C"));
	if (PLAYER_ANIM.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(PLAYER_ANIM.Class);
	}

	SetControlMode(EControlMode::GTA);

	CurrentSpeedRate = DefaultSpeedRate;

	OwnWeapons.Emplace(UEWeaponType::Stick);
	CurrentWeapon = OwnWeapons[0];
	// test
	OwnWeapons.Emplace(UEWeaponType::AR);
	OwnWeapons.Emplace(UEWeaponType::Sniper);

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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
	//if (GetCharacterMovement()->IsFalling()) {
	//	ABLOG(Log,TEXT("Jump"))
	//}
	//else {
	//	ABLOG(Log,TEXT("OnGround"))
	//}
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

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMyCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMyCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AMyCharacter::Turn);
}

void AMyCharacter::MoveForward(float NewAxisValue)
{
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue * CurrentSpeedRate);
}

void AMyCharacter::MoveRight(float NewAxisValue)
{
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue * CurrentSpeedRate);
}

void AMyCharacter::LookUp(float NewAxisValue)
{
	AddControllerPitchInput(NewAxisValue);
}

void AMyCharacter::Turn(float NewAxisValue)
{
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

