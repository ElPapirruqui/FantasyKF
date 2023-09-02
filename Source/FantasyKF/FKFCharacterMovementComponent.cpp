// Fill out your copyright notice in the Description page of Project Settings.


#include "FKFCharacterMovementComponent.h"
#include "FantasyKFPC.h"

FNetworkPredictionData_Client* UFKFCharacterMovementComponent::GetPredictionData_Client() const
{
	if (ClientPredictionData == nullptr)
	{
		// Return our custom client prediction class instead
		UFKFCharacterMovementComponent* MutableThis = const_cast<UFKFCharacterMovementComponent*>(this);
		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_My(*this);
	}

	return ClientPredictionData;
}

void UFKFCharacterMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	// Peform local only checks
	if (GetPawnOwner()->IsLocallyControlled())
	{
		if (pressIgnoreCollision) {
			IgnoreCollision = true;
		}
		else {
			IgnoreCollision = false;
		}

		if (pressSprinting) {
			Sprinting = true;
		}
		else {
			Sprinting = false;
		}

		if (pressBlocking) {
			Blocking = true;
		}
		else {
			Blocking = false;
		}

		if (pressAirBreaking) {
			AirBreaking = true;
		}
		else {
			AirBreaking = false;
		}
	}

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UFKFCharacterMovementComponent::PerformMovement(float DeltaSeconds)
{
	if (IgnoreCollision && !bIsIgnoreOn) {
		AFantasyKFPC* MyChar = Cast<AFantasyKFPC>(GetCharacterOwner());
		MyChar->ToggleCollision(true);
		//bIgnoreClientMovementErrorChecksAndCorrection = true;
		bIsIgnoreOn = true;
	}

	if (!IgnoreCollision && bIsIgnoreOn) {
		AFantasyKFPC* MyChar = Cast<AFantasyKFPC>(GetCharacterOwner());
		MyChar->ToggleCollision(false);
		//bIgnoreClientMovementErrorChecksAndCorrection = false;
		bIsIgnoreOn = false;
	}

	if (Sprinting && !bIsSprintOn) {
		AFantasyKFPC* MyChar = Cast<AFantasyKFPC>(GetCharacterOwner());
		MyChar->ToggleSprint(true);
		bIsSprintOn = true;
	}

	if (!Sprinting && bIsSprintOn) {
		AFantasyKFPC* MyChar = Cast<AFantasyKFPC>(GetCharacterOwner());
		MyChar->ToggleSprint(false);
		bIsSprintOn = false;
	}

	if (Blocking && !bIsBlockOn) {
		AFantasyKFPC* MyChar = Cast<AFantasyKFPC>(GetCharacterOwner());
		MyChar->ToggleBlock(true);
		bIsBlockOn = true;
	}

	if (!Blocking && bIsBlockOn) {
		AFantasyKFPC* MyChar = Cast<AFantasyKFPC>(GetCharacterOwner());
		MyChar->ToggleBlock(false);
		bIsBlockOn = false;
	}

	if (AirBreaking && !bIsAirBreakOn) {
		AFantasyKFPC* MyChar = Cast<AFantasyKFPC>(GetCharacterOwner());
		MyChar->ToggleAirBreak(true);
		bIsAirBreakOn = true;
	}

	if (!AirBreaking && bIsAirBreakOn) {
		AFantasyKFPC* MyChar = Cast<AFantasyKFPC>(GetCharacterOwner());
		MyChar->ToggleAirBreak(false);
		bIsAirBreakOn = false;
	}

	Super::PerformMovement(DeltaSeconds);
}

void UFKFCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	// We don't want simulated proxies detecting their own collision
	UE_LOG(LogTemp, Warning, TEXT("Custom Movement Inited!"));
}

void UFKFCharacterMovementComponent::SetIgnoreCollision(bool bIsIgnoringCollision) {
	pressIgnoreCollision = bIsIgnoringCollision;
}

void UFKFCharacterMovementComponent::SetSprinting(bool bIsSprinting) {
	pressSprinting = bIsSprinting;
}

void UFKFCharacterMovementComponent::SetBlocking(bool bIsBlocking) {
	pressBlocking = bIsBlocking;
}

void UFKFCharacterMovementComponent::SetAirBreaking(bool bIsAirBreaking) {
	pressAirBreaking = bIsAirBreaking;
}

void UFKFCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	/*  There are 4 custom move flags for us to use. Below is what each is currently being used for:
	FLAG_Custom_0		= 0x10, // Sprinting
	FLAG_Custom_1		= 0x20, // WallRunning
	FLAG_Custom_2		= 0x40, // Unused
	FLAG_Custom_3		= 0x80, // Unused
	*/

	// Read the values from the compressed flags
	IgnoreCollision = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
	Sprinting = (Flags & FSavedMove_Character::FLAG_Custom_1) != 0;
	Blocking = (Flags & FSavedMove_Character::FLAG_Custom_2) != 0;
	AirBreaking = (Flags & FSavedMove_Character::FLAG_Custom_3) != 0;
}

void FSavedMove_My::Clear()
{
	Super::Clear();

	// Clear all values
	SavedIgnoreCollision = 0;
	SavedSprinting = 0;
	SavedBlocking = 0;
	SavedAirBreaking = 0;
}

uint8 FSavedMove_My::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	/* There are 4 custom move flags for us to use. Below is what each is currently being used for:
	FLAG_Custom_0		= 0x10, // Sprinting
	FLAG_Custom_1		= 0x20, // WallRunning
	FLAG_Custom_2		= 0x40, // Unused
	FLAG_Custom_3		= 0x80, // Unused
	*/

	// Write to the compressed flags 

	if (SavedIgnoreCollision)
		Result |= FLAG_Custom_0;

	if (SavedSprinting)
		Result |= FLAG_Custom_1;

	if (SavedBlocking)
		Result |= FLAG_Custom_2;

	if (SavedAirBreaking)
		Result |= FLAG_Custom_3;

	return Result;
}

bool FSavedMove_My::CanCombineWith(const FSavedMovePtr& NewMovePtr, ACharacter* Character, float MaxDelta) const
{
	const FSavedMove_My* NewMove = static_cast<const FSavedMove_My*>(NewMovePtr.Get());

	// As an optimization, check if the engine can combine saved moves.

	if (SavedIgnoreCollision != NewMove->SavedIgnoreCollision ||
		SavedSprinting != NewMove->SavedSprinting || 
		SavedBlocking != NewMove->SavedBlocking ||
		SavedAirBreaking != NewMove->SavedAirBreaking)
	{
		return false;
	}

	return Super::CanCombineWith(NewMovePtr, Character, MaxDelta);
}

void FSavedMove_My::SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character& ClientData)
{
	Super::SetMoveFor(Character, InDeltaTime, NewAccel, ClientData);


	UFKFCharacterMovementComponent* CharMov = static_cast<UFKFCharacterMovementComponent*>(Character->GetCharacterMovement());
	if (CharMov)
	{

		// Copy values into the saved move
		SavedIgnoreCollision = CharMov->IgnoreCollision;
		SavedSprinting = CharMov->Sprinting;
		SavedBlocking = CharMov->Blocking;
		SavedAirBreaking = CharMov->AirBreaking;
	}
}

void FSavedMove_My::PrepMoveFor(class ACharacter* Character)
{
	Super::PrepMoveFor(Character);


	UFKFCharacterMovementComponent* CharMov = Cast<UFKFCharacterMovementComponent>(Character->GetCharacterMovement());
	if (CharMov)
	{

		// Copt values out of the saved move
		CharMov->IgnoreCollision = SavedIgnoreCollision;
		CharMov->Sprinting = SavedSprinting;
		CharMov->Blocking = SavedBlocking;
		CharMov->AirBreaking = SavedAirBreaking;
	}
}

FNetworkPredictionData_Client_My::FNetworkPredictionData_Client_My(const UCharacterMovementComponent& ClientMovement)
	: Super(ClientMovement)
{

}

FSavedMovePtr FNetworkPredictionData_Client_My::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_My());
}