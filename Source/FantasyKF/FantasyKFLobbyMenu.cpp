// Fill out your copyright notice in the Description page of Project Settings.


#include "FantasyKFLobbyMenu.h"
#include "Components/Button.h"
#include "FantasyKFGameInstance.h"
#include "FantasyKFLobbyGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/PlayerState.h"

UFantasyKFLobbyMenu::UFantasyKFLobbyMenu(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer) {
	/*static ConstructorHelpers::FClassFinder<UUserWidget> PlayerRowBPClass(TEXT("/Game/FantasyKF/UI/WBP_JoinedPlayer'"));
	if (!ensure(PlayerRowBPClass.Class != nullptr)) return;
	PlayerRowClass = PlayerRowBPClass.Class;*/
}

bool UFantasyKFLobbyMenu::Initialize() {

	auto GameInstance = GetWorld()->GetGameInstance();
	if (GameInstance == nullptr) return false;

	KFGameInstance = Cast<UFantasyKFGameInstance>(GameInstance);

	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(LobbyBackBtn != nullptr)) return false;
	LobbyBackBtn->OnClicked.AddDynamic(this, &UFantasyKFLobbyMenu::LoadMainMenu);

	if (!ensure(StartMatchBtn != nullptr)) return false;
	StartMatchBtn->OnClicked.AddDynamic(this, &UFantasyKFLobbyMenu::CallStartGame);

	return true;
}

void UFantasyKFLobbyMenu::LoadMainMenu() {
	UE_LOG(LogTemp, Warning, TEXT("Load Main Menu"));
	if (KFGameInstance != nullptr) {
		KFGameInstance->LoadMainMenu();
	}
}

void UFantasyKFLobbyMenu::CallStartGame() {
	UE_LOG(LogTemp, Warning, TEXT("Call Start Game"));
	auto GameMode = Cast<AFantasyKFLobbyGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode == nullptr) return;
	GameMode->CallStartGame();
}

void UFantasyKFLobbyMenu::UpdateLobbyPlayersList(const TArray<APlayerState*>& PlayerArray) {
	OnUpdateLobbyPlayersList(PlayerArray);
}