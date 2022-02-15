// Fill out your copyright notice in the Description page of Project Settings.


#include "MyStick.h"
#include "DrawDebugHelpers.h"

AMyStick::AMyStick() {
	AttackPower = 50.0f;
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("STICK"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_STICK(TEXT("/Game/Weapons/Stick.Stick"));
	if (SK_STICK.Succeeded()) {
		Weapon->SetSkeletalMesh(SK_STICK.Object);
	}
	Weapon->SetCollisionProfileName(TEXT("Weapon"));

	AttackRange = 30.0f;
	AttackRadius = 40.0f;
}

void AMyStick::Attack()
{

	AActor* Parent = GetOwner();
	if (Parent != nullptr) {
		FHitResult Result;
		FCollisionQueryParams Params(NAME_None, false, this);
		Params.AddIgnoredActor(Parent);

		FVector Forward = Parent->GetActorForwardVector();
		FVector Start = Parent->GetActorLocation();
		bool bResult = GetWorld()->SweepSingleByChannel(Result, Start, Start + Forward * AttackRange, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel12, FCollisionShape::MakeSphere(AttackRadius), Params);

#if ENABLE_DRAW_DEBUG
		FVector TraceVec = Forward * AttackRange;
		FVector Center = Start + Forward * AttackRange / 2;
		float HalfHeight = AttackRange * 0.5f + AttackRadius;
		FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
		FColor DrawColor = bResult ? FColor::Green : FColor::Red;
		float DebugLifeTime = 0.5f;

		DrawDebugCapsule(
			GetWorld(),
			Center,
			HalfHeight,
			AttackRadius,
			CapsuleRot,
			DrawColor,
			true,
			DebugLifeTime
		);
#endif

		if (bResult) {
			if (Result.Actor.IsValid()) {
				ABLOG(Warning, TEXT("Hit : %s"),*Result.Actor->GetName());
				UGameplayStatics::ApplyDamage(Cast<AActor>(Result.Actor), AttackPower, NULL, Parent, NULL);
			}
		}
	}
}

void AMyStick::BeginPlay()
{
	Super::BeginPlay();
}

void AMyStick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
