// Fill out your copyright notice in the Description page of Project Settings.


#include "FantasyKFPC.h"
#include "Net/UnrealNetwork.h"
#include "Damageable.h"
#include "FKFCharacterMovementComponent.h"

// Sets default values
AFantasyKFPC::AFantasyKFPC(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UFKFCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Health = MaxHealth;
	Stamina = MaxStamina;
}

void AFantasyKFPC::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFantasyKFPC, Health);
	DOREPLIFETIME(AFantasyKFPC, MaxHealth);
	DOREPLIFETIME(AFantasyKFPC, Stamina);
	DOREPLIFETIME(AFantasyKFPC, MaxStamina);
	DOREPLIFETIME(AFantasyKFPC, Healings);
	DOREPLIFETIME(AFantasyKFPC, LastHit);
}

// Called when the game starts or when spawned
void AFantasyKFPC::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFantasyKFPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AFantasyKFPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AFantasyKFPC::SetIgnoreCollision(bool bIsIgnoring) {
	UFKFCharacterMovementComponent* CharMov = static_cast<UFKFCharacterMovementComponent*>(GetCharacterMovement());
	if (CharMov) {
		CharMov->SetIgnoreCollision(bIsIgnoring);
	}
}

void AFantasyKFPC::SetSprinting(bool bIsSprinting) {
	UFKFCharacterMovementComponent* CharMov = static_cast<UFKFCharacterMovementComponent*>(GetCharacterMovement());
	if (CharMov) {
		CharMov->SetSprinting(bIsSprinting);
	}
}

void AFantasyKFPC::SetBlocking(bool bIsBlocking) {
	UFKFCharacterMovementComponent* CharMov = static_cast<UFKFCharacterMovementComponent*>(GetCharacterMovement());
	if (CharMov) {
		CharMov->SetBlocking(bIsBlocking);
	}
}

void AFantasyKFPC::SetAirBreaking(bool bIsAirBreaking) {
	UFKFCharacterMovementComponent* CharMov = static_cast<UFKFCharacterMovementComponent*>(GetCharacterMovement());
	if (CharMov) {
		CharMov->SetAirBreaking(bIsAirBreaking);
	}
}

void AFantasyKFPC::ToggleCollision(bool bEnable) {
	OnToggleCollision(bEnable);
}

void AFantasyKFPC::ToggleSprint(bool bEnable) {
	OnToggleSprint(bEnable);
}

void AFantasyKFPC::ToggleBlock(bool bEnable) {
	OnToggleBlock(bEnable);
}

void AFantasyKFPC::ToggleAirBreak(bool bEnable) {
	OnToggleAirBreak(bEnable);
}

void AFantasyKFPC::ServerApplyDamage_Implementation(AActor* TargetActor, float _Damage, EStaggerType _Stagger, float _Stamina) {
	Stamina += _Stamina;
	if (IsLocallyControlled()) {
		OnUpdateStamina();
	}
	if (TargetActor == nullptr || !TargetActor->GetClass()->ImplementsInterface(UDamageable::StaticClass())) return;

	IDamageable* DamageableInterface = Cast<IDamageable>(TargetActor);
	DamageableInterface->ApplyDamage(_Damage, _Stagger);
}

bool AFantasyKFPC::ServerApplyDamage_Validate(AActor* TargetActor, float _Damage, EStaggerType _Stagger, float _Stamina) {
	return true;
}

void AFantasyKFPC::UseHealing(AFantasyKFPC* TargetPlayer) {
	if (TargetPlayer == nullptr) return;

	ServerHeal(TargetPlayer);
}

void AFantasyKFPC::ServerHeal_Implementation(AFantasyKFPC* TargetPlayer) {
	if (TargetPlayer == nullptr) return;
	if (Healings <= 0) return;

	TargetPlayer->Health = MaxHealth;
	TargetPlayer->Stamina = MaxStamina;
	Healings--;
	if (TargetPlayer->IsLocallyControlled()) {
		TargetPlayer->OnUpdateHealth();
		TargetPlayer->OnUpdateStamina();
	}
	if (IsLocallyControlled()) {
		OnUpdateHealings();
	}
}

bool AFantasyKFPC::ServerHeal_Validate(AFantasyKFPC* TargetPlayer) {
	return true;
}

void AFantasyKFPC::ApplyDamage(float _Damage, EStaggerType _Stagger) {
	OnDamaged(_Damage, _Stagger);
}

float AFantasyKFPC::GetHealth() {
	return Health;
}

void AFantasyKFPC::CallUpdateLastHit(EStaggerType _Stagger, bool bWasBlocked) {
	UpdateLastHit(_Stagger, bWasBlocked);
	OnUpdateLastHit();
}

void AFantasyKFPC::UpdateLastHit(EStaggerType _Stagger, bool bWasBlocked) {
	LastHit.Stagger = _Stagger;
	LastHit.bWasBlocked = bWasBlocked;
	LastHit.HitAttempt = (LastHit.HitAttempt + 1) % 10;
}

void AFantasyKFPC::OnTargetHit(AActor* TargetActor, float _Damage, EStaggerType _Stagger, float _Stamina) {
	ServerApplyDamage(TargetActor, _Damage, _Stagger, _Stamina);
}

void AFantasyKFPC::OnRep_Health() {
	UE_LOG(LogTemp, Warning, TEXT("ON REP HEALTH"));
	if (IsLocallyControlled()) {
		OnUpdateHealth();
	}
}

void AFantasyKFPC::OnRep_Stamina() {
	UE_LOG(LogTemp, Warning, TEXT("ON REP STAMINA"));
	if (IsLocallyControlled()) {
		OnUpdateStamina();
	}
}

void AFantasyKFPC::OnRep_Healings() {
	UE_LOG(LogTemp, Warning, TEXT("ON REP HEALINGS"));
	if (IsLocallyControlled()) {
		OnUpdateHealings();
	}
}

void AFantasyKFPC::OnRep_LastHit() {
	UE_LOG(LogTemp, Warning, TEXT("ON REP LAST HIT"));
	if (IsLocallyControlled()) {
		OnUpdateLastHit();
	}
}