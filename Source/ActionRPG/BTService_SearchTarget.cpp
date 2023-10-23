// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_SearchTarget.h"


// ũ���� AI�� �����ϱ� ����
#include "CreatureAIController.h"

//�÷��̾ �����ϱ� ����
#include "ActionRPGCharacter.h"

//ĳ���Ϳ� �����ϱ� ����
#include "GameFramework/Character.h"

//������ ������Ʈ�� �����ϱ� ����
#include "BehaviorTree/BlackboardComponent.h"

//����׿����� ���� �׷��ֱ� ����
#include "DrawDebugHelpers.h"




struct A {
	int v = 1;
};

void Func() {
	A a;
	A b;
	b.v = 3;
	A& c = a;
	a = b;
}



// �Լ����� ������ ����
UBTService_SearchTarget::UBTService_SearchTarget() {

	/*������ �������� ��� �̸��� ��������*/
	NodeName = TEXT("SearchTarget");

	/*TickNode �Լ��� 1�ʿ� 60�� ȣ���ϴ� ���� �ƴϰ�, 2�ʿ� �ѹ� ȣ���ϰ� ��.*/
	Interval = 2.0f;


}

//�� ������ ȣ��Ǵ� TickNode��� �̸��� �����Լ� ����
void UBTService_SearchTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// �θ� Ŭ������ �Լ��� ������ ��� ������
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	/* AI�� ��Ʈ���ϰ� �ִ� ���� ã��*/
	APawn* CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	
	//����, AI�� ��Ʈ���ϰ� �ִ� ���� ã�� ���ߴ����
	if (CurrentPawn == nullptr) {
		// �� �̻� �� �� �ִ� ���� �����Ƿ� �Լ��� ���������ϴ�.
		return;
	}

	/*������ �ִ� ����(����)�� ã��*/
	UWorld* World = CurrentPawn->GetWorld();

	/*���� AI�� ��Ʈ�� �ϰ� �ִ� ���� ��ġ�� ã��*/
	FVector Center = CurrentPawn->GetActorLocation();

	/*5���� �ݰ濡�� �÷��̾ ã�� ����. �𸮾󿡼� ������ cm*/
	float SearchRadius = 500.0f;

	//����, ���带 ã�� ���ߴ����?
	if (World == nullptr) {
		//�Լ� ��������
		return;
	}

	// 5���� ���ʿ� ������ ���͵��� ã�Ƽ� ������ �ӽ� �迭�� ������
	/*�浹üũ
	1. ���� Ignore
	2. ��ħ Overlap
	3. ���� Block
	*/

	TArray<FOverlapResult> OverlapResults;

	/*�ɼ��Դϴ�*/
	FCollisionQueryParams QueryParams(NAME_None, false, CurrentPawn);

	/*5���� ���ʿ� ������ ���� ���͸� ã�Ҵٸ�, true�� ��ȯ��
	  ũ���ĸ� �������� 5���� �ݰ��� ���� �׷���*/

	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		//ECollisionChannel::ECC_EngineTraceChannel1,
		//FCollisionShape::MakeSphere(SearchRadius),
		ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(SearchRadius),
		QueryParams
	);

	/*����, 5���� ���ʿ� ���� ���͸� ã�Ҵٸ�,*/
	if (bResult) {
		/*ã�� ���͵��� ������� �ݺ��� ó��*/
		for (auto& OverlapResult : OverlapResults) {
			/* �÷��̾����� Ȯ���غ��� ���ؼ� �÷��̾�� ����ȯ �� ��.*/
			AActionRPGCharacter* Target = Cast<AActionRPGCharacter>(OverlapResult.GetActor());

			/*����, �÷��̾ ã�Ұ�, �÷��̾� ��Ʈ�ѷ��� �ִٸ�?*/
			if (Target && Target->GetController()->IsPlayerController()) {

				/*�����忡 �÷��̾ ã�Ҵٰ� �˷��ݴϴ�*/
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("TargetKey")), Target);

				//ã������ ����׿����� ����� ���� �׷���
				DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Green, false, 0.2f);

				/*�÷��̾ ã������ �Լ��� ���� ����*/
				return;
			}


			// �÷��̾ ã�� ���ߴٰ� �����忡 �˷���.
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("TargetKey")), nullptr);

		}
		/*ã�� �������� ����׿����� �������� ���� �׷��ݴϴ�.*/
		DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Red, false, 0.2f);
	}

	//����, 5���� ���ʿ� ������ ���͸� ã�� ���ߴ����?
	else {
		// �÷��̾ ã�� ���ߴٰ� �����忡 �˷���.
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("TargetKey")), nullptr);
		/*ã�� �������� ����׿����� �������� ���� �׷��ݴϴ�.*/
		DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Red, false, 0.2f);
	}

}
