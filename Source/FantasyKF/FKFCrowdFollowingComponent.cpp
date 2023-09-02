// Fill out your copyright notice in the Description page of Project Settings.


#include "FKFCrowdFollowingComponent.h"
#include "NavigationSystem.h"
#include "NavMesh/RecastNavMesh.h"
#include "AI/NavigationSystemBase.h"
#include "NavigationData.h"
#include "DrawDebugHelpers.h"
#include "NavAreaHelper.h"
#include "Enemy.h"

void UFKFCrowdFollowingComponent::SetObstacleAvoidance(bool bEnabled) {
	bEnableObstacleAvoidance = bEnabled;
	UpdateCrowdAgentParams();
}

void UFKFCrowdFollowingComponent::FollowPathSegment(float DeltaTime) {
	Super::FollowPathSegment(DeltaTime);
	OwnerEnemy->CustomTick(DeltaTime);
}
/** sets variables related to current move segment */
void UFKFCrowdFollowingComponent::SetMoveSegment(int32 SegmentStartIndex) {
	Super::SetMoveSegment(SegmentStartIndex);
	if (bShowPathLines) {
		if (CharacterMoveComp != NULL)
		{		
			//const FNavPathPoint& SegmentStart = GetPath()->PathPoints[MoveSegmentStartIndex];
			FNavMeshPath* NavMeshPath = Path->CastPath<FNavMeshPath>();
			if (NavMeshPath != nullptr) {
				ARecastNavMesh* RecastNavData = Cast<ARecastNavMesh>(NavMeshPath->GetNavigationDataUsed());
				for (uint64& PathPoint : NavMeshPath->PathCorridor) {
					FVector PolyCenter;
					uint16 PolyFlags;
					uint16 AreaFlags;
					RecastNavData->GetPolyFlags(PathPoint, PolyFlags, AreaFlags);
					if (RecastNavData->GetPolyCenter(PathPoint, PolyCenter)) {
						DrawDebugSphere(GetWorld(), PolyCenter, 20.0f, 6, FColor::Cyan);
						if (UNavAreaHelper::HasFlag(PolyFlags, ENavAreaFlag::Jump))
						{
							UE_LOG(LogTemp, Warning, TEXT("JUMP!"));
						}
						if (UNavAreaHelper::HasFlag(PolyFlags, ENavAreaFlag::Jump2))
						{
							UE_LOG(LogTemp, Warning, TEXT("JUMP2!"));
						}
					}
				}
				int32 MyPathStartIndex = SegmentStartIndex + 1;
				if (NavMeshPath->PathCorridor.IsValidIndex(MyPathStartIndex))
				{
					FVector PolyCenter;
					uint16 PolyFlags;
					uint16 AreaFlags;
					uint64 CurrentPathIndex = NavMeshPath->PathCorridor[MyPathStartIndex];
					RecastNavData->GetPolyFlags(CurrentPathIndex, PolyFlags, AreaFlags);
					RecastNavData->GetPolyCenter(CurrentPathIndex, PolyCenter);				
					/*UE_LOG(LogTemp, Warning, TEXT("Current Path Index: %d"), CurrentPathIndex);
					UE_LOG(LogTemp, Warning, TEXT("Poly Flags: %d"), PolyFlags);
					UE_LOG(LogTemp, Warning, TEXT("Area Flags: %d"), AreaFlags);*/
					DrawDebugSphere(GetWorld(), PolyCenter, 30.0f, 6, FColor::Red);
					/*if (UNavArea_Jump::HasJumpFlag(PolyFlags))
					{
						// jump! well... fly-in-straight-line!
						//UE_LOG(LogTemp, Warning, TEXT("JUMP!"));
						//CharacterMoveComp->SetMovementMode(MOVE_Flying);
					}
					else
					{
						// regular move
						//CharacterMoveComp->SetMovementMode(MOVE_Walking);
					}*/
				}
			}
		}
	}
}
/** check state of path following, update move segment if needed */
void UFKFCrowdFollowingComponent::UpdatePathSegment() {
	Super::UpdatePathSegment();
}

//Nav Data Main
ANavigationData* UFKFCrowdFollowingComponent::GetMainNavData() const
{
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSys) return NULL;
	return Cast<ANavigationData>(NavSys->GetMainNavData());
}

//Choose Which Nav Data To Use
ANavigationData* UFKFCrowdFollowingComponent::FKFGetNavData() const
{
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	const FNavAgentProperties& AgentProperties = MovementComp->GetNavAgentPropertiesRef();
	ANavigationData* NavData = NavSys->GetNavDataForProps(AgentProperties);
	if (NavData == NULL)
	{
		NavData = GetMainNavData();
	}

	return NavData;
}

//VERY IMPORTANT FOR CRASH PROTECTION !!!!!
bool UFKFCrowdFollowingComponent::TileIsValid(const ARecastNavMesh* NavMesh, int32 TileIndex) const
{
	if (!NavMesh) return false;
	//~~~~~~~~~~~~~~
	const FBox TileBounds = NavMesh->GetNavMeshTileBounds(TileIndex);

	return TileBounds.IsValid != 0;
}

bool UFKFCrowdFollowingComponent::NavPoly_GetAllPolys(TArray<NavNodeRef>& Polys)
{
	if (!MovementComp) return false;
	//~~~~~~~~~~~~~~~~~~

	//Get Nav Data
	const ANavigationData* NavData = FKFGetNavData();

	const ARecastNavMesh* NavMesh = Cast<ARecastNavMesh>(NavData);
	if (!NavMesh)
	{
		return false;
	}

	TArray<FNavPoly> EachPolys;
	for (int32 v = 0; v < NavMesh->GetNavMeshTilesCount(); v++)
	{

		//CHECK IS VALID FIRST OR WILL CRASH!!! 
	   //     256 entries but only few are valid!
		// use continue in case the valid polys are not stored sequentially
		if (!TileIsValid(NavMesh, v))
		{
			continue;
		}

		NavMesh->GetPolysInTile(v, EachPolys);
	}


	//Add them all!
	for (int32 v = 0; v < EachPolys.Num(); v++)
	{
		Polys.Add(EachPolys[v].Ref);
	}

	return true;
}

NavNodeRef UFKFCrowdFollowingComponent::NavPoly_FindNearest(const FVector& Loc, const FVector& Extent)
{
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	const ANavigationData* NavData = FKFGetNavData();

	const ARecastNavMesh* NavMesh = Cast<ARecastNavMesh>(NavData);
	if (!NavMesh)
	{
		return false;
	}

	return NavMesh->FindNearestPoly(Loc, Extent, NavData->GetDefaultQueryFilter());
}

bool UFKFCrowdFollowingComponent::CheckIfPathHasJumps() {
	bool bPathHasJumps = false;

	if (Path != nullptr && Path.IsValid()) {
		FNavMeshPath* NavMeshPath = Path->CastPath<FNavMeshPath>();
		if (NavMeshPath != nullptr) {
			ARecastNavMesh* RecastNavData = Cast<ARecastNavMesh>(NavMeshPath->GetNavigationDataUsed());
			for (uint64& PathPoint : NavMeshPath->PathCorridor) {
				FVector PolyCenter;
				uint16 PolyFlags;
				uint16 AreaFlags;
				RecastNavData->GetPolyFlags(PathPoint, PolyFlags, AreaFlags);
				if (RecastNavData->GetPolyCenter(PathPoint, PolyCenter)) {
					UE_LOG(LogTemp, Warning, TEXT("Flags: %d"), PolyFlags);
					if (UNavAreaHelper::HasFlag(PolyFlags, ENavAreaFlag::Jump))
					{
						bPathHasJumps = true;
					}
				}
			}
		}
	}

	return bPathHasJumps;
}