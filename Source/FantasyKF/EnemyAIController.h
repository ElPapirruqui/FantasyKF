// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

class UFKFCrowdFollowingComponent;

/**
 * 
 */
UCLASS()
class FANTASYKF_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AEnemyAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void ToggleAvoidance(bool bEnabled);

	UFUNCTION(BlueprintCallable)
	void SetDeadMaskGroup(int32 DeadMaskGroup);

	void GetNavPolys();
	/*void BeginPlay() override;
	void OnPossess(class APawn* InPawn) override;
	class UBlackboardComponent* GetBlackboard() const;*/
	UFKFCrowdFollowingComponent* GetPathFollowingComponent() { return PathFollowingComponent; };
	bool bShowPathLines;

private:
	UFKFCrowdFollowingComponent* PathFollowingComponent;
	void DrawPoly(NavNodeRef &Poly, FColor Color, bool bPersistent);

	/*UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* BehaviorTree;

	class UBlackboardComponent* BlackboardComponent;*/
};
