// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CanAttack.h"

//ũ���� AI ��Ʈ�ѷ��� �����ϱ� ����
#include "CreatureAIController.h"

//�÷��̾�� �����ϱ� ����
#include "ActionRPGCharacter.h"

//������ ������Ʈ�� �����ϱ� ����
#include "BehaviorTree/BlackboardComponent.h"



//������ �Լ� ����
UBTDecorator_CanAttack::UBTDecorator_CanAttack() {

	//����� �̸� ����
	NodeName = TEXT("CanAttack");
}

//������ �˻��ϴ� �����Լ� ����
bool UBTDecorator_CanAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	//�θ� Ŭ������ �Լ� ������ ��� ������.
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	//AI�� ��Ʈ���ϰ� �ִ� ���� ������.
	APawn* CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();

	//����, ���� ã�� ���ߴٸ�?
	if (CurrentPawn == nullptr) {

		//���� �Ұ���
		return false;
	}

	//�÷��̾ ã��.
	AActionRPGCharacter* Target = Cast<AActionRPGCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("TargetKey"))));

	//����, �÷��̾ ã�� ���ߴٸ�?
	if (Target == nullptr) {

		//���� �Ұ���
		return false;
	}

	/*�÷��̾���� �Ÿ��� 2m �������� ��ȯ. �𸮾��� ������ cm��.*/
	return bResult && Target->GetDistanceTo(CurrentPawn) <= 200.0f;

}
