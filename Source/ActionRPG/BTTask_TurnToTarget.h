// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TurnToTarget.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API UBTTask_TurnToTarget : public UBTTaskNode
{
	GENERATED_BODY()
	


public:
	/*생성자 함수의 원형을 선언*/
	UBTTask_TurnToTarget();

	//Task를 실행하는 가상함수의 원형을 선언
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
