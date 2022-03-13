// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyProject.h"
#include "GameFramework/Actor.h"
#include "MyWeapon.generated.h"

UCLASS()
class MYPROJECT_API AMyWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyWeapon();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void Attack();

	virtual float GetAttackRange();

protected:
	UPROPERTY(VisibleAnywhere, Category=Weapon)
	float AttackPower;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRange;

public:
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	USkeletalMeshComponent* Weapon;

};
