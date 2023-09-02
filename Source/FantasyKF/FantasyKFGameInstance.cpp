// Fill out your copyright notice in the Description page of Project Settings.


#include "FantasyKFGameInstance.h"
#include "OnlineSessionSettings.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerState.h"
#include "FantasyKFLobbyMenu.h"
#include "FantasyKFMainMenu.h"
#include "Interfaces/OnlineUserInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"

const static FName SESSION_NAME = TEXT("FantasyKFGame");
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("FantasyKFServerName");

UFantasyKFGameInstance::UFantasyKFGameInstance(const FObjectInitializer& ObjectInitializer) {
	Maps.Add("/Game/MedievalDungeon/Maps/Demonstration?listen");
	Maps.Add("/Game/Castle/Maps/Castle?listen");
}

void UFantasyKFGameInstance::Init() {
	Super::Init();
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem != nullptr) {
		UEngine* Engine = GetEngine();
		if (!ensure(Engine != nullptr)) return;
		FString msg = TEXT("Found Subsystem: ");
		msg.Append(*Subsystem->GetSubsystemName().ToString());
		Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, msg);
		UE_LOG(LogTemp, Warning, TEXT("Found Subsystem: %s"), *Subsystem->GetSubsystemName().ToString());

		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid()) {
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UFantasyKFGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UFantasyKFGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UFantasyKFGameInstance::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UFantasyKFGameInstance::OnJoinSessionComplete);
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("No Subsystem"));
	}

	if (GEngine != nullptr) {
		GEngine->OnNetworkFailure().AddUObject(this, &UFantasyKFGameInstance::OnNetworkFailure);
	}
}

void UFantasyKFGameInstance::OnCreateSessionComplete(FName SessionName, bool Success) {
	if (!Success) {
		UE_LOG(LogTemp, Warning, TEXT("No able to create sesison"));
		return;
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;
	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	World->ServerTravel("/Game/MedievalDungeon/Maps/Lobby?listen");
}

void UFantasyKFGameInstance::OnDestroySessionComplete(FName SessionName, bool Success) {
	if (Success) {
		CreateSession();
	}
}

void UFantasyKFGameInstance::OnFindSessionsComplete(bool Success) {
	UE_LOG(LogTemp, Warning, TEXT("Sessions Found!"));
	if (Success && SessionSearch.IsValid() && MainMenu != nullptr) {
		TArray<FServerData> ServerNames;
		for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults) {
			UE_LOG(LogTemp, Warning, TEXT("Found Session Names: %s"), *SearchResult.GetSessionIdStr());
			FServerData Data;
			Data.Name = SearchResult.GetSessionIdStr();
			Data.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
			Data.CurrentPlayer = Data.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;
			Data.HostUsername = SearchResult.Session.OwningUserName;
			FString ServerName;
			if (SearchResult.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, ServerName)) {
				UE_LOG(LogTemp, Warning, TEXT("Data Found in Settings: %s"), *ServerName);
				Data.Name = ServerName;
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("Didnt get expected data"));
				Data.Name = "Could not find name.";
			}
			ServerNames.Add(Data);
		}
		MainMenu->SetServerList(ServerNames);
	}
}

void UFantasyKFGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result) {
	if (!SessionInterface.IsValid()) return;

	FString Address;
	if (!SessionInterface->GetResolvedConnectString(SessionName, Address)) {
		UE_LOG(LogTemp, Warning, TEXT("Could not get connect string"));
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;
	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("Joining: %s"), *Address));
	APlayerController* PlayerController = GetFirstLocalPlayerController();

	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel(*Address, ETravelType::TRAVEL_Absolute);
}

void UFantasyKFGameInstance::Join(uint32 Index) {
	if (!SessionInterface.IsValid()) return;
	if (!SessionSearch.IsValid()) return;

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
}

void UFantasyKFGameInstance::OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString) {
	LoadMainMenu();
}

void UFantasyKFGameInstance::StartSession() {
	if (SessionInterface.IsValid()) {
		SessionInterface->StartSession(SESSION_NAME);
	}
}


void UFantasyKFGameInstance::Host(FString ServerName)    
{
	DesiredServerName = ServerName;
	if (SessionInterface.IsValid()) {
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (ExistingSession != nullptr) {
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else {
			CreateSession();
		}
	}
}

void UFantasyKFGameInstance::CreateSession() {
	if (SessionInterface.IsValid()) {
		bool bShouldBeLAN = IOnlineSubsystem::Get()->GetSubsystemName().ToString() == "NULL";
		FOnlineSessionSettings SessionSettings;
		SessionSettings.bIsLANMatch = bShouldBeLAN;
		SessionSettings.NumPublicConnections = 4;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.bUseLobbiesIfAvailable = true;
		SessionSettings.Set(SERVER_NAME_SETTINGS_KEY, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}

void UFantasyKFGameInstance::LoadMainMenu() {
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel("/Game/MedievalDungeon/Maps/Menu", ETravelType::TRAVEL_Absolute);
}

void UFantasyKFGameInstance::UpdateLobbyPlayersList(TArray<APlayerState*> PlayerArray) {
	if (LobbyMenu != nullptr) {
		LobbyMenu->UpdateLobbyPlayersList(PlayerArray);
	}
}

void UFantasyKFGameInstance::RefreshServerList() {
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid()) {
		//SessionSearch->bIsLanQuery = true;
		SessionSearch->MaxSearchResults = 1000;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		UE_LOG(LogTemp, Warning, TEXT("Finding Sessions..."));
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

FString UFantasyKFGameInstance::GetPlayerNickname(const FUniqueNetId& UserId) {
	FString PlayerNickname;
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem != nullptr) {
		PlayerNickname = Subsystem->GetIdentityInterface()->GetPlayerNickname(UserId);
	}
	return PlayerNickname;
}

void UFantasyKFGameInstance::PrintString(FString Message) {
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;
	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, Message);
}