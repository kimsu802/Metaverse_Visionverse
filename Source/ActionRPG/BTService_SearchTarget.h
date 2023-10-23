// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_SearchTarget.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API UBTService_SearchTarget : public UBTService
{
	GENERATED_BODY()


public:
	//생성자 함수 원형을 선언.
	UBTService_SearchTarget();

	// 매 프레임 호출되는 TickNode라는 이름의 가상함수의 원형을 선언.
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	
};
