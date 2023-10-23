// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"


//크리쳐 AIController에 접근하기 위한
#include "CreatureAIController.h"


//크리쳐에 접근하기 위한
#include "Creature.h"


//생성자 함수 정의
UBTTask_Attack::UBTTask_Attack() {

	//TickNode() 함수를 사용.
	bNotifyTick = true;


}


//ExecuteTask 가상 함수 정의
EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//부모 클래스에 있는 함수의 내용을 모두 가져옴.
	Super::ExecuteTask(OwnerComp, NodeMemory);

	//크리쳐를 찾습니다.
	ACreature* Creature = Cast<ACreature>(OwnerComp.GetAIOwner()->GetPawn());


	//만약, 크리쳐를 찾지 못했다면 ?
	if (Creature == nullptr) {

		//태스크는 실패했습니다.
		return EBTNodeResult::Failed;

	}
	//공격함.
	Creature->StartAttack();

	//이제 공격중
	bIsAttacking = true;

	/* 공격 몽타주 애니메이션이 끝나면, bIsAttacking 변수의 값을 false로 해주어서 다시 공격이 가능하도록 해 줌.
	람다 함수를 이용하여 bIsAttacking 변수의 값을 false로 초기화 해줌.*/
	Creature->OnAttackEnd.AddLambda([this]()->void {bIsAttacking = false; });

	//태스크는 진행중.
	return EBTNodeResult::InProgress;
}

//TickTask 가상 함수 정의
void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//부모 클래스에 있는 함수의 내용을 모두 가져옴.
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	//만일, 람다 함수에 의해서 bIsAttacking 의 변수의 값이 false가 되었다면?
	if (bIsAttacking == false) {
		/*FinishLatentTask() 함수가 호출되어 태스크는 성공*/
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
