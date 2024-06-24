// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerState.h"
#include "Project2/Player/MyPlayerController.h"
#include "Project2/UI/MyMatchTimerWidget.h"
#include "MyGameState.h"
#include "EngineUtils.h"

AMyGameMode::AMyGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Script/Engine.Blueprint'/Game/Project2/Blueprint/BP_MyCharacterPlayer.BP_MyCharacterPlayer_C'"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}

	PlayerControllerClass = AMyPlayerController::StaticClass();
	GameStateClass = AMyGameState::StaticClass();
}

void AMyGameMode::StartPlay()
{
	Super::StartPlay();

	for (APlayerStart* PlayerStart : TActorRange<APlayerStart>(GetWorld()))
	{
		PlayerStartArray.Add(PlayerStart);
	}
}

void AMyGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	GetWorldTimerManager().SetTimer(GameTimerHandle, this, &AMyGameMode::DefaultGameTimer, GetWorldSettings()->GetEffectiveTimeDilation(), true);
}

void AMyGameMode::DefaultGameTimer()
{
	AMyGameState* const MyGameState = Cast<AMyGameState>(GameState);
	if (MyGameState && MyGameState->RemainingTime > 0)
	{
		MyGameState->RemainingTime--;

		//UE_LOG(LogTemp, Warning, TEXT("Remaining Time : %d"), MyGameState->RemainingTime);

		if (MyGameState->RemainingTime <= 0)
		{
			if (GetMatchState() == MatchState::InProgress)
			{
				FinishMatch();
			}
		}
	}

}

void AMyGameMode::FinishMatch()
{
	AMyGameState* const MyGameState = Cast<AMyGameState>(GameState);
	if (MyGameState && IsMatchInProgress())
	{
		EndMatch();

		OnEndMatchDelegate.Broadcast();
	}
}

FTransform AMyGameMode::GetRandomStartTransform() const
{
	if (PlayerStartArray.Num() == 0)
	{
		return FTransform(FVector(0.f, 0.f, 230.f));
	}

	int32 RandIndex = FMath::RandRange(0, PlayerStartArray.Num() - 1);

	return PlayerStartArray[RandIndex]->GetActorTransform();
}

void AMyGameMode::OnPlayerKilled(AController* Killer, AController* KilledPlayer, APawn* KilledPawn)
{
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *Killer->GetPathName());

	APlayerState* KillerPlayerState = Killer->PlayerState;
	if (KillerPlayerState)
	{
		/**
		* PlayerState의 SetScore()를 해도 Score에 값이 저장되지 않음
		* 해결 못함..
		*/
		KillerPlayerState->SetScore(KillerPlayerState->GetScore() + 1);
		UE_LOG(LogTemp, Warning, TEXT("Player current score :  %d"), KillerPlayerState->GetScore());
	}
}

/*
void AMyGameMode::SetupHUDWidget(UMyMatchTimerWidget* InHUDWidget)
{
	if (InHUDWidget)
	{
		AMyGameState* const MyGameState = Cast<AMyGameState>(GameState);
		if (MyGameState)
		{
			InHUDWidget->UpdateTimer(MyGameState->RemainingTime);
		}
	}
}
*/
