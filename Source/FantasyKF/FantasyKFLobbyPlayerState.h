// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "FantasyKFLobbyPlayerState.generated.h"

class UFantasyKFGameInstance;
/**
 * 
 */
UCLASS()
class FANTASYKF_API AFantasyKFLobbyPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_FantasyKFName, Category = Custom)
	FString FantasyKFName;

	void SetName(FString _FantasyKFName);
	void SetOnlineIdentityName(FString PlayerNickname);

private:
	UFUNCTION()
	void OnRep_FantasyKFName();

	void UpdateLobbyPlayerList();
	UFantasyKFGameInstance* GetFKFGameInstance();
};
