// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/Pawn.h"
#include "FKFCrowdFollowingComponent.h"
#include "DrawDebugHelpers.h"
#include "NavigationData.h"
#include "NavMesh/RecastNavMesh.h"
#include "DrawDebugHelpers.h"

AEnemyAIController::AEnemyAIController(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UFKFCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
	PathFollowingComponent = Cast<UFKFCrowdFollowingComponent>(GetDefaultSubobjectByName(TEXT("PathFollowingComponent")));

	/*static ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTreeObject(TEXT("BehaviorTree'/Game/FantasyKF/BT_Enemy.BT_Enemy'"));
	if (BehaviorTreeObject.Succeeded())
	{
		BehaviorTree = BehaviorTreeObject.Object;
	}
	BehaviorTreeComponent = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorTreeComponent"));
	BlackboardComponent = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComponent"));*/
}

void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bShowPathLines) {
		NavNodeRef CurrentPoly = PathFollowingComponent->NavPoly_FindNearest(GetPawn()->GetActorLocation(), FVector(0, 0, 100));
		DrawPoly(CurrentPoly, FColor::Magenta, false);
		const ANavigationData* NavData = PathFollowingComponent->FKFGetNavData();
		const ARecastNavMesh* NavMesh = Cast<ARecastNavMesh>(NavData);
		TArray<NavNodeRef> NeighborPolys;
		if (NavMesh->GetPolyNeighbors(CurrentPoly, NeighborPolys)) {
			for (NavNodeRef NeighborPoly : NeighborPolys) {
				DrawPoly(NeighborPoly, FColor::Blue, false);
			}
		}
	}
}

void AEnemyAIController::ToggleAvoidance(bool bEnabled) {
	PathFollowingComponent->SetCrowdObstacleAvoidance(bEnabled, true);
}

void AEnemyAIController::SetDeadMaskGroup(int32 DeadMaskGroup) {
	PathFollowingComponent->SetAvoidanceGroup(DeadMaskGroup, true);
}

void AEnemyAIController::GetNavPolys() {
	TArray<NavNodeRef> Polys;
	if (PathFollowingComponent->NavPoly_GetAllPolys(Polys)) {
		for (NavNodeRef PolyId : Polys) {
			DrawPoly(PolyId, FColor::Green, true);
		}
	}
}

void AEnemyAIController::DrawPoly(NavNodeRef& Poly, FColor Color, bool bPersistent) {
	FVector PolyLocation;
	TArray<FVector> PolyVertsArray;
	const ANavigationData* NavData = PathFollowingComponent->FKFGetNavData();
	const ARecastNavMesh* NavMesh = Cast<ARecastNavMesh>(NavData);
	if (NavMesh->GetPolyCenter(Poly, PolyLocation)) {
		DrawDebugSphere(GetWorld(), PolyLocation, 10.0f, 6, Color, bPersistent, 0.2f);
	}
	if (NavMesh->GetPolyVerts(Poly, PolyVertsArray)) {
		uint32 i = 0;
		for (FVector PolyVert : PolyVertsArray) {
			if (i > 0) {
				DrawDebugLine(GetWorld(), PolyVertsArray[i - 1], PolyVert, Color, bPersistent, 0.2f);
			}
			i++;
		}
	}
}

/*void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(BehaviorTree);
	BehaviorTreeComponent->StartTree(*BehaviorTree);
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (BlackboardComponent != nullptr)
	{
		BlackboardComponent->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	}
}

UBlackboardComponent* AEnemyAIController::GetBlackboard() const
{
	return BlackboardComponent;
}*/