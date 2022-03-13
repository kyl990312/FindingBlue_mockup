// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyProject.h"
#include "AIController.h"
#include "MyEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AMyEnemyAIController : public AAIController
{
	GENERATED_BODY()
public:
	AMyEnemyAIController();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	static const FName CurrentPosKey;
	static const FName NextPosKey;
	static const FName TargetKey;

private:
	UPROPERTY()
		class UBehaviorTree* BTAsset;
	UPROPERTY()
		class UBlackboardData* BBAsset;
	
};
