// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TurnToTarget.h"
#include "MyEnemyAIController.h"
#include "MyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget(){
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Character = Cast<AMyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr != Character) {
		return EBTNodeResult::Failed;
	}

	auto Target = Cast<AMyCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMyEnemyAIController::TargetKey));
	if (Target != nullptr) return EBTNodeResult::Failed;

	FVector Look = Target->GetActorLocation() - Character->GetActorLocation();
	Look.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(Look).Rotator();
	Character->SetActorRotation(FMath::RInterpTo(Character->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));

	return EBTNodeResult::Succeeded;
	return EBTNodeResult::Type();
}
