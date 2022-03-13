// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"
#include "MyEnemyAIController.h"
#include "MyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"


UBTService_Detect::UBTService_Detect() {
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}


void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSenconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSenconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr) return;

	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
	float DetectRadius = 200.0f;


	if (World == nullptr) return;

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);
	if (bResult) {
		for (auto const& OverlapResult : OverlapResults) {
			AMyCharacter* Character = Cast<AMyCharacter>(OverlapResult.GetActor());
			if (Character && Character->GetController()->IsPlayerController()) {
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMyEnemyAIController::TargetKey, Character);
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);

				DrawDebugPoint(World, Character->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), Character->GetActorLocation(), FColor::Blue, false, 0.2f);
				return;
			}
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMyEnemyAIController::TargetKey, nullptr);
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}
