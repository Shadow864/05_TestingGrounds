// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "PatrolRouteComponent.h"
#include <BehaviorTree/BehaviorTreeComponent.h>
#include <GameFramework/Actor.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <GameFramework/Controller.h>
#include <AIModule/Classes/AIController.h>


EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
    UPatrolRouteComponent* PatrolRouteConponent = Pawn->FindComponentByClass<UPatrolRouteComponent>();
    UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
   
    if (!PatrolRouteConponent || !BlackboardComponent)
        return EBTNodeResult::Failed;

    auto PatrolPoints = PatrolRouteConponent->GetPatrolPoints();

    int32 Index = BlackboardComponent->GetValueAsInt(WaypointIndex.SelectedKeyName);
 
    if (Index >= PatrolPoints.Num())
        return EBTNodeResult::Failed;

    BlackboardComponent->SetValueAsObject(Waypoint.SelectedKeyName, PatrolPoints[Index]);

    Index++;

    Index = Index % PatrolPoints.Num();

    BlackboardComponent->SetValueAsInt(WaypointIndex.SelectedKeyName, Index);
    return EBTNodeResult::Succeeded;
}




