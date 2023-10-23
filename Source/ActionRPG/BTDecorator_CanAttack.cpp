// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CanAttack.h"

//크리쳐 AI 컨트롤러에 접근하기 위한
#include "CreatureAIController.h"

//플레이어에게 접근하기 위한
#include "ActionRPGCharacter.h"

//블랙보드 컴포넌트에 접근하기 위한
#include "BehaviorTree/BlackboardComponent.h"



//생성자 함수 정의
UBTDecorator_CanAttack::UBTDecorator_CanAttack() {

	//노드의 이름 지정
	NodeName = TEXT("CanAttack");
}

//조건을 검사하는 가상함수 정의
bool UBTDecorator_CanAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	//부모 클래스의 함수 내용을 모두 가져옴.
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	//AI가 컨트롤하고 있는 폰을 가져옴.
	APawn* CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();

	//만약, 폰을 찾지 못했다면?
	if (CurrentPawn == nullptr) {

		//공격 불가능
		return false;
	}

	//플레이어를 찾음.
	AActionRPGCharacter* Target = Cast<AActionRPGCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("TargetKey"))));

	//만약, 플레이어를 찾지 못했다면?
	if (Target == nullptr) {

		//공격 불가능
		return false;
	}

	/*플레이어와의 거리가 2m 안쪽인지 반환. 언리얼은 단위가 cm임.*/
	return bResult && Target->GetDistanceTo(CurrentPawn) <= 200.0f;

}
