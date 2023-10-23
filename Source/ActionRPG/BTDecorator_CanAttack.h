// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CanAttack.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API UBTDecorator_CanAttack : public UBTDecorator
{
	GENERATED_BODY()

public:
	/*������ �Լ��� ������ ����*/
	UBTDecorator_CanAttack();

	/*������ �˻��ϴ� �����Լ��� ������ ����*/
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;


};
