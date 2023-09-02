// Fill out your copyright notice in the Description page of Project Settings.


#include "FKFStaticFunctions.h"

void UFKFStaticFunctions::OrderByDistance(AActor* originActor, TArray<AActor*> actorsArray, TArray<AActor*>& orderedArray) {
	actorsArray.Sort ([originActor](const AActor& A, const AActor& B) {
		float DistanceA = A.GetDistanceTo(originActor);
		float DistanceB = B.GetDistanceTo(originActor);
		return DistanceA <= DistanceB;
		});
	orderedArray = actorsArray;
}

void UFKFStaticFunctions::OrderByHeight(TArray<AActor*> actorsArray, TArray<AActor*>& orderedArray) {
	actorsArray.Sort([](const AActor& A, const AActor& B) {
		float DistanceA = A.GetActorLocation().Z;
		float DistanceB = B.GetActorLocation().Z;
		return DistanceA <= DistanceB;
		});
	orderedArray = actorsArray;
}

void UFKFStaticFunctions::FilterByHeight(float MinHeight, TArray<AActor*> actorsArray, TArray<AActor*>& orderedArray) {
	orderedArray = actorsArray.FilterByPredicate([MinHeight](const AActor* Act) {
		bool bIsHigher = (Act->GetActorLocation().Z) >= MinHeight;
		return bIsHigher;
	});
}