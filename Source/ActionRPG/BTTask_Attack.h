// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Attack.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API UBTTask_Attack : public UBTTaskNode
{
	GENERATED_BODY()
public:

	//생성자 함수의 원형을 선언
	UBTTask_Attack();

	//Task를 실행하는 가상함수의 원형을 선언함.
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	//TickTask 가상함수의 원형을 선언
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	//공격중인지 판별해 줄 불리언 변수를 선언.
	bool bIsAttacking = false;
	
};
