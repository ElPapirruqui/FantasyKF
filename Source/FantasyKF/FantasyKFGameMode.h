// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FantasyKFGameMode.generated.h"

class AEnemy;

USTRUCT(BlueprintType)
struct FSpawnProperties
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AEnemy> EnemyType;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Quantity;
};

UCLASS(minimalapi)
class AFantasyKFGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFantasyKFGameMode();

	UFUNCTION(BlueprintCallable)
	void OrderVectorArray(FVector Origin, TArray<FVector> VectorsArray, TArray<FVector>& OrderedArray);

	UFUNCTION(BlueprintCallable)
	void RestartGame();

	UPROPERTY(BlueprintReadWrite)
	TArray<AController*> PlayerControllers;

private:
	void HandleSeamlessTravelPlayer(AController*& Controller) override;
	void PostLogin(APlayerController* NewPlayer) override;
	void Logout(AController* Exiting) override;
};



