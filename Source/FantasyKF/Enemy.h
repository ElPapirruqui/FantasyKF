// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Damageable.h"
#include "AI/Navigation/NavigationAvoidanceTypes.h"
#include "Enemy.generated.h"

class UNavigationQueryFilter;

UCLASS()
class FANTASYKF_API AEnemy : public ACharacter, public IDamageable
{
	GENERATED_BODY()

	/*UPROPERTY(VisibleDefaultsOnly)
	class UCapsuleComponent* EnemyCapsule;*/

	/*UPROPERTY(VisibleDefaultsOnly)
	class UEnemyMovementComponent* EnemyMovementComponent;*/

	

public:
	// Sets default values for this pawn's properties
	AEnemy();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	ACharacter* TargetPlayer;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Mesh)
	class USceneComponent* MeshContainer;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Mesh)
	class USkeletalMeshComponent* BodyMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom)
	bool bShowPathLines;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom)
	bool bShowAllNavPolys;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom)
	bool bIsAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom)
	bool bCanAttack = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom)
	float AttackRange = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom)
	float DamageRange = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom)
	float NormalSpeed = 300;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom)
	float AttackSpeed = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom)
	float NormalTurnRate = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom)
	float AttackTurnRate = 10;

	float TurnRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom)
	bool bStopMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom)
	bool bAffectedByVariation = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, ReplicatedUsing = OnRep_Health)
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, Replicated)
	float MaxHealth = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom)
	float Damage = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, Replicated)
	bool bIsDisabledCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom)
	int MinVariation = -20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom)
	int MaxVariation = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom)
	TEnumAsByte<EStaggerType> StaggerType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom)
	bool bUsePathfinding = true;

	UPROPERTY(ReplicatedUsing = OnRep_Variation)
	int Variation;

	UFUNCTION(BlueprintImplementableEvent)
	void OnAttack(AActor* TargetActor);

	UFUNCTION(BlueprintImplementableEvent)
	void OnDeath();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDamaged(EStaggerType _Stagger);

	UFUNCTION(BlueprintCallable)
	void AttackTarget();

	UFUNCTION(BlueprintCallable)
	bool CheckIfPathHasJumps();

	void ApplyDamage(float Damage, EStaggerType Stagger) override;
	float GetHealth() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom)
	float SearchTime = 0.6f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom)
	FVector SearchExtent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, ReplicatedUsing = OnRep_AttackProperties)
	FAttackProperties AttackProperties;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom)
	uint8 NewAttackIndex;

	UFUNCTION(BlueprintImplementableEvent)
	void SetNewAttackIndex(float Distance);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, ReplicatedUsing = OnRep_LastHit)
	FHitProperties LastHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom)
	FNavAvoidanceMask DeadMaskGroup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom)
	TSubclassOf<UNavigationQueryFilter> QueryFilter;

	void CustomTick(float DeltaTime);

private:
	void GetNavPath(AActor* TargetActor, FVector& Goal, bool bCheckForAttack);
	FVector NextPathGoal;
	float CurrentSearchTime;
	float CurrentAttackTime;
	ACharacter* GetClosestCharacter();
	void ApplyVariation();
	FVector GetAggroLocation(AActor* TargetActor);
	void UpdateAttackProperties();
	void UpdateLastHit(EStaggerType _Stagger, bool bWasBlocked);
	bool bAttackOnSearch = false;

	UFUNCTION()
	void OnRep_AttackProperties();

	UFUNCTION()
	void OnRep_Health();

	UFUNCTION()
	void OnRep_Variation();

	UFUNCTION()
	void OnRep_LastHit();
};
