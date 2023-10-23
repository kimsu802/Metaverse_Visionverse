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
	/*생성자 함수의 원형을 선언*/
	UBTDecorator_CanAttack();

	/*조건을 검사하는 가상함수의 원형을 선언*/
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;


};
