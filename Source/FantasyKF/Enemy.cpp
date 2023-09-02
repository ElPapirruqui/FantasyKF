// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/CapsuleComponent.h"
#include "EngineUtils.h"
#include "EnemyAIController.h"
#include "DrawDebugHelpers.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "EnemyMovementComponent.h"
#include "Components/SceneComponent.h"
#include "Net/UnrealNetwork.h"
#include "Damageable.h"
#include "FantasyKFGameMode.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FKFCrowdFollowingComponent.h"
#include "NavFilters/NavigationQueryFilter.h"
#include "Navigation/PathFollowingComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//BodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyMesh"));
	//EnemyCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RootCapsule"));
	//EnemyMovementComponent = CreateDefaultSubobject<UEnemyMovementComponent>(TEXT("EnemyMovementComponent"));

	//EnemyCapsule->SetRelativeRotation(FRotator::ZeroRotator);
	//EnemyCapsule->SetRelativeLocation(FVector::ZeroVector);

	//EnemyCapsule->SetShouldUpdatePhysicsVolume(true);
	//EnemyCapsule->SetCanEverAffectNavigation(false);
	//EnemyCapsule->bDynamicObstacle = true;

	//BodyMesh->SetRelativeRotation(FRotator::ZeroRotator);
	//BodyMesh->SetRelativeLocation(FVector::ZeroVector);

	/*if (EnemyMovementComponent)
	{
		EnemyMovementComponent->UpdatedComponent = EnemyCapsule;
	}*/

	//RootComponent = EnemyCapsule;
	BodyMesh = GetMesh();
	MeshContainer = CreateDefaultSubobject<USceneComponent>(TEXT("MeshContainer"));
	MeshContainer->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform, FName(TEXT("RootCapsule")));
	BodyMesh->AttachToComponent(MeshContainer, FAttachmentTransformRules::KeepRelativeTransform, FName(TEXT("RootContainer")));
	//MeshContainer->SetIsReplicated(true);
}

void AEnemy::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEnemy, AttackProperties);
	DOREPLIFETIME(AEnemy, Health);
	DOREPLIFETIME(AEnemy, MaxHealth);
	DOREPLIFETIME(AEnemy, bIsDisabledCollision);
	DOREPLIFETIME(AEnemy, Variation);
	DOREPLIFETIME(AEnemy, LastHit);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
	CurrentSearchTime = SearchTime;
	if (HasAuthority()) {
		if (bAffectedByVariation) {
			Variation = FMath::RandRange(MinVariation, MaxVariation);
			ApplyVariation();
		}
		AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController());
		AIController->GetPathFollowingComponent()->CharacterMoveComp = GetCharacterMovement();
		if (bShowAllNavPolys) {
			AIController->GetNavPolys();
		}
		AIController->bShowPathLines = bShowPathLines;
		AIController->GetPathFollowingComponent()->bShowPathLines = bShowPathLines;
		AIController->GetPathFollowingComponent()->OwnerEnemy = this;
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Health <= 0) return;

	if (HasAuthority()) {
		if (CurrentSearchTime > 0) {
			if (!bIsAttacking) {
				TargetPlayer = GetClosestCharacter();
			}

			AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController());
			if (TargetPlayer != nullptr && !bStopMovement) {
				FVector Goal;
				GetNavPath(TargetPlayer, Goal, bAttackOnSearch);
				EPathFollowingRequestResult::Type MoveResult = AIController->MoveToLocation(Goal, 55, true, bUsePathfinding, false, true, QueryFilter);
				/*switch (MoveResult) {
					case EPathFollowingRequestResult::Type::AlreadyAtGoal:
						UE_LOG(LogTemp, Warning, TEXT("Already At Goal!"));
					break;
					case EPathFollowingRequestResult::Type::Failed:
						UE_LOG(LogTemp, Warning, TEXT("Failed!"));
					break;
					case EPathFollowingRequestResult::Type::RequestSuccessful:
						UE_LOG(LogTemp, Warning, TEXT("Request Successful!"));
					break;
				}*/

				auto StartRotation = GetActorForwardVector().Rotation().Quaternion();
				FQuat TargetRotation;
				if (bIsAttacking) {
					TurnRate = AttackTurnRate;
				}
				else {
					TurnRate = NormalTurnRate;
				}
				if (GetVelocity().Size() > 0 && !bIsAttacking) {
					TargetRotation = GetVelocity().GetSafeNormal().Rotation().Quaternion();
				}
				else {
					NextPathGoal = TargetPlayer->GetActorLocation();
					NextPathGoal.Z = GetActorLocation().Z;
					TargetRotation = (NextPathGoal - GetActorLocation()).GetSafeNormal().Rotation().Quaternion();
				}
				
				FQuat NewRotation = FQuat::Slerp(StartRotation, TargetRotation, DeltaTime * TurnRate);
				if (!GetCharacterMovement()->IsFalling()) {
					SetActorRotation(NewRotation);
				}
			}
			CurrentSearchTime -= DeltaTime;
		}
		else {
			CurrentSearchTime = SearchTime;
		}
	}
}

void AEnemy::CustomTick(float DeltaTime) {
	if (Health <= 0 || !bCanAttack) return;

	if (HasAuthority()) {
		if (CurrentAttackTime > 0) {
			AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController());
			if (TargetPlayer != nullptr) {
				FVector Goal;
				GetNavPath(TargetPlayer, Goal, !bAttackOnSearch);
			}
			CurrentAttackTime -= DeltaTime;
		}
		else {
			CurrentAttackTime = SearchTime;
		}
	}
}

ACharacter* AEnemy::GetClosestCharacter() {
	bool bIsFirstMeasure = true;
	float MinDistance;
	ACharacter* ClosestCharacter = nullptr;
	UWorld* World = GetWorld();
	if (World != nullptr) {
		AFantasyKFGameMode* FKFGameMode = Cast<AFantasyKFGameMode>(World->GetAuthGameMode());
		if (FKFGameMode != nullptr) {
			for (AController* PlayerController : FKFGameMode->PlayerControllers) {
				if (PlayerController != nullptr) {
					APawn* ControlledPawn = PlayerController->GetPawn();
					if (ControlledPawn) {
						if (ControlledPawn->GetClass()->ImplementsInterface(UDamageable::StaticClass())) {
							IDamageable* DamageableInterface = Cast<IDamageable>(ControlledPawn);
							if (DamageableInterface->GetHealth() > 0) {
								float AggroDistance = FVector::Distance(ControlledPawn->GetActorLocation(), GetActorLocation());
								if (bIsFirstMeasure) {
									MinDistance = AggroDistance;
									ClosestCharacter = Cast<ACharacter>(ControlledPawn);
									bIsFirstMeasure = false;
								}
								else if(AggroDistance < MinDistance){
									MinDistance = AggroDistance;
									ClosestCharacter = Cast<ACharacter>(ControlledPawn);
								}
							}
						}
					}
				}
			}
		}
	}

	return ClosestCharacter;
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetNavPath(AActor *TargetActor, FVector& Goal, bool bCheckForAttack) {
	if (TargetActor == nullptr) return;

	UNavigationSystemV1* navSys = UNavigationSystemV1::GetCurrent(GetWorld());
	FVector TargetLocation = TargetActor->GetActorLocation();

	UNavigationPath* path = navSys->FindPathToLocationSynchronously(GetWorld(), GetActorLocation(), TargetLocation);
	if (path->IsPartial()) {
		//UE_LOG(LogTemp, Warning, TEXT("PARTIAL"));
	}
	else {
		//UE_LOG(LogTemp, Warning, TEXT("FULL"));
	}
	if (!path->IsValid()) {
		//UE_LOG(LogTemp, Warning, TEXT("PATH IS INVALID"));
		////OLD//// 
		//TargetLocation = navSys->ProjectPointToNavigation(GetWorld(), TargetLocation, (ANavigationData*)0, 0, SearchExtent);
		///////////
		FNavLocation navLoc;
		if (navSys->ProjectPointToNavigation(TargetLocation, navLoc, SearchExtent)){
			TargetLocation = navLoc.Location;
		}		
		path = navSys->FindPathToLocationSynchronously(GetWorld(), GetActorLocation(), TargetLocation);
		if (bShowPathLines) {
			DrawDebugSphere(GetWorld(), TargetLocation, 10.0f, 12, FColor(255, 0, 0));
		}
	}
	Goal = TargetLocation;
	//navSys->ProjectPointToNavigation
	if (bCheckForAttack) {
		float pathLength = path->GetPathLength();
		float AggroDistance = FVector::Distance(TargetActor->GetActorLocation(), GetAggroLocation(TargetActor));
		if (pathLength < AttackRange && AggroDistance < AttackRange && !bIsAttacking && bCanAttack) {
			if (GetCharacterMovement()->IsFalling()) return;

			bIsAttacking = true;
			SetNewAttackIndex(AggroDistance);
			UpdateAttackProperties();
			OnAttack(TargetActor);
		}
	}

	if (path != NULL)
	{
		for (int32 pointiter = 0; pointiter < path->PathPoints.Num(); pointiter++)
		{
			if (pointiter > 0 && bShowPathLines) {
				DrawDebugLine(GetWorld(), path->PathPoints[pointiter - 1], path->PathPoints[pointiter], FColor::Red, false, 0.2f, 1, 2);
			}
			if (pointiter == 1) {
				NextPathGoal = path->PathPoints[pointiter];
			}
			//DrawDebugSphere(GetWorld(), path->PathPoints[pointiter], 10.0f, 6, FColor(255, 0, 0));
		}
		if (path->PathPoints.Num() <= 2) {
			bAttackOnSearch = true;
		}
		else {
			bAttackOnSearch = false;
		}
		//UE_LOG(LogTemp, Warning, TEXT("PATH LENGTH: %d"), path->PathPoints.Num());
	}
}

void AEnemy::UpdateAttackProperties() {
	AttackProperties.AttackIndex = NewAttackIndex;
	AttackProperties.AttackAttempt = (AttackProperties.AttackAttempt + 1) % 10;
}

FVector AEnemy::GetAggroLocation(AActor* TargetActor) {
	FVector AggroLocation;
	FVector FromLocation = GetActorLocation();
	FVector ToLocation = TargetActor->GetActorLocation();
	float MidZPoint = (FromLocation.Z + ToLocation.Z) / 2;
	MidZPoint = FMath::Clamp(MidZPoint, FromLocation.Z - 44, FromLocation.Z + 44);
	AggroLocation = FVector(FromLocation.X, FromLocation.Y, MidZPoint);

	return AggroLocation;
}

void AEnemy::OnRep_AttackProperties() {
	OnAttack(nullptr);
}

void AEnemy::OnRep_Health() {
	if (Health <= 0) {
		OnDeath();
	}
}

void AEnemy::OnRep_Variation() {
	ApplyVariation();
}

void AEnemy::OnRep_LastHit() {
	OnDamaged(LastHit.Stagger);
}

void AEnemy::ApplyVariation() {
	float Variant = (1 + float(Variation) / 100);
	float InverseVariant = (1 - float(Variation) / 100);
	float PositiveVariant = FMath::Clamp<float>(Variant, 1, FMath::Abs(Variant));
	NormalSpeed = NormalSpeed * InverseVariant;
	AttackSpeed = AttackSpeed * InverseVariant;
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	AttackRange = AttackRange * PositiveVariant;
	DamageRange = DamageRange * PositiveVariant;
	FVector PreviousScale = BodyMesh->GetRelativeScale3D();
	FVector NewScale = FVector::ZeroVector;
	NewScale.X = PreviousScale.X * Variant;
	NewScale.Z = PreviousScale.Z * Variant;
	NewScale.Y = PreviousScale.Y * Variant;
	BodyMesh->SetRelativeScale3D(NewScale);
	if (HasAuthority()) {
		Damage = Damage * Variant;
		MaxHealth = MaxHealth * Variant;
		Health = MaxHealth;
	}
}

void AEnemy::AttackTarget(){
	if (!HasAuthority()) return;
	if (TargetPlayer == nullptr || !TargetPlayer->GetClass()->ImplementsInterface(UDamageable::StaticClass())) return;

	IDamageable* DamageableInterface = Cast<IDamageable>(TargetPlayer);
	float AggroDistance = FVector::Distance(TargetPlayer->GetActorLocation(), GetAggroLocation(TargetPlayer));
	if (AggroDistance > DamageRange) return;

	DamageableInterface->ApplyDamage(Damage, StaggerType);
}

void AEnemy::ApplyDamage(float _Damage, EStaggerType _Stagger) {
	Health -= _Damage;
	bCanAttack = false;
	OnDamaged(_Stagger);
	UpdateLastHit(_Stagger, false);
	if (Health <= 0) {
		AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController());
		AIController->SetDeadMaskGroup(DeadMaskGroup.Packed);
		OnDeath();
	}
}

void AEnemy::UpdateLastHit(EStaggerType _Stagger, bool bWasBlocked) {
	LastHit.Stagger = _Stagger;
	LastHit.bWasBlocked = bWasBlocked;
	LastHit.HitAttempt = (LastHit.HitAttempt + 1) % 10;
}

float AEnemy::GetHealth() {
	return Health;
}

bool AEnemy::CheckIfPathHasJumps() {
	bool bPathHasJumps = false;
	AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController());
	if (AIController != nullptr) {
		bPathHasJumps = AIController->GetPathFollowingComponent()->CheckIfPathHasJumps();
	}

	return bPathHasJumps;
}