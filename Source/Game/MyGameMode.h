// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Project2/Interface/MyGameInterface.h"
#include "Project2/Interface/MyCharacterHUDInterface.h"
#include "MyGameMode.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE(FOnEndMatchDelegate)

UCLASS()
class PROJECT2_API AMyGameMode : public AGameMode, public IMyGameInterface //, public IMyCharacterHUDInterface
{
	GENERATED_BODY()

public:
	AMyGameMode();

	virtual FTransform GetRandomStartTransform() const;
	/** Add player's score when player killed other player */
	virtual void OnPlayerKilled(AController* Killer, AController* KilledPlayer, APawn* KilledPawn);

	virtual void StartPlay() override;
	
public:
	FOnEndMatchDelegate OnEndMatchDelegate;

protected:
	virtual void PostInitializeComponents() override;
	virtual void DefaultGameTimer();
	/** Broadcast tha match has end */
	void FinishMatch();

	FTimerHandle GameTimerHandle;

protected:
	TArray<TObjectPtr<class APlayerStart>> PlayerStartArray;

//public:
// 화면 상단에 남은 경기 시간을 표시하는 UMyMatchTimerWidget에 RemainingTime을 
// 인터페이스를 통해 전달하고 싶었는데 잘 안돼서 주석처리.
// UMyMatchTimerWidget에서 PlayerPawn -> Interface cast가 안됨
// 
	//virtual void SetupHUDWidget(class UMyMatchTimerWidget* InHUDWidget);

};
