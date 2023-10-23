// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_SearchTarget.h"


// 크리쳐 AI에 접근하기 위한
#include "CreatureAIController.h"

//플레이어에 접근하기 위한
#include "ActionRPGCharacter.h"

//캐릭터에 접근하기 위한
#include "GameFramework/Character.h"

//블랙보드 컴포넌트에 접근하기 위한
#include "BehaviorTree/BlackboardComponent.h"

//디버그용으로 구를 그려주기 위한
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



// 함수자의 원형을 정의
UBTService_SearchTarget::UBTService_SearchTarget() {

	/*툴에서 보여지는 노드 이름을 지정해줌*/
	NodeName = TEXT("SearchTarget");

	/*TickNode 함수를 1초에 60번 호출하는 것이 아니고, 2초에 한번 호출하게 함.*/
	Interval = 2.0f;


}

//매 프레임 호출되는 TickNode라는 이름의 가상함수 정의
void UBTService_SearchTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// 부모 클래스의 함수의 내용을 모두 가져옴
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	/* AI가 컨트롤하고 있는 폰을 찾음*/
	APawn* CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	
	//만약, AI가 컨트롤하고 있는 폰을 찾지 못했더라면
	if (CurrentPawn == nullptr) {
		// 더 이상 할 수 있는 것이 없으므로 함수를 빠져나갑니다.
		return;
	}

	/*레벨이 있는 월드(세상)을 찾음*/
	UWorld* World = CurrentPawn->GetWorld();

	/*현재 AI가 컨트롤 하고 있는 폰의 위치를 찾음*/
	FVector Center = CurrentPawn->GetActorLocation();

	/*5미터 반경에서 플레이어를 찾을 것임. 언리얼에서 단위는 cm*/
	float SearchRadius = 500.0f;

	//만약, 월드를 찾지 못했더라면?
	if (World == nullptr) {
		//함수 빠져나감
		return;
	}

	// 5미터 안쪽에 겹쳐진 액터들을 찾아서 저장할 임시 배열을 선언함
	/*충돌체크
	1. 무시 Ignore
	2. 겹침 Overlap
	3. 막힘 Block
	*/

	TArray<FOverlapResult> OverlapResults;

	/*옵션입니다*/
	FCollisionQueryParams QueryParams(NAME_None, false, CurrentPawn);

	/*5미터 안쪽에 겹쳐진 무언가 액터를 찾았다면, true를 반환함
	  크리쳐를 기준으로 5미터 반경의 구를 그려줌*/

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

	/*만약, 5미터 안쪽에 무언가 액터를 찾았다면,*/
	if (bResult) {
		/*찾은 액터들을 대상으로 반복문 처리*/
		for (auto& OverlapResult : OverlapResults) {
			/* 플레이어인지 확인해보기 위해서 플레이어로 형변환 해 봄.*/
			AActionRPGCharacter* Target = Cast<AActionRPGCharacter>(OverlapResult.GetActor());

			/*만약, 플레이어를 찾았고, 플레이어 컨트롤러가 있다면?*/
			if (Target && Target->GetController()->IsPlayerController()) {

				/*블랙보드에 플레이어를 찾았다고 알려줍니다*/
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("TargetKey")), Target);

				//찾았으니 디버그용으로 녹색의 구를 그려줌
				DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Green, false, 0.2f);

				/*플레이어를 찾았으니 함수를 빠져 나감*/
				return;
			}


			// 플레이어를 찾지 못했다고 블랙보드에 알려줌.
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("TargetKey")), nullptr);

		}
		/*찾지 못했으니 디버그용으로 붉은색의 구를 그려줍니다.*/
		DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Red, false, 0.2f);
	}

	//만약, 5미터 안쪽에 겹쳐진 액터를 찾지 못했더라면?
	else {
		// 플레이어를 찾지 못했다고 블랙보드에 알려줌.
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("TargetKey")), nullptr);
		/*찾지 못했으니 디버그용으로 붉은색의 구를 그려줍니다.*/
		DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Red, false, 0.2f);
	}

}
