// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CreatureAIController.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API ACreatureAIController : public AAIController
{
	GENERATED_BODY()

public:
	//생성자 선언
	ACreatureAIController();

	//컨트롤러가 폰을 빙의하는 가상 함수의 원형을 선언
	virtual void OnPossess(APawn* InPawn) override;

	//컨트롤러가 폰을 빙의해제 하는 함수 원형 선언
	virtual void OnUnPossess() override;

	//타겟을 향해 회전시켜주는 가상함수 원형 선언
	virtual void UpdateControlRotation(float DeltaTime, bool bUpdatePawn) override;


private:


	/*Transient 키워드
	-> 언리얼 오브젝트에는 직렬화 기능이 있어서 오브젝트의 UPROPERTY 속성을 저장하고 로딩할 수 있음.
	컴포넌트 스탯 중 CurrentHP (현재 가지고 있는 생명력) 같은 값은 게임을 시작할 때마다 변경되므로 이 값을 보관하는 것은 의미가 없고,
	오히려, 오브젝트를 저장할 때 필요없는 디스크 공간만 차지하게 됨.
	이러한 속성에는 Transient(휘발성) 키워드를 추가해 해당 속성을 직렬화에서 제외시키는 것을 의미함.
	*/
	UPROPERTY(Transient, meta =(AllowPrivateAccess = true))
	class UBehaviorTreeComponent* BehaviorTreeComponent;

	//BlackboardComponent를 저장할 변수를 선언함.
	UPROPERTY(Transient, meta = (AllowPrivateAccess = true))
	class UBlackboardComponent* BlackboardComponent;

	//크리쳐 입장에서 적은 플레이어임. 적의 ID를 저장해 둘 변수를 선언.
	int32 TargetID;




};
