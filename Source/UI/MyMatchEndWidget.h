// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyMatchEndWidget.generated.h"

/**
 * HUD that will be shown when match ends
 * 두 플레이어의 점수를 화면에 띄우려고 함
 * 문제점 1 : PlayerState의 Score 값이 갱신되지 않아  항상 0
 */
UCLASS()
class PROJECT2_API UMyMatchEndWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UMyMatchEndWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
		TObjectPtr<class UTextBlock> PlayerScore1;

	UPROPERTY()
		TObjectPtr<class UTextBlock> PlayerScore2;
	/** Score of Server (PlayerController_0) */
	UPROPERTY()
		float EndScore1 = 0;
	/** Score of Server (PlayerController_1) */
	UPROPERTY()
		float EndScore2 = 0;

public:
	/** Show screen with player's end scores */
	void ViewScores();
	FString GetScoreText(float PlayerNumber);
};
