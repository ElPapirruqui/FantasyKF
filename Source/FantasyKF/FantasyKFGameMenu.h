// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FantasyKFGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class FANTASYKF_API UFantasyKFGameMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void QuitGame();
};
