// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "MyEnemyAIController.h"
#include"MyCharacter.h"

UBTTask_Attack::UBTTask_Attack() {
	bNotifyTick = true;
	IsAttacking = false;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super:: ExecuteTask(OwnerComp, NodeMemory);

	auto Character = Cast<AMyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (Character == nullptr) return EBTNodeResult::Failed;

	ABLOG(Warning, TEXT("Attack : NPC"));
	Character->Attack();
	IsAttacking = true;
	Character->OnAttackEnd.AddLambda([this]()->void {
		IsAttacking = false;
		});

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaTime)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaTime);
	if (!IsAttacking) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
