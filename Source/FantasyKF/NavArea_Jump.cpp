// Fill out your copyright notice in the Description page of Project Settings.


#include "NavArea_Jump.h"
#include "NavAreaHelper.h"

UNavArea_Jump::UNavArea_Jump(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	UNavAreaHelper::Set(AreaFlags, ENavAreaFlag::Jump);
}