// Fill out your copyright notice in the Description page of Project Settings.


#include "FantasyKFLobbyGameMode.h"
#include "TimerManager.h"
#include "FantasyKFGameInstance.h"
#include "FantasyKFLobbyGameState.h"
#include "FantasyKFLobbyController.h"
#include "FantasyKFLobbyPlayerState.h"

AFantasyKFLobbyGameMode::AFantasyKFLobbyGameMode() : Super(){
	GameStateClass = AFantasyKFLobbyGameState::StaticClass();
	PlayerControllerClass = AFantasyKFLobbyController::StaticClass();
	PlayerStateClass = AFantasyKFLobbyPlayerState::StaticClass();
}

void AFantasyKFLobbyGameMode::CallStartGame() {
	UE_LOG(LogTemp, Warning, TEXT("CALL START GAME"));
	GetWorldTimerManager().SetTimer(GetStartTimer, this, &AFantasyKFLobbyGameMode::StartGame, 3);
	bUseSeamlessTravel = 1;
}

void AFantasyKFLobbyGameMode::StartGame() {
	UE_LOG(LogTemp, Warning, TEXT("START GAME"));
	auto KFGameInstance = Cast<UFantasyKFGameInstance>(GetGameInstance());
	if (KFGameInstance == nullptr) return;

	KFGameInstance->StartSession();
	KFGameInstance->OnSessionCreated();
	
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	World->ServerTravel(KFGameInstance->SelectedMap);
}

void AFantasyKFLobbyGameMode::UpdateLobbyPlayerList() {
	UWorld* World = GetWorld();
	if (World == nullptr) return;

	AGameStateBase* MyGameState = World->GetGameState();
	if (MyGameState == nullptr) return;

	UFantasyKFGameInstance* FantasyKFGameInstance = Cast<UFantasyKFGameInstance>(World->GetGameInstance());
	if (FantasyKFGameInstance == nullptr) return;

	FantasyKFGameInstance->UpdateLobbyPlayersList(MyGameState->PlayerArray);
}