// Fill out your copyright notice in the Description page of Project Settings.


#include "FantasyKFServerRow.h"
#include "FantasyKFMainMenu.h"
#include "Components/Button.h"

void UFantasyKFServerRow::Setup(UFantasyKFMainMenu* _Parent, uint32 _Index) {
	Parent = _Parent;
	Index = _Index;
	RowButton->OnClicked.AddDynamic(this, &UFantasyKFServerRow::OnClicked);
}

void UFantasyKFServerRow::OnClicked() {
	Parent->SelectIndex(Index);
}