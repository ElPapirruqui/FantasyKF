// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "FKFGameState.generated.h"

UENUM(BlueprintType)
enum EGameStatus
{
	WaveCompleted       UMETA(DisplayName = "WaveCompleted"),
	NextWave			UMETA(DisplayName = "NextWave"),
	FinalWave			UMETA(DisplayName = "FinalWave"),
	Win					UMETA(DisplayName = "Win"),
};
/**
 * 
 */
UCLASS()
class FANTASYKF_API AFKFGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, ReplicatedUsing = OnRep_EnemiesLeft)
	int32 EnemiesLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, Replicated)
	int32 Wave = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, ReplicatedUsing = OnRep_CurrentGameStatus)
	TEnumAsByte<EGameStatus> CurrentGameStatus;

	UFUNCTION(BlueprintImplementableEvent)
	void OnGameStatusChange();

	UFUNCTION(BlueprintImplementableEvent)
	void OnEnemiesUpdated();

private:
	UFUNCTION()
	void OnRep_CurrentGameStatus();

	UFUNCTION()
	void OnRep_EnemiesLeft();
};
