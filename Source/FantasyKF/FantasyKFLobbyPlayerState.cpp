// Fill out your copyright notice in the Description page of Project Settings.


#include "FantasyKFLobbyPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "FantasyKFLobbyGameState.h"
#include "FantasyKFGameInstance.h"

void AFantasyKFLobbyPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFantasyKFLobbyPlayerState, FantasyKFName);
}

void AFantasyKFLobbyPlayerState::SetName(FString _FantasyKFName) { 
	FantasyKFName = _FantasyKFName; 
	UpdateLobbyPlayerList();
};

void AFantasyKFLobbyPlayerState::OnRep_FantasyKFName() {
	UpdateLobbyPlayerList();
}

void AFantasyKFLobbyPlayerState::UpdateLobbyPlayerList() {
	UWorld* World = GetWorld();
	if (World == nullptr) return;

	AFantasyKFLobbyGameState* FKFLobbyGameState = Cast<AFantasyKFLobbyGameState>(GetWorld()->GetGameState());
	if (FKFLobbyGameState == nullptr) return;

	FKFLobbyGameState->UpdateLobbyPlayerList();
}

void AFantasyKFLobbyPlayerState::SetOnlineIdentityName(FString PlayerNickname) {
    SetName(PlayerNickname);
}

UFantasyKFGameInstance* AFantasyKFLobbyPlayerState::GetFKFGameInstance() {
	UFantasyKFGameInstance* FantasyKFGameInstance = nullptr;

	UWorld* World = GetWorld();
	if (World == nullptr) FantasyKFGameInstance;

	UGameInstance* GameInstance = World->GetGameInstance();
	if (GameInstance == nullptr) return FantasyKFGameInstance;

	FantasyKFGameInstance = Cast<UFantasyKFGameInstance>(GameInstance);

	return FantasyKFGameInstance;
}