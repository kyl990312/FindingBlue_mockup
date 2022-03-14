// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsInAttackRange.h"
#include "MyEnemyAIController.h"
#include "MyCharacter.h"
#include "BehaviorTree/BlackBoardComponent.h"
#include "MyWeapon.h"
#include "DrawDebugHelpers.h"

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange() {
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

    auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
    if (nullptr == ControllingPawn) return false;

    auto Target = Cast<AMyCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMyEnemyAIController::TargetKey));
    if (nullptr == Target) return false;
    
    float AttackRange = 60.0f;
    AMyWeapon* Weapon = nullptr;
    if (Cast<AMyCharacter>(ControllingPawn)->GetCurrentWeaponObject(Weapon)) {
        if(Weapon != nullptr)
            AttackRange = Weapon->GetAttackRange();
        else {
            ABLOG(Warning, TEXT("Weapon isn't set"));
        }
    }
    bResult = (Target->GetDistanceTo(ControllingPawn) <= AttackRange);
    if (!bResult) Cast<AMyCharacter>(ControllingPawn)->AttackEnd();

#if ENABLE_DRAW_DEBUG
    UWorld* World = ControllingPawn->GetWorld();
    FVector Center = ControllingPawn->GetActorLocation();
    DrawDebugSphere(World, Center, AttackRange, 16, bResult ? FColor::Blue : FColor::Yellow, false, 0.2f);
#endif

    return bResult;
}
