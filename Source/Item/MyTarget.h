// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyTarget.generated.h"

UCLASS()
class PROJECT2_API AMyTarget : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyTarget();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SplineController")
		TObjectPtr<class USplineComponent> SplineComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ActorToMove")
		TSubclassOf<class AActor> ActorToMoveClass;

	/** This Actor will move along spline path*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ActorToMove")
		class AActor* ActorToMove;
	/** Total time actor will travel from start to end */
	UPROPERTY(EditAnywhere, Category = "SplineController")
		float TotalPathTime = 10.f;
	/** If true, allow loop */
	UPROPERTY(EditAnywhere, Category = "SplineController")
		bool bSplineInLoop;
	/** Time Actor will start traveling */
	float StartTime;

protected:
	UPROPERTY(EditAnywhere, Category = "ActorToMove")
		FVector TargetSize;

};
