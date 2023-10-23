// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Creature.generated.h"


//크리쳐가 공격 콩타주를 종료하면, 이를 알려주도록 델리게이트 선언
DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate)



UCLASS()
class ACTIONRPG_API ACreature : public ACharacter
{
	GENERATED_BODY()



public:
	// Sets default values for this character's properties
	ACreature();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	//충돌을 처리하는 가상함수의 원형을 선언
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;


	//크리쳐가 플레이어를 발견하면, 플레이어를 향해 회전해야함.
	//크리쳐가 플레이어를 향해 회전할 수 있도록 해주는 가상 함수의 원형을 선언함.
	virtual void FaceRotation(FRotator NewControlRotation, float DeltaTime) override;

	//공격을 시작하는 함수의 원형
	void StartAttack();

	//공격을 끝내는 함수의 원형
	void EndAttack();

	//게임모드를 반환하는 함수 선언
	class AActionRPGGameMode* GetGameMode();

	//BehaviorTree를 반환하는 함수의 원형을 선언
	class UBehaviorTree* GetCreatureBehavior();




private:
	//충돌체를 저장하기 위한 변수 선언
	UPROPERTY(VisibleAnywhere, Category = "Collision",meta=(AllowPrivateAccess = true))
	class USphereComponent* SphereComponent;

	//공격 AnimMontage를 설정하기 위한 변수 선언
	UPROPERTY(EditAnywhere, Category = "Pawn", meta = (AllowPrivateAccess = true))
	class UAnimMontage* AttackAnimMontage;

	//크리쳐가 공격했을 때만 데미지를 주도록 하는 불리언 변수를 선언.
	bool bIsDuringAttack;

	//게임모드를 저장하는 변수를 선언.
	class AActionRPGGameMode* GameMode;

	//체력값을 저장해 둘 변수를 선언
	UPROPERTY(EditAnywhere, Category = "Health", meta = (AllowPrivateAccess = true))
	float Health;

	//최대 체력값
	UPROPERTY(EditAnywhere, Category = "Health", meta = (AllowPrivateAccess = true))
	float MaxHealth;

	//Hit 애니메이션을 출력하기 위한 변수 선언
	UPROPERTY(EditAnywhere, Category = "Pawn", meta = (AllowPrivateAccess = true))
	class UAnimMontage* HitAnimMontage;


	//Death 애니메이션을 출력하기 위한 변수 선언
	UPROPERTY(EditAnywhere, Category = "Pawn", meta = (AllowPrivateAccess = true))
	class UAnimMontage* DeathAnimMontage;

	//크리쳐가 데미지를 받았을 때, 호출되는 함수의 원형을 선언.
	void Hit();

	//크리쳐가 체력을 다 했을 때, 호출되는 함수의 원형을 선언.
	void Die();

	//크리쳐가 체력을 다 했을 때, 메모리에서 삭제해주는 함수의 원형 선언.
	void DestroyPawn();


	virtual float TakeDamage(
		float Damage,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;



	//Behavior Tree에 접근하기 위한 변수 선언.
	UPROPERTY(EditAnywhere, Category="Behavior", meta = (AllowPrivateAccess = true))
	class UBehaviorTree* CreatureBehavior;

	//공격중인지 판별해 줄 불리언 변수 선언.
	UPROPERTY(EditAnywhere, category = "Attack", meta = (AllowPrivateAccess = true))
	bool IsAttacking;





	/* 
	델리게이트에서 호출되는 함수의 원형을 선언.
	크리쳐의 공격 몽타주 재생이 끝나면 OnAttackEnd를 호출. Broadcast(방송)
	태스트에 있는 모든 OnAttackEnd를 호출함.

	공격 태스크는 공격 애니메이션이 끌날 때까지 대기해야 하는 지연 태스크이므로, ExecuteTask의 결과 값을,
	InProgress(진행중)로 반환하고, 공격이 끝났을 때, 태스크가 끝났다고 알려주어야 함. 이를 알려주는 함수가 FinishLatentTask.
	이 함수를 호출하지 않으면, BehaviorTree 시스템은 현재 태스크에 계속 머물러 있게 됨. 그래서 호출 할 수 있도록 노드의 Tick기능을 활성화 하고 조건을 파악한 뒤에,
	태스크 종료 명령을 내려줘야 함.

	따라서, 정리하면, 태스크에서 공격 애니메이션을 호출하고 몽타주가 끝나면, OnAttackEnd에 의해 람다 함수가 호출되어,
	IsAttacking이 false가 되어 Tick함수에서 FinishLatentTask 함수가 호출되어 태스크가 종료하게 됨.
	*/
	public:
	FOnAttackEndDelegate OnAttackEnd;

	/*공격 몽타주가 끝났을 때 호출되는 함수의 원형을 선언함.
	애님 인스턴스는 애니메이션 몽타주 재생이 끝나면 발동하는 OnMontageEnded라는 델리게이트를 제공함.
	어떤 언리얼 오브젝트라도 UAnimMontage* 매개변수와 bool 매개 변수를 가진 함수를 가지고 있다면,
	이를 OnMontageEnded 델리게이트에 등록해 몽타주 재생이 끝나는 타이밍을 파악할 수가 있음.*/

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:
	/*위젯컴포넌트를 저장할 변수를 선언합니다.
	UObject 자식클래스들은 무조건 UPROPERTY 매크로를 붙여서 엔진이 메모리 관리를 하도록 해줍시다.*/
	UPROPERTY()
	class UWidgetComponent* HealthBarWidget;

public:
	//체력바를 설정해 주는 함수의 원형을 선언합니다.
	void SetHealthBar();

	/*무기 충돌체크를 가능하도록 해줌.
	애니메이션 블루프린트에서 호출할 것이기 때문에 UFUNCTION 매크로를 붙여줍니다.
	블루프린트에서 호출할 수 있도록 BlueprintCallable 지정어를 붙여줍니다.*/
	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled();

	//무기 충돌체크가 불가능해도록 해줍니다.
	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionDisabled();


	//무기 충돌체크 초기화 해줍니다.
	UFUNCTION(BlueprintCallable)
	void SetInitCollision();


};
