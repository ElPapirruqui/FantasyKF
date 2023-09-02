// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "FantasyKFLobbyGameState.generated.h"

/**
 * 
 */
UCLASS()
class FANTASYKF_API AFantasyKFLobbyGameState : public AGameStateBase
{
	GENERATED_BODY()

	void HandleBeginPlay() override;

	/** Add PlayerState to the PlayerArray */
	void AddPlayerState(APlayerState* PlayerState) override;

	/** Remove PlayerState from the PlayerArray. */
	void RemovePlayerState(APlayerState* PlayerState) override;

public:
	UFUNCTION(BlueprintCallable)
	void UpdateLobbyPlayerList();
};
