// Fill out your copyright notice in the Description page of Project Settings.


#include "FantasyKFLobbyGameState.h"
#include "FantasyKFGameInstance.h"

void AFantasyKFLobbyGameState::HandleBeginPlay(){
	Super::HandleBeginPlay();
	if (HasAuthority()) {
		UE_LOG(LogTemp, Warning, TEXT("GAME STATE STARTED"));
	}
}

void AFantasyKFLobbyGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);
	UE_LOG(LogTemp, Warning, TEXT("ADDED PLAYER STATE"));
	//UpdateLobbyPlayerList();
}

void AFantasyKFLobbyGameState::RemovePlayerState(APlayerState* PlayerState)
{
	Super::RemovePlayerState(PlayerState);
	UE_LOG(LogTemp, Warning, TEXT("REMOVED PLAYER STATE"));
	UpdateLobbyPlayerList();
}

void AFantasyKFLobbyGameState::UpdateLobbyPlayerList() {
	UWorld* World = GetWorld();
	if (World == nullptr) return;

	UFantasyKFGameInstance* FantasyKFGameInstance = Cast<UFantasyKFGameInstance>(World->GetGameInstance());
	if (FantasyKFGameInstance == nullptr) return;

	FantasyKFGameInstance->UpdateLobbyPlayersList(PlayerArray);
}