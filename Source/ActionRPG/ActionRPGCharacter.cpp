// Copyright Epic Games, Inc. All Rights Reserved.

#include "ActionRPGCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
//무기에 접근하기 위한 무기 헤더 파일 추가
#include "PlayerWeapon.h"

//타이머 관련 라이브러리
#include "TimerManager.h"

// 위젯컴포넌트에 접근하기 위한
#include "Components/WidgetComponent.h"

// 체력바에 접근하기 위한
#include "HealthBar.h"

// 생성자에서 체력바를 찾기 위한
#include "UObject/ConstructorHelpers.h"

//////////////////////////////////////////////////////////////////////////
// AActionRPGCharacter

AActionRPGCharacter::AActionRPGCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)


	//CreateDefaultSubObject() : 생성자 함수에서 컴포넌트를 동적으로 생성
	//GetMesh() : 캐릭터에 기본적으로 포함되어 있는 스켈레탈메쉬. 지금은 얼굴 기준
	//SetupAttachment(GetMesh()) : 생성된 상체를 얼굴에 어태치(붙이기) 해준다.

	UpperBody = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("UpperBody"));
	UpperBody->SetupAttachment(GetMesh());

    LowerBody = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LowerBody"));
	LowerBody->SetupAttachment(GetMesh());

	Hand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hand"));
	Hand->SetupAttachment(GetMesh());

	Hair = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());

	//캐릭터의 체력값을 테스트로 500으로 설정해줍니다.
	//Health = 500.0f;

	//플레이어의 최대 체력값을 지정해 줍니다.
	MaxHealth = 20000.0f;

	//플레이어의 체력값을 맥스체력값으로 설정해줍니다.
	Health = MaxHealth;

	//체력바 위치를 가져옵니다.
	///Script/UMGEditor.WidgetBlueprint'/Game/ProjectRPG/UI/WBP_Health.WBP_Health'

	//위젯컴포넌트를 생성해줍니다.
	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));

	//위젯컴포넌틀르 트랜스폼의 기준이 되는 RootComponent에 어태치 시켜줍니다.
	HealthBarWidget->SetupAttachment(RootComponent);

	//위젯컴포넌트가 항상 화면을 보는 스크린모드로 적용해줍니다.
	HealthBarWidget->SetWidgetSpace(EWidgetSpace::Screen);

	//위젯컴포넌트의 위치값을 적용해줍니다.
	HealthBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 80.0f));
	
	//HealthBar UI위젯을 찾아줍니다.
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/ProjectRPG/UI/WBP_Health'"));

	//만일, HealthBar UI 위젯을 찾았다면?
	if (WidgetClass.Succeeded()) {

		/*위젯컴포넌트의 위젯을 설정해줍니다.*/
		HealthBarWidget->SetWidgetClass(WidgetClass.Class);


	}

}

void AActionRPGCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	//플레이어의 무기들을 생성하고, 기본 무기를 플레이어에게 장착
	SpawnPlayerWeapon();

	/*체력바를 표시합니다*/
	SetHealthBar();



}

//사용자가 지정한 무기를 장착해주는 함수 정의
void AActionRPGCharacter::EquipWeapon(APlayerWeapon* Weapon)
{
	//매개 변수로 전달받은 무기를 체크
	if (Weapon != nullptr) {
		/* 무기객체를 저장할 변수를 선언하고 초기값을 nullptr를 대입해준다.
		포인터 변수는 변수를 선언할 때, 대상을 지정하여 초기화를 해주어야 한다.
		당장 지정해 줄 대상이 없다면 nullptr을 대입해서 에러를 없애주어야 한다.*/
		APlayerWeapon* TempWeapon = nullptr;

		/*현재 플레이어가 가지고 있는 무기를 저장*/
		TempWeapon = CurrentWeapon;

		/*현재 플레이어가 가지고 있는 무기가 저장되었다면?*/
		if (TempWeapon) {
			//무기 교체를 위해서 현재 가지고 있는 무기를 장착해제 해준다.
			TempWeapon->UnEquipWeapon();
		}

		/* 현재 무기에 매개 변수로 전달받은 무기를 적용해 줌*/
		CurrentWeapon = Weapon;

		/*무기의 주인을 플레이어로 지정해 줍니다.*/
		Weapon->SetOwnerPawn(this);
		
		/*현재 가지고 있는 무기를 장착해준다*/
		Weapon->EquipWeapon(CurrentWeapon);


	}
}
//플레이어의 무기들을 생성하는 함수를 정의합니다
void AActionRPGCharacter::SpawnPlayerWeapon()
{
	/*BP_Weapon(무기 블루프린트들을 저장해 둘 배열입니다)
	대상으로 반복문 처리해줌.
	*/
	for (int32 i = 0; i < BP_Weapons.Num(); i++) {
		//만약, 무기 블루프린트가 현재 적용되어 있다면?
		if (BP_Weapons[i]) {
			/*생성 옵션*/
			FActorSpawnParameters SpawnParams;

			/*무기를 생성해줌
			AActor 자식 클래스들은 SpawnActor() 함수로 동적으로 객체 생성을 함*/
			APlayerWeapon* Weapon = GetWorld()->SpawnActor<APlayerWeapon>(BP_Weapons[i],SpawnParams);

			/*만약, 무기가 제대로 생성이 되었다면?*/
			if (Weapon != nullptr) {
				/* 
				메모리에만 생성을 한다면, 접근이 불가능함.
				접근이 가능하도록 배열에 저장해줌.
				생성된 무기를 실질적인 무기 배열인 Weapons 배열에 저장해줌
				*/
				Weapons.Add(Weapon);

			}

		}
	}

	UE_LOG(LogTemp, Warning, TEXT("%d 종의 무기가 생성되었습니다 !!"), Weapons.Num());

	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::MakeRandomColor(), FString::Printf(TEXT("%d종의 무기가 생성되었습니다 !!"), Weapons.Num()));
	}

	/*만일 무기들이 Weapons 배열에 들어가 있다면?*/
	if (Weapons.Num() > 0) {
		/* 테스트로 기본무기를 플레이어에게 장착해줌*/
		EquipWeapon(Weapons[(int32)EWeaponType::Blade_BlackKnight]);
	}
}

//무기를 교체하기 위한 함수 정의
void AActionRPGCharacter::OnChangeWeapon()
{
	/* 배열에서 현재 장착하고 있는 무기의 인덱스를 가져옴*/
	int32 CurrentWeaponIndex = Weapons.IndexOfByKey(CurrentWeapon);
		                       //GetWeapons().IndexOfByKey(GetCurrentWeapon());
								
	/* 다음 무기를 장착하기 위해서 인덱스를 증가시킴*/
	++CurrentWeaponIndex;

	/*만일, 인덱스가 무기 배열의 사이즈라면?*/
	if (CurrentWeaponIndex == Weapons.Num()) {
		/*반복해주기 위해서 인덱스를 초기화 해줌*/
		CurrentWeaponIndex = 0;

	}

	/*인덱스를 통해서 무기 배열에서 무기를 가져옴*/
	APlayerWeapon* NextWeapon = Weapons[CurrentWeaponIndex];

	/*무기 장착*/
	EquipWeapon(NextWeapon);

}
//공격 함수를 정의합니다.
void AActionRPGCharacter::OnAttack()
{
	//만약, 플레이어가 현재 무기를 가지고 있다면?
	if (CurrentWeapon != nullptr) {
		//만약, 공격중이 아니라면?
		if (CurrentWeapon->GetBIsAttack() == false) {
			//공격합니다.
			CurrentWeapon->StartAttack();

		}
	}

}
//플레이어의 체력값을 반환하는 함수를 정의
float AActionRPGCharacter::GetHealth()
{
	return Health;
}
//플레이어가 데미지를 받았을 때, 호출되는 함수 정의.
void AActionRPGCharacter::Hit()
{
	// 지금은 Hit 애니메이션을 보여줌.
	float HitAnimDuration = PlayAnimMontage(HitAnimMontage);
}

//플레이어의 체력이 다 했을때, 호출되는 함수를 정의
void AActionRPGCharacter::Die()
{
	/* 플레이어가 생명력을 다 했으므로, 체력값을 0으로 해줌*/
	Health = 0.0f;
	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), Health);

	/* 타이머가 돌아가고 있다면 우선 멈춰줌.*/
	GetWorldTimerManager().ClearAllTimersForObject(this);

	// Die 애니메이션을 보여줌.
	float DieAnimDuration = PlayAnimMontage(DeathAnimMontage);

	/*만일, 플레이어가 충돌체를 가지고 있다면, 이미 생명력을 다 했으므로 충돌하지 않도록 해줌.*/
	if (GetCapsuleComponent()) {
		GetCapsuleComponent()->BodyInstance.SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	//이동을 멈춰줌.
	if (GetMovementComponent()) {
		GetMovementComponent()->StopMovementImmediately();
	}

	/*Pawn : 몸
	PlayerController : 영혼
	영혼이 몸에 빙의해서 살아있게 함. OnPossess()
	따라서, 빙의를 해제해줌. UnPossess()
	*/

	if (Controller != nullptr) {
		Controller->UnPossess();
	}

	FTimerHandle TimerHandle;

	//DieAnimDuration이 끝나면, DestroyPawn 함수를 호출함.
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AActionRPGCharacter::DestroyPawn, DieAnimDuration);

}

//Die 했을 때, 플레이어를 메모리에서 삭제. 타이머를 사용할 것임.
void AActionRPGCharacter::DestroyPawn()
{

	/* 현재 플레이어를 게임에서 안 보이게 해줌*/
	SetActorHiddenInGame(true);
	
	/*0.1초에 Destroy() 함수를 호출해서 캐릭터를 메모리에서 삭제시켜줌.*/
	SetLifeSpan(0.1f);


}


// 캐릭터가 데미지를 받는 이벤트 가상함수를 정의
float AActionRPGCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//플레이어의 체력이 0보다 작거나 같다고 한다면 플레이어의 체력을 0으로 설정함.

	if (Health <= 0.0f) {
		return 0.0f;
	}
	
	/*실제 데미지 값을 받아옴.*/
	float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	//실제 데미지 값이 0보다 크다면?
	if (ActualDamage > 0) {

		//체력값에서 실제 데미지값을 빼줌
		Health -= ActualDamage;
	}

	//플레이어의 체력값이 0보다 작거나 같다면?
	if (Health <= 0.0f) {

		/*플레이어는 죽음*/
		Die();
	}
	//그렇지 않고 만약, 플레이어의 체력값이 0보다 크다면?
	else {
		//히트 애니메이션을 보여줌.
		Hit();
	}


	UE_LOG(LogTemp, Warning, TEXT("Player's Health : %f"), Health);

	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 10.0f,FColor::Green, FString::Printf(TEXT("Player's Health : %f, ActualDamage : %f"), Health, ActualDamage));
	}


	/*체력바를 표시합니다.*/
	SetHealthBar();




	//실제 데미지 값을 반환
	return ActualDamage;
}

//무기 충돌체크를 가능하게 해줍니다.
void AActionRPGCharacter::SetWeaponCollisionEnabled()
{
	CurrentWeapon->SetWeaponCollisionEnabled();
}


//무기 충돌체크를 불가능하게 해줍니다.
void AActionRPGCharacter::SetWeaponCollisionDisabled()
{
	CurrentWeapon->SetWeaponCollisionDisabled();
}


//무기 충돌체크를 초기화 해줍니다.
void AActionRPGCharacter::SetInitCollision()
{
	CurrentWeapon->SetWeaponCollisionDisabled();
}

TArray<class USkeletalMesh*> AActionRPGCharacter::GetUpperBodys() const
{
	return UpperBodys;
}

TArray<class USkeletalMesh*> AActionRPGCharacter::GetLowerBodys() const
{
	return LowerBodys;
}

TArray<class USkeletalMesh*> AActionRPGCharacter::GetHairs() const
{
	return Hairs;
}
// 파츠 교체하는 함수 정의
void AActionRPGCharacter::OnChangeParts()
{
	//상체 배열에서 랜덤으로 인덱스를 가져옴.
	int32 RandomIndex = FMath::RandRange(0, GetUpperBodys().Num() - 1);
	//상체 배열에서 랜덤인덱스로 가져온 상태를 교체 해 줌
	UpperBody->SetSkeletalMesh(GetUpperBodys()[RandomIndex]);

	//상체 배열에서 랜덤으로 인덱스를 가져옴.
	int32 RandomIndex2 = FMath::RandRange(0, GetLowerBodys().Num() - 1);
	//상체 배열에서 랜덤인덱스로 가져온 상태를 교체 해 줌
	LowerBody->SetSkeletalMesh(GetLowerBodys()[RandomIndex2]);


	//상체 배열에서 랜덤으로 인덱스를 가져옴.
	int32 RandomIndex3 = FMath::RandRange(0, GetHairs().Num() - 1);
	//상체 배열에서 랜덤인덱스로 가져온 상태를 교체 해 줌
	Hair->SetSkeletalMesh(GetHairs()[RandomIndex3]);

}
//플레이어의 최대 체력값을 반환하는 함수를 정의
float AActionRPGCharacter::GetMaxHealth()
{
	return MaxHealth;
}


// 체력바를 설정해주는 함수를 정의합니다.
void AActionRPGCharacter::SetHealthBar()
{
	/*체력바를 가져옵니다.*/
	auto const UserWidget = Cast<UHealthBar>(HealthBarWidget->GetUserWidgetObject());

	//만일, 체력바를 찾았다면?
	if (UserWidget) {

		//체력바에 현재 플레이어의 체력값을 표시합니다.
		/*게임엔진은 값을 0~1 사이의 노멀라이즈(정규화)된 값을 사용합니다. 효율을 위해*/
		UserWidget->SetHealthBarValue((Health / MaxHealth * 100) * 0.01f);
	}
}

FName AActionRPGCharacter::GetWeaponSocketName() const
{
	return RightWeaponSocket;
}
//무기들을 반환할 함수를 정의
TArray<class APlayerWeapon*> AActionRPGCharacter::GetWeapons() const
{
	return Weapons;
}
//현재 플레이어가 가지고 있는 무기를 반환할 함수를 정의
APlayerWeapon* AActionRPGCharacter::GetCurrentWeapon() 
{
	return CurrentWeapon;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AActionRPGCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{

	PlayerInputComponent->BindAction("ChangeParts", IE_Pressed, this, &AActionRPGCharacter::OnChangeParts);
	PlayerInputComponent->BindAction("ChangeWeapon", IE_Pressed, this, &AActionRPGCharacter::OnChangeWeapon);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AActionRPGCharacter::OnAttack);
	


	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AActionRPGCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AActionRPGCharacter::Look);

	}

}

void AActionRPGCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AActionRPGCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}




