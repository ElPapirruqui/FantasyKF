// Fill out your copyright notice in the Description page of Project Settings.


#include "FantasyKFMainMenu.h"
#include "UObject/ConstructorHelpers.h"
#include "FantasyKFGameInstance.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "FantasyKFServerRow.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/ComboBoxString.h"

UFantasyKFMainMenu::UFantasyKFMainMenu(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer) {
	static ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/FantasyKF/UI/WBP_ServerRow"));
	if (!ensure(ServerRowBPClass.Class != nullptr)) return;
	ServerRowClass = ServerRowBPClass.Class;
}

bool UFantasyKFMainMenu::Initialize() {

	auto GameInstance = GetWorld()->GetGameInstance();
	if (GameInstance == nullptr) return false;

	KFGameInstance = Cast<UFantasyKFGameInstance>(GameInstance);

	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(HostBtn != nullptr)) return false;
	HostBtn->OnClicked.AddDynamic(this, &UFantasyKFMainMenu::HostServer);

	if (!ensure(JoinBtn != nullptr)) return false;
	JoinBtn->OnClicked.AddDynamic(this, &UFantasyKFMainMenu::OpenJoinMenu);

	if (!ensure(JoinServerBtn != nullptr)) return false;
	JoinServerBtn->OnClicked.AddDynamic(this, &UFantasyKFMainMenu::JoinGame);

	if (!ensure(OptionsBtn != nullptr)) return false;
	OptionsBtn->OnClicked.AddDynamic(this, &UFantasyKFMainMenu::OpenOptionsMenu);

	if (!ensure(LobbyBackBtn != nullptr)) return false;
	LobbyBackBtn->OnClicked.AddDynamic(this, &UFantasyKFMainMenu::BackToMainMenu);

	if (!ensure(QuitBtn != nullptr)) return false;
	QuitBtn->OnClicked.AddDynamic(this, &UFantasyKFMainMenu::QuitGame);

	if (!ensure(OptionsBackBtn != nullptr)) return false;
	OptionsBackBtn->OnClicked.AddDynamic(this, &UFantasyKFMainMenu::BackToMainMenu);

	return true;
}

void UFantasyKFMainMenu::HostServer() {
	if (KFGameInstance != nullptr) {
		KFGameInstance->SelectedMap = KFGameInstance->Maps[MapComboBox->GetSelectedIndex()];
	}
	FString ServerName = ServerNameInput->GetText().ToString();
	KFGameInstance->Host(ServerName);
}

void UFantasyKFMainMenu::OpenJoinMenu() {
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(ServersMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(ServersMenu);
	if (KFGameInstance != nullptr) {
		KFGameInstance->RefreshServerList();
	}
}

void UFantasyKFMainMenu::BackToMainMenu() {
	if (!ensure(MenuSwitcher != nullptr)) return;
	MenuSwitcher->SetActiveWidgetIndex(0);
}

void UFantasyKFMainMenu::QuitGame() {
	UWorld* World = GetWorld();
	if (World == nullptr) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ConsoleCommand("quit");
}

void UFantasyKFMainMenu::SelectIndex(uint32 Index) {
	UE_LOG(LogTemp, Warning, TEXT("Index Selected!"));
	SelectedIndex = Index;
}

void UFantasyKFMainMenu::SetServerList(TArray<FServerData> ServerNames) {
	if (!ensure(ServerRowClass != nullptr)) return;
	if (!ensure(ServerList != nullptr)) return;
	ServerList->ClearChildren();
	uint32 i = 0;
	for (const FServerData& ServerData : ServerNames) {
		UFantasyKFServerRow* ServerRow = CreateWidget<UFantasyKFServerRow>(this, ServerRowClass);
		ServerRow->ServerName->SetText(FText::FromString(*ServerData.Name));
		ServerRow->HostUser->SetText(FText::FromString(*ServerData.HostUsername));
		ServerRow->ConnectionFraction->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), ServerData.CurrentPlayer, ServerData.MaxPlayers)));
		ServerRow->Setup(this, i);
		++i;
		ServerList->AddChild(ServerRow);
	}
}

void UFantasyKFMainMenu::JoinGame() {
	if (SelectedIndex.IsSet()) {
		UE_LOG(LogTemp, Warning, TEXT("Selected Index: %u"), SelectedIndex.GetValue());
		if (KFGameInstance != nullptr) {
			KFGameInstance->Join(SelectedIndex.GetValue());
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Selected Index not set"));
	}
}

void UFantasyKFMainMenu::OpenOptionsMenu() {
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(OptionsMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(OptionsMenu);
}