// Fill out your copyright notice in the Description page of Project Settings.


#include "CreatureAIController.h"

//AI 관련 라이브러리
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

//크리쳐에 접근하기 위한
#include "Creature.h"

//플레이어에게 접근하기 위한
#include "ActionRPGCharacter.h"

// 플레이어나 게임모드를 찾기 위한
#include "Kismet/GameplayStatics.h"





ACreatureAIController::ACreatureAIController() {

	//Behavior Tree Component를 생성해줌.
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));

	//BlackBoard Component를 생성해줌
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

}
//컨트롤러가 폰을 빙의하는 가상 함수 정의
void ACreatureAIController::OnPossess(APawn* InPawn)
{
	/*부모 클래스의 함수 내용을 모두 가져옴*/
	Super::OnPossess(InPawn);

	UE_LOG(LogTemp, Warning, TEXT("OnPossess"));

	/*매개 변수로 전달받은 폰을 크리쳐로 형변환 합니다.*/
	ACreature* Creature = Cast<ACreature>(InPawn);

	//항상, 캐스트를 하고나서 형변환이 잘 되었는지 if문 검사를 하는 것이 중요함.
	//만약, 크리쳐가 있고 크리쳐의 BehaviorTree가 있다면?
	if (Creature->GetCreatureBehavior()->BlackboardAsset) {
		
		//BlackboardAsset을 초기화 해줌.
		BlackboardComponent->InitializeBlackboard(*(Creature->GetCreatureBehavior()->BlackboardAsset));

		//적의 ID를 가져옵니다.
		TargetID = BlackboardComponent->GetKeyID("TargetKey");

		//이제 크리쳐의 AI를 시작합니다.
		BehaviorTreeComponent->StartTree(*(Creature->GetCreatureBehavior()));


		UE_LOG(LogTemp, Warning, TEXT("BehaviorTreeComponent->StartTree"));



	}

}

//컨트롤러가 폰을 빙의해제하는 가상 함수 정의
void ACreatureAIController::OnUnPossess()
{	
	/*부모 클래스의 함수 내용을 모두 가져옴*/
	Super::OnUnPossess();
}


//타겟을 향해 회전시켜주는 가상함수 정의
void ACreatureAIController::UpdateControlRotation(float DeltaTime, bool bUpdatePawn)
{	
	/*부모 클래스의 함수 내용을 모두 가져옴*/
	Super::UpdateControlRotation(DeltaTime, bUpdatePawn);


	/*플레이어를 찾습니다.*/
	AActionRPGCharacter* Player = Cast<AActionRPGCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	//만약, 플레이어를 찾았다면?
	if (Player != nullptr) {
		/*플레이어의 위치를 가져옵니다*/
		FVector PlayerPosition = Player->GetActorLocation();

		/*만일, 플레이어가 움직이고 있고 컨트롤러가 컨트롤 하고 있는 폰이 있다면?*/
		if (!PlayerPosition.IsZero() && GetPawn()) {

			/*플레이어로 향하는 방향값을 구해옴.*/
			/*PlayerPosition : 플레이어의 위치
			GetPawn()->GetActorLocation() : 크리쳐의 위치
			*/
			FVector Direction = PlayerPosition - GetPawn()->GetActorLocation();

			//타겟으로 향하는 회전값을 구해옴.
			FRotator RotationToTarget = Direction.Rotation();

			//회전값을 0에서 360 사이로 제한시킴.
			RotationToTarget.Yaw = FRotator::ClampAxis(RotationToTarget.Yaw);

			//타겟을 향한 회전값을 설정
			SetControlRotation(RotationToTarget);

			//크리쳐폰을 선택하고
			APawn* CreaturePawn = GetPawn();

			//만약 크리쳐 폰이 있고, 회전이 가능하다면?
			if (CreaturePawn && bUpdatePawn) {

				//타겟을 향해 회전함.
				CreaturePawn->FaceRotation(RotationToTarget, DeltaTime);
			}

		}
	}
}
