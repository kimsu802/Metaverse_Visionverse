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
	//�׽�Ʈ(��)�� �����ϴ� �����Լ��� ���� ����.
	/*Aborted : �½�ũ ���� �߿� �ߴ�. ��������δ� ����. 
	  Failed : �½�ũ�� ���������� ����.
	  Succeeded : �½�ũ�� ���������� ����.
	  InProgress : �½�ũ�� ��� �����ϰ� ����. �½�ũ�� ���� ��� ���� �˷���.*/

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	
};
