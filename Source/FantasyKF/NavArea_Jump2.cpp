// Fill out your copyright notice in the Description page of Project Settings.


#include "NavArea_Jump2.h"
#include "NavAreaHelper.h"

UNavArea_Jump2::UNavArea_Jump2(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	UNavAreaHelper::Set(AreaFlags, ENavAreaFlag::Jump2);
}