// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGun.h"
#include "DrawDebugHelpers.h"

AMyGun::AMyGun() {
	AttackPower = 100.0f;
	AttackRange = 500.0f;
	BulletRadius = 5.0f;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("STICK"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_STICK(TEXT("/Game/Weapons/Gun_1.Gun_1"));
	if (SK_STICK.Succeeded()) {
		Weapon->SetSkeletalMesh(SK_STICK.Object);
	}
}

void AMyGun::BeginPlay()
{
	Super::BeginPlay();
}

void AMyGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyGun::Attack()
{
	AActor* Parent = GetOwner();
	if (Parent != nullptr) {
		FHitResult Result;
		FCollisionQueryParams Params(NAME_None, false, this);
		Params.AddIgnoredActor(Parent);

		FVector Forward = Parent->GetActorForwardVector();
		FVector Start = Parent->GetActorLocation();
		bool bResult = GetWorld()->SweepSingleByChannel(Result, Start, Start + Forward * AttackRange, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel12, FCollisionShape::MakeSphere(BulletRadius), Params);

#if ENABLE_DRAW_DEBUG
		FVector TraceVec = Forward * AttackRange;
		FVector Center = Start + Forward * AttackRange / 2;
		float HalfHeight = AttackRange * 0.5f + BulletRadius;
		FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
		FColor DrawColor = bResult ? FColor::Green : FColor::Red;
		float DebugLifeTime = 0.5f;

		DrawDebugCapsule(
			GetWorld(),
			Center,
			HalfHeight,
			BulletRadius,
			CapsuleRot,
			DrawColor,
			true,
			DebugLifeTime
		);
#endif

		if (bResult) {
			if (Result.Actor.IsValid()) {
				ABLOG(Warning, TEXT("Hit : %s"), *Result.Actor->GetName());
				UGameplayStatics::ApplyDamage(Cast<AActor>(Result.Actor), AttackPower, NULL, Parent, NULL);
			}
		}
	}
}
