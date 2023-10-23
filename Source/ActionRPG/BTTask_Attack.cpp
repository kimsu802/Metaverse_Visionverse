// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"


//ũ���� AIController�� �����ϱ� ����
#include "CreatureAIController.h"


//ũ���Ŀ� �����ϱ� ����
#include "Creature.h"


//������ �Լ� ����
UBTTask_Attack::UBTTask_Attack() {

	//TickNode() �Լ��� ���.
	bNotifyTick = true;


}


//ExecuteTask ���� �Լ� ����
EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//�θ� Ŭ������ �ִ� �Լ��� ������ ��� ������.
	Super::ExecuteTask(OwnerComp, NodeMemory);

	//ũ���ĸ� ã���ϴ�.
	ACreature* Creature = Cast<ACreature>(OwnerComp.GetAIOwner()->GetPawn());


	//����, ũ���ĸ� ã�� ���ߴٸ� ?
	if (Creature == nullptr) {

		//�½�ũ�� �����߽��ϴ�.
		return EBTNodeResult::Failed;

	}
	//������.
	Creature->StartAttack();

	//���� ������
	bIsAttacking = true;

	/* ���� ��Ÿ�� �ִϸ��̼��� ������, bIsAttacking ������ ���� false�� ���־ �ٽ� ������ �����ϵ��� �� ��.
	���� �Լ��� �̿��Ͽ� bIsAttacking ������ ���� false�� �ʱ�ȭ ����.*/
	Creature->OnAttackEnd.AddLambda([this]()->void {bIsAttacking = false; });

	//�½�ũ�� ������.
	return EBTNodeResult::InProgress;
}

//TickTask ���� �Լ� ����
void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//�θ� Ŭ������ �ִ� �Լ��� ������ ��� ������.
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	//����, ���� �Լ��� ���ؼ� bIsAttacking �� ������ ���� false�� �Ǿ��ٸ�?
	if (bIsAttacking == false) {
		/*FinishLatentTask() �Լ��� ȣ��Ǿ� �½�ũ�� ����*/
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
