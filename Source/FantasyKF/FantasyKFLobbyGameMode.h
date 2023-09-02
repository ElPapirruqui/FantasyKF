// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FantasyKFLobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class FANTASYKF_API AFantasyKFLobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

	AFantasyKFLobbyGameMode();

	FTimerHandle GetStartTimer;

public:
	void CallStartGame();
	void StartGame();

	UFUNCTION(BlueprintCallable)
	void UpdateLobbyPlayerList();
};
