// Fill out your copyright notice in the Description page of Project Settings.

//크리쳐 AIController에 접근하기 위한
#include "CreatureAIController.h"

#include "BTTask_TurnToTarget.h"

//크리쳐에게 접근하기 위한
#include "Creature.h"

//플레이어에게 접근하기 위한
#include "ActionRPGCharacter.h"

//블랙보드 컴포넌트에 접근하기 위한
#include "BehaviorTree/BlackboardComponent.h"

//생성자 함수 정의
UBTTask_TurnToTarget::UBTTask_TurnToTarget() {

	// 행동트리에서 보여질 노드이름을 지정해 줍니다.
	NodeName = TEXT("TurnToTarget");


}
//Task를 실행하는 가상함수를 정의
EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	/*부모 클래스의 함수의 내용을 모두 가져옵니다.*/
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	//크리쳐를 찾습니다.
	ACreature* Creature = Cast<ACreature>(OwnerComp.GetAIOwner()->GetPawn());

	//만일, 크리쳐를 찾지 못했다면?
	if (Creature == nullptr) {

		//Task는 실패함.
		return EBTNodeResult::Failed;

	}

	//플레이어를 찾습니다.
	AActionRPGCharacter* Target = Cast<AActionRPGCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("TargetKey"))));

	//만약, 플레이어를 찾지 못했다면?
	if (Target == nullptr) {
		return EBTNodeResult::Failed;
	}

	//크리쳐에서 플레이어로 향하는 방향과 거리를 구함.
	FVector Direction = Target->GetActorLocation() - Creature->GetActorLocation();
	Direction.Z = 0.0f; //높이는 필요 없으므로 0으로 지정

	//방향벡터로부터 각도를 얻어냅니다.
	FRotator TargetRot = FRotationMatrix::MakeFromX(Direction).Rotator();

	//크리쳐에서 플레이어로 부드럽게 회전하는 회전값을 구해옴.
	//FMath::RInterpTo : Rotation Interpolation, 회전 보간.
	FRotator RotationTo = FMath::RInterpTo(Creature->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(),2.0f);

	//타겟을 향한 회전값을 설정.
	Creature->SetActorRotation(RotationTo);

	//테스크는 성공
	return EBTNodeResult::Succeeded;
}
