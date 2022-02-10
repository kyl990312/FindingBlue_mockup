// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyProject.h"
#include "MyWeapon.h"
#include "MyStick.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AMyStick : public AMyWeapon
{
	GENERATED_BODY()
public:
	AMyStick();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;	
};
