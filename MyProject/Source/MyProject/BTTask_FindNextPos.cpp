// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindNextPos.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "MyEnemyAIController.h"

UBTTask_FindNextPos::UBTTask_FindNextPos() {
	NodeName = TEXT("FindNextPos");
}

EBTNodeResult::Type UBTTask_FindNextPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr) return EBTNodeResult::Failed;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (NavSystem == nullptr) return EBTNodeResult::Failed;

	float DetectRadius = 500.0f;
	FVector CurrentPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AMyEnemyAIController::CurrentPosKey);
	FNavLocation NextLocation;

	if (NavSystem->GetRandomPointInNavigableRadius(CurrentPos, DetectRadius, NextLocation)) {
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AMyEnemyAIController::NextPosKey, NextLocation.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
