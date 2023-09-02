// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FantasyKFMainMenu.generated.h"


USTRUCT()
struct FServerData {
	GENERATED_BODY()

	FString Name;
	uint16 CurrentPlayer;
	uint16 MaxPlayers;
	FString HostUsername;
};
/**
 * 
 */
UCLASS()
class FANTASYKF_API UFantasyKFMainMenu : public UUserWidget
{
	GENERATED_BODY()

	UFantasyKFMainMenu(const FObjectInitializer& ObjectInitializer);

public:
	void SelectIndex(uint32 Index);
	void SetServerList(TArray<FServerData> ServerNames);

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* HostBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinServerBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* OptionsBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* LobbyBackBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* OptionsBackBtn;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UWidget* ServersMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* OptionsMenu;

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* ServerList;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* ServerNameInput;

	UPROPERTY(meta = (BindWidget))
	class UComboBoxString* MapComboBox;

	class UFantasyKFGameInstance* KFGameInstance;

	TOptional<uint32> SelectedIndex;

	TSubclassOf<class UUserWidget> ServerRowClass;
	
protected:
	virtual bool Initialize() override;

	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void OpenOptionsMenu();

	UFUNCTION()
	void BackToMainMenu();

	UFUNCTION()
	void JoinGame();

	UFUNCTION()
	void QuitGame();
};
