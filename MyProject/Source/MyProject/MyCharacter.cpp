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

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -40.0f), FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 400.0f;
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
		SpringArm->TargetArmLength = 450.0f;
		SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bInheritPitch = true;
		SpringArm->bInheritRoll = true;
		SpringArm->bInheritYaw = true;
		SpringArm->bDoCollisionTest = true;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
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
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMyCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMyCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AMyCharacter::Turn);
}

void AMyCharacter::MoveForward(float NewAxisValue)
{
	AddMovementInput(GetActorForwardVector(), NewAxisValue);
}

void AMyCharacter::MoveRight(float NewAxisValue)
{
	AddMovementInput(GetActorRightVector(), NewAxisValue);
}

void AMyCharacter::LookUp(float NewAxisValue)
{
	AddControllerPitchInput(NewAxisValue);
}

void AMyCharacter::Turn(float NewAxisValue)
{
	AddControllerYawInput(NewAxisValue);
}

