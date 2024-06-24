// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyTargetMesh.generated.h"

UCLASS()
class PROJECT2_API AMyTargetMesh : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyTargetMesh();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, Category = Mesh)
		TObjectPtr<class UBoxComponent> Trigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
		TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, Category = Effect)
		TObjectPtr<class UParticleSystemComponent> Effect;

	/** Mesh lies back when overlaped */
	void LeanBackward();

	FTimerHandle LeanBackTimerhandle;
	/** How long this actor will be lie down before get up */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Duration)
	float LieDuration = 4.f;

protected:
	/** Damage count this actor gets before lie back */
	int32 DamageCount = 0;
	/** When DamageCount == MaxDamageCount, this actor will lie back */
	int32 MaxDamageCount = 3;

public:
	/** If true, this actor is attackable */
	UPROPERTY(ReplicatedUsing = OnRep_HasDamagedMax)
	uint8 bAttackable : 1;

public:
	/**
	* Server Section */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
		void OnRep_HasDamagedMax();
};
