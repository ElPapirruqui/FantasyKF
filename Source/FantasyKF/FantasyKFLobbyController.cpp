// Fill out your copyright notice in the Description page of Project Settings.


#include "FantasyKFLobbyController.h"
#include "Camera/CameraActor.h"
#include "EngineUtils.h"
#include "FantasyKFGameInstance.h"
#include "TimerManager.h"
#include "FantasyKFLobbyPlayerState.h"

void AFantasyKFLobbyController::BeginPlay() {
    Super::BeginPlay();
    if (HasAuthority() && IsLocalController()) {
        AFantasyKFLobbyPlayerState* FKFPlayerState = GetPlayerState<AFantasyKFLobbyPlayerState>();
        if (FKFPlayerState == nullptr) return;
        auto UniqueId = FKFPlayerState->GetUniqueId();

        if (!UniqueId.IsValid()) return;

        UFantasyKFGameInstance* FantasyKFGameInstance = GetFKFGameInstance();
        if (FantasyKFGameInstance == nullptr) return;

        FString PlayerNickname = FantasyKFGameInstance->GetPlayerNickname(*UniqueId);
        ServerUpdateName(PlayerNickname);
    }
}

void AFantasyKFLobbyController::OnPossess(APawn* InPawn) {
    Super::OnPossess(InPawn);
    SetUpLobby();
}

void AFantasyKFLobbyController::AcknowledgePossession(APawn* InPawn) {
    Super::AcknowledgePossession(InPawn);
    SetUpLobby();
}

void AFantasyKFLobbyController::SetUpLobby() {
    UE_LOG(LogTemp, Warning, TEXT("GENERAL"));
    if (IsLocalController()) {
        UE_LOG(LogTemp, Warning, TEXT("LOCAL"));
        for (TActorIterator<AActor> ActorIterator(GetWorld()); ActorIterator; ++ActorIterator) {
            AActor* Actor = *ActorIterator;
            // ensure actor is not null
            // ignore self if found
            // ensure we find actors of a specific interface only
            if (Actor->ActorHasTag(FName(TEXT("LobbyCamera")))) {
                UE_LOG(LogTemp, Warning, TEXT("CAMERA FOUND"));
                LobbyCamera = Cast<ACameraActor>(Actor);
                GetWorldTimerManager().SetTimer(GetStartTimer, this, &AFantasyKFLobbyController::SetCamera, 0.1f);
            }

            if (Actor->ActorHasTag(FName(TEXT("MainMenuBlood")))) {
                UE_LOG(LogTemp, Warning, TEXT("BLOOD FOUND"));
                Actor->CustomTimeDilation = 0.3f;
            }

            UFantasyKFGameInstance* FantasyKFGameInstance = GetFKFGameInstance();
            if (FantasyKFGameInstance != nullptr) {
                FantasyKFGameInstance->CallSetLobbyMenu();
            }
        }
    }
}

void AFantasyKFLobbyController::SetCamera() {
    if (LobbyCamera == nullptr) return;
    UE_LOG(LogTemp, Warning, TEXT("CAMERA SET"));
    SetViewTargetWithBlend(LobbyCamera);
}

UFantasyKFGameInstance* AFantasyKFLobbyController::GetFKFGameInstance() {
    UFantasyKFGameInstance* FantasyKFGameInstance = nullptr;

    UWorld* World = GetWorld();
    if (World == nullptr) FantasyKFGameInstance;

    UGameInstance* GameInstance = World->GetGameInstance();
    if (GameInstance == nullptr) return FantasyKFGameInstance;

    FantasyKFGameInstance = Cast<UFantasyKFGameInstance>(GameInstance);

    return FantasyKFGameInstance;
}

void AFantasyKFLobbyController::OnRep_PlayerState() {
    AFantasyKFLobbyPlayerState* FKFPlayerState = GetPlayerState<AFantasyKFLobbyPlayerState>();
    if (FKFPlayerState == nullptr) return;
    auto UniqueId = FKFPlayerState->GetUniqueId();

    if (!UniqueId.IsValid()) return;

    UFantasyKFGameInstance* FantasyKFGameInstance = GetFKFGameInstance();
    if (FantasyKFGameInstance == nullptr) return;

    FString PlayerNickname = FantasyKFGameInstance->GetPlayerNickname(*UniqueId);
    ServerUpdateName(PlayerNickname);
}

void AFantasyKFLobbyController::ServerUpdateName_Implementation(const FString& PlayerNickname) {
    AFantasyKFLobbyPlayerState* FKFPlayerState = GetPlayerState<AFantasyKFLobbyPlayerState>();
    if (FKFPlayerState == nullptr) return;

    FKFPlayerState->SetOnlineIdentityName(PlayerNickname);
}

bool AFantasyKFLobbyController::ServerUpdateName_Validate(const FString& PlayerNickname) {
    return true;
}