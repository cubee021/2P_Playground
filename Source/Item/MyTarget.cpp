// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTarget.h"
#include "Components/SplineComponent.h"

// Sets default values
AMyTarget::AMyTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));

	SplineComponent->SetupAttachment(RootComponent);
	SplineComponent->Duration = TotalPathTime;
	SplineComponent->bDrawDebug = true;
	
	static ConstructorHelpers::FClassFinder<AActor> ActorToMoveRef(TEXT("/Script/Engine.Blueprint'/Game/Project2/Blueprint/BP_TargetMesh.BP_TargetMesh_C'"));
	if (ActorToMoveRef.Class)
	{
		ActorToMoveClass = ActorToMoveRef.Class;
	}
	
	bSplineInLoop = true;
	TargetSize = FVector(1.f, 1.f, 1.f);
}

// Called when the game starts or when spawned
void AMyTarget::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		ActorToMove = GetWorld()->SpawnActor<AActor>(ActorToMoveClass, SplineComponent->GetComponentTransform());
		if (ActorToMove)
		{
			ActorToMove->SetActorScale3D(TargetSize);
			StartTime = GetWorld()->GetTimeSeconds();
		}
	
	}
}

// Called every frame
void AMyTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ActorToMove)
	{
		float CurrentSplineTime = (GetWorld()->GetTimeSeconds() - StartTime) / TotalPathTime;
		float Distance = SplineComponent->GetSplineLength() * CurrentSplineTime;

		FVector Position = SplineComponent->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
		ActorToMove->SetActorLocation(Position);

		if (CurrentSplineTime >= 1.f)
		{
			if (bSplineInLoop)
			{
				StartTime = GetWorld()->GetTimeSeconds();
				CurrentSplineTime = (GetWorld()->GetTimeSeconds() - StartTime) / TotalPathTime;
			}
		}
	}

}

