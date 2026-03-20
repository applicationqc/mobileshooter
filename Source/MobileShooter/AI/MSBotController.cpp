// Copyright 2024 MobileShooter. All Rights Reserved.

#include "AI/MSBotController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/MobileShooterCharacter.h"
#include "Perception/AIPerceptionComponent.h"

const FName AMSBotController::EnemyTargetKey     = TEXT("EnemyTarget");
const FName AMSBotController::PatrolLocationKey  = TEXT("PatrolLocation");
const FName AMSBotController::bCanSeeEnemyKey    = TEXT("bCanSeeEnemy");

AMSBotController::AMSBotController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	BlackboardComponent   = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

	// Set up AI perception (sight)
	UAIPerceptionComponent* Perception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetPerceptionComponent(*Perception);
}

void AMSBotController::BeginPlay()
{
	Super::BeginPlay();
}

void AMSBotController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BotBehaviorTree)
	{
		BlackboardComponent->InitializeBlackboard(*BotBehaviorTree->BlackboardAsset);
		BehaviorTreeComponent->StartTree(*BotBehaviorTree);
	}
}

void AMSBotController::SetEnemyTarget(AActor* Enemy)
{
	if (BlackboardComponent)
	{
		BlackboardComponent->SetValueAsObject(EnemyTargetKey, Enemy);
		BlackboardComponent->SetValueAsBool(bCanSeeEnemyKey, Enemy != nullptr);
	}
}

void AMSBotController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		SetEnemyTarget(Actor);
	}
	else
	{
		SetEnemyTarget(nullptr);
	}
}
