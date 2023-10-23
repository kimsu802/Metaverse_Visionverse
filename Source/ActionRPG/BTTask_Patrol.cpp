// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Patrol.h"

//AI ���� ���̺귯��
#include "BehaviorTree/BTNode.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"




//��ã�� ���̺귯��
#include "NavigationSystem.h"

//CreatureAIController�� �����ϱ� ����
#include "CreatureAIController.h"






//�׽�Ʈ(��)�� �����ϴ� �����Լ��� ���� ����.
EBTNodeResult::Type UBTTask_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	//NavigationSystem�� ������.
	UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	
	//ũ������ AIController�� ������
	ACreatureAIController* CreatureAIController = Cast<ACreatureAIController>(OwnerComp.GetAIOwner());

	//ĳ��Ʈ �ϸ�, if������ Ȯ�����ִ� ���� ����.
	//����, ��ã�� �ý����� ã�ư�, ũ������ AIController�� ã�Ұ� ũ���İ� ��Ʈ���ϰ� �ִ� ���� ã�Ҵٸ�?

	if (NavigationSystem && CreatureAIController && CreatureAIController->GetPawn()) {
		/*AIController�� ��Ʈ���ϰ� �ִ� ���� ������.*/
		APawn* CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();

		//Patrol �ݰ��� 6���ͷ� ������. �𸮾��� ������ cm��.
		const float PatrolRadius = 600.0f;

		/*�ӽ� ��Ʈ�� ��ġ�� ������ �� ������ ����*/
		FNavLocation PatrolLocation;

		/*��ġ�� ã�Ҵ��� �Ǻ����ִ� �Ҹ��� ���� ����
		6���� �ݰ濡�� ������ ��ġ�� ã��
		GetRandomPointInNavagableRadius() : �ݰ�ȿ��� ������ ��ġ�� ã��.
		CreatureAIController->GetPawn()->GetActorLocation() : AIController�� ��Ʈ���ϰ� �ִ� ���� ��ġ.
		PatrolRadius : ã�� �ݰ�.
		PatrolLocation : ã���� �� ��ġ�� ������ �� ����.
		*/

		const bool bIsFound = NavigationSystem->GetRandomPointInNavigableRadius(
		CreatureAIController->GetPawn()->GetActorLocation(), PatrolRadius, PatrolLocation
		);

		//����, ��Ʈ�� �� ���� ��ġ�� ã�Ҵٸ�?
		if (bIsFound) {

			/*��Ʈ�� �� ���� ��ġ�� �����忡 ������ �ݴϴ�. */
			OwnerComp.GetBlackboardComponent()->
				SetValue<UBlackboardKeyType_Vector>(GetSelectedBlackboardKey(), PatrolLocation.Location);

			//��Ʈ�� �� ���� ��ġ�� ã�Ҵٰ� ������ �ݴϴ�.
			return EBTNodeResult::Succeeded;


		}


	}


	/*�� ã������, �����ߴٰ� ����*/
	return EBTNodeResult::Failed;


}