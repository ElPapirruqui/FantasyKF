// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FantasyKFLobbyMenu.generated.h"

/**
 * 
 */
UCLASS()
class FANTASYKF_API UFantasyKFLobbyMenu : public UUserWidget
{
	GENERATED_BODY()

	UFantasyKFLobbyMenu(const FObjectInitializer& ObjectInitializer);
	virtual bool Initialize() override;

	UPROPERTY(meta = (BindWidget))
	class UButton* LobbyBackBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* StartMatchBtn;

	class UFantasyKFGameInstance* KFGameInstance;

	UFUNCTION()
	void LoadMainMenu();

	UFUNCTION()
	void CallStartGame();

	TSubclassOf<class UUserWidget> PlayerRowClass;

public:
	void UpdateLobbyPlayersList(const TArray<class APlayerState*>& PlayerArray);

	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdateLobbyPlayersList(const TArray<class APlayerState*>& PlayerArray);
	
};
