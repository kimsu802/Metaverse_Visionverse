// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
//#include "EngineMinimal.h"
#include "ActionRPGCharacter.generated.h"

//무기를 구분해줄 열거형 선언
//언리얼에서 열거형은 uint8타입을 부모 클래스로 상속받음.
//uint8은 256이므로, 총 256개의 열거형 요소를 가질 수 있음
//UENUM(BlueprintType) : 블루프린트에서 읽을 수가 있음.

enum class EWeaponType : uint8 {
	Blade_BlackKnight,
	Blade_BlackWyrmBlade,
	Blade_DragonSword,
	Blade_Fencer,
	Blade_Glimmer,
};





UCLASS(config=Game)
class AActionRPGCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

public:
	AActionRPGCharacter();
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }



private:

	//값타입인 EditAnywhere로 편집이 가능하고
	//UObject 자식클래스인 Component들은 VisibleAnywhere로 읽기 뿐만 아니라 편집도 가능.

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Parts", meta = (AllowPrivateAccess = true))
	class USkeletalMeshComponent* UpperBody;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Parts", meta = (AllowPrivateAccess = true))
	class USkeletalMeshComponent* LowerBody;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Parts", meta = (AllowPrivateAccess = true))
	class USkeletalMeshComponent* Hand;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Parts", meta = (AllowPrivateAccess = true))
	class USkeletalMeshComponent* Hair;



	//교체를 상의 파츠들을 저장해 둘 배열을 선언
	UPROPERTY(EditDefaultsOnly, Category = "Inventory", meta = (AllowPrivateAccess = true))
	TArray<class USkeletalMesh*> UpperBodys;

	//교체를 하의 파츠들을 저장해 둘 배열을 선언
	UPROPERTY(EditDefaultsOnly, Category = "Inventory", meta = (AllowPrivateAccess = true))
	TArray<class USkeletalMesh*> LowerBodys;

	//교체를 헤어 파츠들을 저장해 둘 배열을 선언
	UPROPERTY(EditDefaultsOnly, Category = "Inventory", meta = (AllowPrivateAccess = true))
	TArray<class USkeletalMesh*> Hairs;

public:

	//상체 하체 머리 배열을 반환하는 함수를 선언. 
	//const 키워드를 두어 멤버변수 값을 수정하지 못하도록 해줌.

	TArray<class USkeletalMesh*> GetUpperBodys() const;

	TArray<class USkeletalMesh*> GetLowerBodys() const;

	TArray<class USkeletalMesh*> GetHairs() const;

	
	//코드에서만 접근을 하려면 UFUNCTION 매크로가 필요없지만
	//엔진과 상호작용하려면 UFUNTION 매크로가 필요함.
	//키보드의 특정 키를 눌렀을때, OnChangeParts 함수가 호출되도록 액션 매핑을 해줄 것이기 때문에,
	//델리게이트(대리자) 형식이기 때문에, UFUNTION() 매크로가 필요함.
	//함수 이름만 보더라도 델리게이트나 이벤트 함수인 것을 알 수 있도록 접두사 On을 붙여주도록함.
	UFUNCTION()
	void OnChangeParts();


private:
	//무기를 장착할 때, 소켓 이름은 FName 타입을 요구
	//FName타입은 대,소문자 구분x / 소켓이름을 저장해 둘 변수 선언
	
	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (AllowPrivateAccess = true))
	FName RightWeaponSocket;

	//여러개의 플레이어의 무기들에 접근하기 위한 배열 선언
	UPROPERTY()
	TArray<class APlayerWeapon*> Weapons;

	//여러개의 무기중에서 현재 플레이어가 가지고 있는 무기를 저장할 변수 선언
	//UObject 자식클래스들과 AActor 자식클래스들은 기본적으로 UPROPERTY 매크로를 붙여주는 것이 좋다.
	UPROPERTY()
	class APlayerWeapon* CurrentWeapon;

	//무기 블루프린트들을 블루프린트 디테일 패널에서 추가하기 위한 배열을 선언
	UPROPERTY(EditDefaultsOnly,Category = "Inventory",meta = (AllowPrivateAccess = true))
	//TSubClassOf<class APlayerWeapon>> : APlayerWeapon 클래스를 포함한 자식클래스(블루프린트 클래스도 포함)만을 
	//선택할 수 있도록 도와주는 템플릿.
	TArray<TSubclassOf<class APlayerWeapon>> BP_Weapons;

	//플레이어의 체력값을 저장할 변수를 선언
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "health", meta = (AllowPrivateAccess = true))
		float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "health", meta = (AllowPrivateAccess = true))
	UAnimMontage* HitAnimMontage;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "health", meta = (AllowPrivateAccess = true))
	UAnimMontage* DeathAnimMontage;

	/* 플레이어의 최대 체력값을 저장할 변수를 선언합니다.
	Category = "Health" : 새로운 항목, 그냥 새로운 항목으로 하시는 게 좋습니다.
	Category = Health : 이미 있는 항목 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = true))
	float MaxHealth;


	/*위젯컴포넌트를 저장할 변수를 선언합니다.
	UObject 자식클래스들은 무조건 UPROPERTY 매크로를 붙여서 엔진이 메모리 관리를 하도록 해줍시다.*/
	UPROPERTY()
	class UWidgetComponent* HealthBarWidget;


public:
	//플레이어의 최대 체력값을 반환하는 함수의 원형을 선언합니다.
	float GetMaxHealth();

	//체력바를 설정 해주는 함수의 원형을 선언
	void SetHealthBar();

	//소켓 이름을 반환하는 함수의 원형을 선언
	FName GetWeaponSocketName() const;

	//무기들을 반환할 함수의 원형 선언
	TArray<class APlayerWeapon*> GetWeapons() const;


	//현재 플레이어가 가지고 있는 무기를 반환할 함수 선언
	class APlayerWeapon* GetCurrentWeapon();

	/*BeginPlay 가상함수의 원형을 선언해줌
	BeginPlay 함수 안에서 플레이어의 무기들을 생성하고 장착해줌
	부모 클래스의 BeginPlay 함수를 자식 클래스에서 재정의 하기 이해서 가상함수로 오버라이딩 해줌*/

	//사용자가 지정해준 무기를 장착하는 함수의 원형 선언
	void EquipWeapon(class APlayerWeapon* Weapon);


	//플레이어의 무기들을 생성하는 함수의 원형 선언
	void SpawnPlayerWeapon();


	//무기를 변경할 함수의 원형 선언
	//델리게이트 이기 때문에 UFUNTION() 매크로를 붙여줌.
	UFUNCTION()
	void OnChangeWeapon();


	//공격 함수의 원형을 선언.
	//액션 매핑(델리게이트)이기 때문에 UFUNTION() 매크로를 붙여줌.
	UFUNCTION()
	void OnAttack();

	//플레이어의 체력값을 반환하는 함수의 원형을 선언
	float GetHealth();

	//플레이어가 데미지를 받았을 때, 호출되는 함수의 원형을 선언.
	void Hit();

	//플레이어의 체력이 다 했을때, 호출되는 함수의 원형을 선언.
	void Die();

	//Die 애니메이션이 끝나면, 플레이어를 메모리에서 삭제해야함.
	void DestroyPawn();



	/*캐릭터가 데미지를 받는 이벤트 가상함수의 원형을 선언.
	이벤트 함수이며, 내가 호출하지 않아도 엔진에서 자동으로 호출되는 이벤트 함수임.
	물론, 내가 호출할 수도 있음.

	데미지를 주는 이벤트 함수는
	1.ApplyDamage : 일반적으로 데미지를 주는 이벤트 함수.
	2.ApplyPointDamage : 특정 부분에 데미지를 주는 이벤트 함수.
	3.ApplyRadialDamage : 범위 데미지를 주는 이벤트 함수

	총 이렇게 3가지가 있으며,

	데미지를 받는 이벤트 함수는.
	1.TakeDamage가 있는데 매개변수가 총 4개가 존재함.

	1.float Damage : 받은 데미지양
	2.struct DamageEvent : 받은 데미지 타입
	3.class AController* EventInstigator : 누가 데미지를 일으켰는지? ex) PlayerController, AIController
	4.AActor* DamageCauser : 무엇이(도구) 데미지를 일으켰는지?
	*/
	
	virtual float TakeDamage(
		float Damage,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

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

