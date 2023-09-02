// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Damageable.h"
#include "FantasyKFPC.generated.h"

UCLASS()
class FANTASYKF_API AFantasyKFPC : public ACharacter, public IDamageable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFantasyKFPC(const class FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, ReplicatedUsing = OnRep_Health)
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, Replicated)
	float MaxHealth = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, Replicated)
	float Damage = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, Replicated)
	float MaxStamina = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, ReplicatedUsing = OnRep_Stamina)
	float Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, ReplicatedUsing = OnRep_Healings)
	int32 Healings;

	UFUNCTION(BlueprintCallable, Category = "My Character Movement")
	void SetIgnoreCollision(bool bIsIgnoring);

	UFUNCTION(BlueprintCallable, Category = "My Character Movement")
	void SetSprinting(bool bIsSprinting);

	UFUNCTION(BlueprintCallable, Category = "My Character Movement")
	void SetBlocking(bool bIsBlocking);

	UFUNCTION(BlueprintCallable, Category = "My Character Movement")
	void SetAirBreaking(bool bIsAirBreaking);

	UFUNCTION(BlueprintCallable)
	void CallUpdateLastHit(EStaggerType _Stagger, bool bWasBlocked);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, ReplicatedUsing = OnRep_LastHit)
	FHitProperties LastHit;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnRep_Health();

	UFUNCTION()
	void OnRep_Stamina();

	UFUNCTION()
	void OnRep_Healings();

	UFUNCTION()
	void OnRep_LastHit();

	void UpdateLastHit(EStaggerType _Stagger, bool bWasBlocked);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerApplyDamage(AActor* TargetActor, float _Damage, EStaggerType _Stagger, float _Stamina);

	UFUNCTION(BlueprintCallable)
	void UseHealing(AFantasyKFPC* TargetPlayer);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerHeal(AFantasyKFPC* TargetPlayer);

	void ApplyDamage(float Damage, EStaggerType Stagger) override;
	float GetHealth() override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnDamaged(float _Damage, EStaggerType _Stagger);

	UFUNCTION(BlueprintCallable)
	void OnTargetHit(AActor* TargetActor, float _Damage, EStaggerType _Stagger, float _Stamina);

	void ToggleSprint(bool bEnable);

	UFUNCTION(BlueprintImplementableEvent)
	void OnToggleSprint(bool bEnable);

	void ToggleCollision(bool bEnable);

	UFUNCTION(BlueprintImplementableEvent)
	void OnToggleCollision(bool bEnable);

	void ToggleBlock(bool bEnable);

	UFUNCTION(BlueprintImplementableEvent)
	void OnToggleBlock(bool bEnable);

	void ToggleAirBreak(bool bEnable);

	UFUNCTION(BlueprintImplementableEvent)
	void OnToggleAirBreak(bool bEnable);

	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdateHealth();

	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdateStamina();

	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdateHealings();

	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdateLastHit();
};
