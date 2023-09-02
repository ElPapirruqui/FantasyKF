// Fill out your copyright notice in the Description page of Project Settings.


#include "FKFGameState.h"
#include "Net/UnrealNetwork.h"

void AFKFGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFKFGameState, Wave);
	DOREPLIFETIME(AFKFGameState, CurrentGameStatus);
	DOREPLIFETIME(AFKFGameState, EnemiesLeft);
}

void AFKFGameState::OnRep_CurrentGameStatus() {
	OnGameStatusChange();
}

void AFKFGameState::OnRep_EnemiesLeft() {
	OnEnemiesUpdated();
}