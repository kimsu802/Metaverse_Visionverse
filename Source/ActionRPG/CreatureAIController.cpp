// Fill out your copyright notice in the Description page of Project Settings.


#include "CreatureAIController.h"

//AI ���� ���̺귯��
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

//ũ���Ŀ� �����ϱ� ����
#include "Creature.h"

//�÷��̾�� �����ϱ� ����
#include "ActionRPGCharacter.h"

// �÷��̾ ���Ӹ�带 ã�� ����
#include "Kismet/GameplayStatics.h"





ACreatureAIController::ACreatureAIController() {

	//Behavior Tree Component�� ��������.
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));

	//BlackBoard Component�� ��������
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

}
//��Ʈ�ѷ��� ���� �����ϴ� ���� �Լ� ����
void ACreatureAIController::OnPossess(APawn* InPawn)
{
	/*�θ� Ŭ������ �Լ� ������ ��� ������*/
	Super::OnPossess(InPawn);

	UE_LOG(LogTemp, Warning, TEXT("OnPossess"));

	/*�Ű� ������ ���޹��� ���� ũ���ķ� ����ȯ �մϴ�.*/
	ACreature* Creature = Cast<ACreature>(InPawn);

	//�׻�, ĳ��Ʈ�� �ϰ��� ����ȯ�� �� �Ǿ����� if�� �˻縦 �ϴ� ���� �߿���.
	//����, ũ���İ� �ְ� ũ������ BehaviorTree�� �ִٸ�?
	if (Creature->GetCreatureBehavior()->BlackboardAsset) {
		
		//BlackboardAsset�� �ʱ�ȭ ����.
		BlackboardComponent->InitializeBlackboard(*(Creature->GetCreatureBehavior()->BlackboardAsset));

		//���� ID�� �����ɴϴ�.
		TargetID = BlackboardComponent->GetKeyID("TargetKey");

		//���� ũ������ AI�� �����մϴ�.
		BehaviorTreeComponent->StartTree(*(Creature->GetCreatureBehavior()));


		UE_LOG(LogTemp, Warning, TEXT("BehaviorTreeComponent->StartTree"));



	}

}

//��Ʈ�ѷ��� ���� ���������ϴ� ���� �Լ� ����
void ACreatureAIController::OnUnPossess()
{	
	/*�θ� Ŭ������ �Լ� ������ ��� ������*/
	Super::OnUnPossess();
}


//Ÿ���� ���� ȸ�������ִ� �����Լ� ����
void ACreatureAIController::UpdateControlRotation(float DeltaTime, bool bUpdatePawn)
{	
	/*�θ� Ŭ������ �Լ� ������ ��� ������*/
	Super::UpdateControlRotation(DeltaTime, bUpdatePawn);


	/*�÷��̾ ã���ϴ�.*/
	AActionRPGCharacter* Player = Cast<AActionRPGCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	//����, �÷��̾ ã�Ҵٸ�?
	if (Player != nullptr) {
		/*�÷��̾��� ��ġ�� �����ɴϴ�*/
		FVector PlayerPosition = Player->GetActorLocation();

		/*����, �÷��̾ �����̰� �ְ� ��Ʈ�ѷ��� ��Ʈ�� �ϰ� �ִ� ���� �ִٸ�?*/
		if (!PlayerPosition.IsZero() && GetPawn()) {

			/*�÷��̾�� ���ϴ� ���Ⱚ�� ���ؿ�.*/
			/*PlayerPosition : �÷��̾��� ��ġ
			GetPawn()->GetActorLocation() : ũ������ ��ġ
			*/
			FVector Direction = PlayerPosition - GetPawn()->GetActorLocation();

			//Ÿ������ ���ϴ� ȸ������ ���ؿ�.
			FRotator RotationToTarget = Direction.Rotation();

			//ȸ������ 0���� 360 ���̷� ���ѽ�Ŵ.
			RotationToTarget.Yaw = FRotator::ClampAxis(RotationToTarget.Yaw);

			//Ÿ���� ���� ȸ������ ����
			SetControlRotation(RotationToTarget);

			//ũ�������� �����ϰ�
			APawn* CreaturePawn = GetPawn();

			//���� ũ���� ���� �ְ�, ȸ���� �����ϴٸ�?
			if (CreaturePawn && bUpdatePawn) {

				//Ÿ���� ���� ȸ����.
				CreaturePawn->FaceRotation(RotationToTarget, DeltaTime);
			}

		}
	}
}
