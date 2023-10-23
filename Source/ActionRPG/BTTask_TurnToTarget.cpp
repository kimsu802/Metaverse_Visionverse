// Fill out your copyright notice in the Description page of Project Settings.

//ũ���� AIController�� �����ϱ� ����
#include "CreatureAIController.h"

#include "BTTask_TurnToTarget.h"

//ũ���Ŀ��� �����ϱ� ����
#include "Creature.h"

//�÷��̾�� �����ϱ� ����
#include "ActionRPGCharacter.h"

//������ ������Ʈ�� �����ϱ� ����
#include "BehaviorTree/BlackboardComponent.h"

//������ �Լ� ����
UBTTask_TurnToTarget::UBTTask_TurnToTarget() {

	// �ൿƮ������ ������ ����̸��� ������ �ݴϴ�.
	NodeName = TEXT("TurnToTarget");


}
//Task�� �����ϴ� �����Լ��� ����
EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	/*�θ� Ŭ������ �Լ��� ������ ��� �����ɴϴ�.*/
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	//ũ���ĸ� ã���ϴ�.
	ACreature* Creature = Cast<ACreature>(OwnerComp.GetAIOwner()->GetPawn());

	//����, ũ���ĸ� ã�� ���ߴٸ�?
	if (Creature == nullptr) {

		//Task�� ������.
		return EBTNodeResult::Failed;

	}

	//�÷��̾ ã���ϴ�.
	AActionRPGCharacter* Target = Cast<AActionRPGCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("TargetKey"))));

	//����, �÷��̾ ã�� ���ߴٸ�?
	if (Target == nullptr) {
		return EBTNodeResult::Failed;
	}

	//ũ���Ŀ��� �÷��̾�� ���ϴ� ����� �Ÿ��� ����.
	FVector Direction = Target->GetActorLocation() - Creature->GetActorLocation();
	Direction.Z = 0.0f; //���̴� �ʿ� �����Ƿ� 0���� ����

	//���⺤�ͷκ��� ������ �����ϴ�.
	FRotator TargetRot = FRotationMatrix::MakeFromX(Direction).Rotator();

	//ũ���Ŀ��� �÷��̾�� �ε巴�� ȸ���ϴ� ȸ������ ���ؿ�.
	//FMath::RInterpTo : Rotation Interpolation, ȸ�� ����.
	FRotator RotationTo = FMath::RInterpTo(Creature->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(),2.0f);

	//Ÿ���� ���� ȸ������ ����.
	Creature->SetActorRotation(RotationTo);

	//�׽�ũ�� ����
	return EBTNodeResult::Succeeded;
}
