// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FKFCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class FANTASYKF_API UFKFCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
	friend class FSavedMove_My;

public:
	void UpdateFromCompressedFlags(uint8 Flags) override;

	UFUNCTION(BlueprintCallable, Category = "My Character Movement")
	void SetIgnoreCollision(bool bIsIgnoringCollision);

	UFUNCTION(BlueprintCallable, Category = "My Character Movement")
	void SetSprinting(bool bIsSprinting);

	UFUNCTION(BlueprintCallable, Category = "My Character Movement")
	void SetBlocking(bool bIsBlocking);

	UFUNCTION(BlueprintCallable, Category = "My Character Movement")
	void SetAirBreaking(bool bIsAirBreaking);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Character Movement", Meta = (AllowPrivateAccess = "true"))
	uint8 IgnoreCollision : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Character Movement", Meta = (AllowPrivateAccess = "true"))
	uint8 Sprinting : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Character Movement", Meta = (AllowPrivateAccess = "true"))
	uint8 Blocking : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Character Movement", Meta = (AllowPrivateAccess = "true"))
	uint8 AirBreaking : 1;
	
	bool bIsIgnoreOn = false;
	bool bIsSprintOn = false;
	bool bIsBlockOn = false;
	bool bIsAirBreakOn = false;

	enum ENewMove newMovement;

	FNetworkPredictionData_Client* GetPredictionData_Client() const override;

protected:
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void BeginPlay() override;
	void PerformMovement(float DeltaTime) override;

private:
	bool pressIgnoreCollision = false;
	bool pressSprinting = false;
	bool pressBlocking = false;
	bool pressAirBreaking = false;
};

class FSavedMove_My : public FSavedMove_Character
{
public:
	typedef FSavedMove_Character Super;
	// Sets sprinting to either enabled or disabled
	// Resets all saved variables.
	virtual void Clear() override;
	// Store input commands in the compressed flags.
	virtual uint8 GetCompressedFlags() const override;
	// This is used to check whether or not two moves can be combined into one.
	// Basically you just check to make sure that the saved variables are the same.
	virtual bool CanCombineWith(const FSavedMovePtr& NewMovePtr, ACharacter* Character, float MaxDelta) const override;
	// Sets up the move before sending it to the server. 
	virtual void SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character& ClientData) override;
	// Sets variables on character movement component before making a predictive correction.
	virtual void PrepMoveFor(class ACharacter* Character) override;

private:
	uint8 SavedIgnoreCollision : 1;
	uint8 SavedSprinting : 1;
	uint8 SavedBlocking : 1;
	uint8 SavedAirBreaking : 1;
};

class FNetworkPredictionData_Client_My : public FNetworkPredictionData_Client_Character
{
public:
	typedef FNetworkPredictionData_Client_Character Super;

	// Constructor
	FNetworkPredictionData_Client_My(const UCharacterMovementComponent& ClientMovement);

	//brief Allocates a new copy of our custom saved move
	virtual FSavedMovePtr AllocateNewMove() override;
};
