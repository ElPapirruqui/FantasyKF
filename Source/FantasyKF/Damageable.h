// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Damageable.generated.h"

UENUM(BlueprintType)
enum EStaggerType
{
	None		  UMETA(DisplayName = "None"),
	Weaker        UMETA(DisplayName = "Weaker"),
	Weak		  UMETA(DisplayName = "Weak"),
	Strong		  UMETA(DisplayName = "Strong"),
	Stronger	  UMETA(DisplayName = "Stronger"),
};


USTRUCT(BlueprintType)
struct FAttackProperties
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 AttackIndex;

	UPROPERTY()
	uint8 AttackAttempt;
};

USTRUCT(BlueprintType)
struct FHitProperties
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TEnumAsByte<EStaggerType> Stagger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bWasBlocked;

	UPROPERTY()
	uint8 HitAttempt;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamageable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FANTASYKF_API IDamageable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void ApplyDamage(float Damage, EStaggerType Stagger) = 0;
	virtual float GetHealth() = 0;
};
