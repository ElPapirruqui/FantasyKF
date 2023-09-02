// Fill out your copyright notice in the Description page of Project Settings.


#include "NavAreaHelper.h"
#include "NavMesh/NavMeshPath.h"

bool UNavAreaHelper::IsSet(uint16 Flags, ENavAreaFlag Bit) {
	return (Flags & (1 << Bit)) != 0;
};

void UNavAreaHelper::Set(uint16& Flags, ENavAreaFlag Bit) {
	Flags |= (1 << Bit);
};

bool UNavAreaHelper::IsNavLink(const FNavPathPoint& PathVert) {
	return (FNavMeshNodeFlags(PathVert.Flags).PathFlags & RECAST_STRAIGHTPATH_OFFMESH_CONNECTION) != 0;
};

bool UNavAreaHelper::HasFlag(const uint16& PolyFlags, ENavAreaFlag Flag) {
	return IsSet(PolyFlags, Flag);
};