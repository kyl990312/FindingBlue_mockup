// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName AMyEnemyAIController::CurrentPosKey(TEXT("CurrentPos"));
const FName AMyEnemyAIController::NextPosKey(TEXT("NextPos"));
const FName AMyEnemyAIController::TargetKey(TEXT("Target"));

AMyEnemyAIController::AMyEnemyAIController() {
	static ConstructorHelpers::FObjectFinder<UBlackboardData>BBObject(TEXT("/Game/AI/BB_MyCharacter.BB_MyCharacter"));
	if (BBObject.Succeeded()) {
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree>BTObject(TEXT("/Game/AI/BT_MyCharacter.BT_MyCharacter"));
	if (BTObject.Succeeded()) {
		BTAsset = BTObject.Object;
	}
}

void AMyEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (UseBlackboard(BBAsset, Blackboard)) {
		ABLOG(Log, TEXT("Use Blackboard"));
		Blackboard->SetValueAsVector(CurrentPosKey, InPawn->GetActorLocation());
		if (!RunBehaviorTree(BTAsset)) {
			ABLOG(Error, TEXT("AIController couldn't run behavior tree"));
		}
	}
}

void AMyEnemyAIController::OnUnPossess()
{
	Super::OnUnPossess();
}
