// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NavAreaHelper.generated.h"

UENUM()
enum ENavAreaFlag
{
	Default		UMETA(DisplayName = "Default"),
	Jump		UMETA(DisplayName = "Jump"),
	Jump2		UMETA(DisplayName = "Jump2"),
};
/**
 * 
 */
UCLASS()
class FANTASYKF_API UNavAreaHelper : public UObject
{
	GENERATED_BODY()
	
public:
	static bool HasFlag(const uint16& PolyFlags, ENavAreaFlag Flag);
	static bool IsSet(uint16 Flags, ENavAreaFlag Bit);
	static bool IsNavLink(const FNavPathPoint& PathVert);
	static void Set(uint16& Flags, ENavAreaFlag Bit);
};
