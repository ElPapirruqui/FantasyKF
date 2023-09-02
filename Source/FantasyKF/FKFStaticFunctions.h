// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FKFStaticFunctions.generated.h"

/**
 * 
 */
UCLASS()
class FANTASYKF_API UFKFStaticFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, Category = "Helpers")
	static void OrderByDistance(AActor* originActor, TArray<AActor*> actorsArray, TArray<AActor*>& orderedArray);

	UFUNCTION(BlueprintCallable, Category = "Helpers")
	static void OrderByHeight(TArray<AActor*> actorsArray, TArray<AActor*>& orderedArray);

	UFUNCTION(BlueprintCallable, Category = "Helpers")
	static void FilterByHeight(float MinHeight, TArray<AActor*> actorsArray, TArray<AActor*>& orderedArray);
};
