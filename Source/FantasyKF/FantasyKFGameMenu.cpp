// Fill out your copyright notice in the Description page of Project Settings.


#include "FantasyKFGameMenu.h"

void UFantasyKFGameMenu::QuitGame() {
	UWorld* World = GetWorld();
	if (World == nullptr) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController == nullptr) return;

	PlayerController->ClientTravel("/Game/MedievalDungeon/Maps/Menu", ETravelType::TRAVEL_Absolute);
}