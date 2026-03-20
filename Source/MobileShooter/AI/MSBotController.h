// Copyright 2024 MobileShooter. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MSBotController.generated.h"

class AMobileShooterCharacter;
class UBehaviorTreeComponent;
class UBlackboardComponent;

/**
 * AI controller for bot players.
 * Uses a Behavior Tree + Blackboard to drive navigation, targeting, and shooting.
 */
UCLASS(Blueprintable)
class MOBILESHOOTER_API AMSBotController : public AAIController
{
	GENERATED_BODY()

public:
	AMSBotController();

	// ─── Behavior Tree ────────────────────────────────────────────────────

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	TObjectPtr<class UBehaviorTree> BotBehaviorTree;

	// ─── Blackboard Keys ──────────────────────────────────────────────────

	static const FName EnemyTargetKey;
	static const FName PatrolLocationKey;
	static const FName bCanSeeEnemyKey;

	// ─── Interface ────────────────────────────────────────────────────────

	/** Update the blackboard's enemy target */
	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetEnemyTarget(AActor* Enemy);

	/** Called by perception system when a pawn is detected */
	UFUNCTION(BlueprintCallable, Category = "AI")
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	// ─── Overrides ────────────────────────────────────────────────────────

	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UBlackboardComponent> BlackboardComponent;
};
