// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "FKFCrowdFollowingComponent.generated.h"

class ANavigationData;
class ARecastNavMesh;
class UCharacterMovementComponent;
class AEnemy;

/**
 * 
 */
UCLASS()
class FANTASYKF_API UFKFCrowdFollowingComponent : public UCrowdFollowingComponent
{
	GENERATED_BODY()
	
public:
	UCharacterMovementComponent* CharacterMoveComp;
	AEnemy* OwnerEnemy;
	void SetObstacleAvoidance(bool bEnabled);

	void FollowPathSegment(float DeltaTime) override;
	/** sets variables related to current move segment */
	void SetMoveSegment(int32 SegmentStartIndex) override;
	/** check state of path following, update move segment if needed */
	void UpdatePathSegment() override;
	bool CheckIfPathHasJumps();

	//Nav Data Main
	ANavigationData* GetMainNavData() const;
	//Choose Which Nav Data To Use
	ANavigationData* FKFGetNavData() const;
	//VERY IMPORTANT FOR CRASH PROTECTION !!!!!
	bool TileIsValid(const ARecastNavMesh* NavMesh, int32 TileIndex) const;
	//add this to your custom path follow component!
	bool NavPoly_GetAllPolys(TArray<NavNodeRef>& Polys);
	NavNodeRef NavPoly_FindNearest(const FVector& Loc, const FVector& Extent);
	bool bShowPathLines;	

};
