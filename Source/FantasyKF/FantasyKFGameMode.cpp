// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "FantasyKFGameMode.h"
#include "FantasyKFHUD.h"
#include "FantasyKFCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFantasyKFGameMode::AFantasyKFGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFantasyKFHUD::StaticClass();
}

void AFantasyKFGameMode::HandleSeamlessTravelPlayer(AController*& Controller) {
	Super::HandleSeamlessTravelPlayer(Controller);
	PlayerControllers.Add(Controller);
}

void AFantasyKFGameMode::PostLogin(APlayerController* NewPlayer) {
	Super::PostLogin(NewPlayer);
	PlayerControllers.Add(NewPlayer);
};

void AFantasyKFGameMode::Logout(AController* Exiting) {
	Super::Logout(Exiting);
	PlayerControllers.Remove(Exiting);
}

void AFantasyKFGameMode::OrderVectorArray(FVector Origin, TArray<FVector> VectorsArray, TArray<FVector>& OrderedArray) {
	VectorsArray.Sort([Origin](const FVector& A, const FVector& B) {
		float DistanceA = (A - Origin).Size();
		float DistanceB = (B - Origin).Size();
		return DistanceA <= DistanceB;
	});
	OrderedArray = VectorsArray;
}

void AFantasyKFGameMode::RestartGame(){
	GetWorld()->ServerTravel("?Restart", true);
}