// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Patrol.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API UBTTask_Patrol : public UBTTask_BlackboardBase
{
	GENERATED_BODY()


public:
	//테스트(일)을 수행하는 가상함수의 원형 선언.
	/*Aborted : 태스크 실행 중에 중단. 결과적으로는 실패. 
	  Failed : 태스크를 수행했지만 실패.
	  Succeeded : 태스크를 성공적으로 수행.
	  InProgress : 태스크를 계속 수행하고 있음. 태스크의 실행 결과 향후 알려줌.*/

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	
};
