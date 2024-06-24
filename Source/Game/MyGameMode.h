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
// ȭ�� ��ܿ� ���� ��� �ð��� ǥ���ϴ� UMyMatchTimerWidget�� RemainingTime�� 
// �������̽��� ���� �����ϰ� �;��µ� �� �ȵż� �ּ�ó��.
// UMyMatchTimerWidget���� PlayerPawn -> Interface cast�� �ȵ�
// 
	//virtual void SetupHUDWidget(class UMyMatchTimerWidget* InHUDWidget);

};
