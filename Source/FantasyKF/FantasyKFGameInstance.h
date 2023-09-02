// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "interfaces/OnlineSessionInterface.h"
#include "FantasyKFGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class FANTASYKF_API UFantasyKFGameInstance : public UGameInstance
{
	GENERATED_BODY()

	UFantasyKFGameInstance(const FObjectInitializer& ObjectInitializer);
	virtual void Init();

public:
	UFUNCTION()
	void Host(FString ServerName);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom)
	class UFantasyKFMainMenu* MainMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom)
	class UFantasyKFLobbyMenu* LobbyMenu;

	UFUNCTION(BlueprintImplementableEvent)
	void OnSessionCreated();

	UFUNCTION(BlueprintImplementableEvent)
	void CallSetLobbyMenu();

	void StartSession();
	void Join(uint32 Index);
	void LoadMainMenu();
	void RefreshServerList();
	void UpdateLobbyPlayersList(TArray<class APlayerState*> PlayerArray);
	FString GetPlayerNickname(const FUniqueNetId& UserId);
	void PrintString(FString Message);
	TArray<FString> Maps;
	FString SelectedMap;

private:
	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionsComplete(bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);
	void CreateSession();

	IOnlineSessionPtr SessionInterface;
	FString DesiredServerName;

	TSharedPtr<class FOnlineSessionSearch> SessionSearch;
};
