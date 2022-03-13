// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyProject.h"
#include "MyWeapon.h"
#include "MyGun.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AMyGun : public AMyWeapon
{
	GENERATED_BODY()

public:
	AMyGun();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void Attack() override;

	virtual float GetAttackRange() override;

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float BulletRadius;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float BulletLifeTime;
};
