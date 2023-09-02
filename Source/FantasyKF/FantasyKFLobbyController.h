// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FantasyKFLobbyController.generated.h"

class UFantasyKFGameInstance;
/**
 * 
 */
UCLASS()
class FANTASYKF_API AFantasyKFLobbyController : public APlayerController
{
	GENERATED_BODY()

protected:
	void BeginPlay() override;

	void OnPossess(APawn* InPawn) override;
	void AcknowledgePossession(class APawn* InPawn) override;
	void SetUpLobby();
	void OnRep_PlayerState() override;
	UFantasyKFGameInstance* GetFKFGameInstance();
	FTimerHandle GetStartTimer;
	class ACameraActor* LobbyCamera;

	UFUNCTION()
	void SetCamera();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerUpdateName(const FString &PlayerNickname);
};
