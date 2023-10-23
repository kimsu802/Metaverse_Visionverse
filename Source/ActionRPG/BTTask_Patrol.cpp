// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Patrol.h"

//AI 관련 라이브러리
#include "BehaviorTree/BTNode.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"




//길찾기 라이브러리
#include "NavigationSystem.h"

//CreatureAIController에 접근하기 위한
#include "CreatureAIController.h"






//테스트(일)을 수행하는 가상함수의 원형 정의.
EBTNodeResult::Type UBTTask_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	//NavigationSystem을 가져옴.
	UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	
	//크리쳐의 AIController를 가져옴
	ACreatureAIController* CreatureAIController = Cast<ACreatureAIController>(OwnerComp.GetAIOwner());

	//캐스트 하면, if문으로 확인해주는 것이 좋음.
	//만일, 길찾기 시스템을 찾아고, 크리쳐의 AIController를 찾았고 크리쳐가 컨트롤하고 있는 폰을 찾았다면?

	if (NavigationSystem && CreatureAIController && CreatureAIController->GetPawn()) {
		/*AIController가 컨트롤하고 있는 폰을 가져옴.*/
		APawn* CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();

		//Patrol 반경을 6미터로 설정함. 언리얼은 단위가 cm임.
		const float PatrolRadius = 600.0f;

		/*임시 패트롤 위치를 저장해 둘 변수를 선언*/
		FNavLocation PatrolLocation;

		/*위치를 찾았는지 판별해주는 불리언 변수 선언
		6미터 반경에서 임의의 위치를 찾음
		GetRandomPointInNavagableRadius() : 반경안에서 랜덤한 위치를 찾음.
		CreatureAIController->GetPawn()->GetActorLocation() : AIController가 컨트롤하고 있는 폰의 위치.
		PatrolRadius : 찾을 반경.
		PatrolLocation : 찾았을 때 위치를 저장해 둘 변수.
		*/

		const bool bIsFound = NavigationSystem->GetRandomPointInNavigableRadius(
		CreatureAIController->GetPawn()->GetActorLocation(), PatrolRadius, PatrolLocation
		);

		//만약, 패트롤 할 랜덤 위치를 찾았다면?
		if (bIsFound) {

			/*패트롤 할 랜덤 위치를 블랙보드에 지정해 줍니다. */
			OwnerComp.GetBlackboardComponent()->
				SetValue<UBlackboardKeyType_Vector>(GetSelectedBlackboardKey(), PatrolLocation.Location);

			//패트롤 할 랜덤 위치를 찾았다고 리턴해 줍니다.
			return EBTNodeResult::Succeeded;


		}


	}


	/*못 찾았으면, 실패했다고 리턴*/
	return EBTNodeResult::Failed;


}