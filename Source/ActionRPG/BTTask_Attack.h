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

	//������ �Լ��� ������ ����
	UBTTask_Attack();

	//Task�� �����ϴ� �����Լ��� ������ ������.
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	//TickTask �����Լ��� ������ ����
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	//���������� �Ǻ��� �� �Ҹ��� ������ ����.
	bool bIsAttacking = false;
	
};
