// RolePlay Game, All Rights Reserved

#include "RPGBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/RPGCharacterMovementComponent.h"
#include "Components/RPGHealthComponent.h"
#include "Components/RPGWeaponComponent.h"
#include "Components/RolePlayComponent.h"
#include "Components/TextRenderComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Weapon/RPG_AttackSphere.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseCharacter, All, All);

// Sets default values
ARPGBaseCharacter::ARPGBaseCharacter(const FObjectInitializer &ObjInit)
    : Super(
          ObjInit.SetDefaultSubobjectClass<URPGCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need
    // it.
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);

    HealthComponent = CreateDefaultSubobject<URPGHealthComponent>("HealthComponent");

    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
    HealthTextComponent->SetupAttachment(GetRootComponent());

    ManaTextComponent = CreateDefaultSubobject<UTextRenderComponent>("ManaTextComponent");
    ManaTextComponent->SetupAttachment(GetRootComponent());

    WeaponComponent = CreateDefaultSubobject<URPGWeaponComponent>("WeaponComponent");

    RolePlayComponent = CreateDefaultSubobject<URolePlayComponent>("RolePlayCompponent");
}

// Called when the game starts or when spawned
void ARPGBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(HealthComponent);
    check(HealthTextComponent);
    check(ManaTextComponent);
    check(GetCharacterMovement());

    OnHealthChanged(HealthComponent->GetHealth());
    HealthComponent->OnDeath.AddUObject(this, &ARPGBaseCharacter::OnDeath);
    HealthComponent->OnHealthChanged.AddUObject(this, &ARPGBaseCharacter::OnHealthChanged);

    LandedDelegate.AddDynamic(this, &ARPGBaseCharacter::OnGroundLanded);

    RolePlayComponent->OnLevelUp.AddUObject(this, &ARPGBaseCharacter::LevelUp);

    ChangeMana(MaxMana);
}

// Called every frame
void ARPGBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ARPGBaseCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward-Back", this, &ARPGBaseCharacter::MoveForwardBack);
    PlayerInputComponent->BindAxis("MoveLeft-Right", this, &ARPGBaseCharacter::MoveLeftRight);
    PlayerInputComponent->BindAxis("LookUp", this, &ARPGBaseCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("TurnAround", this, &ARPGBaseCharacter::AddControllerYawInput);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ARPGBaseCharacter::Jump);
    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ARPGBaseCharacter::OnStartRunning);
    PlayerInputComponent->BindAction("Run", IE_Released, this, &ARPGBaseCharacter::OnStopRunning);
    PlayerInputComponent->BindAction("Heal", IE_Pressed, this, &ARPGBaseCharacter::Healing);
    PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ARPGBaseCharacter::Attack);
    PlayerInputComponent->BindAction("ChangeOnSword", IE_Pressed, this, &ARPGBaseCharacter::ChangeAttackTypeOnSword);
    PlayerInputComponent->BindAction("ChangeOnMagic", IE_Pressed, this, &ARPGBaseCharacter::ChangeAttackTypeOnMagic);
    PlayerInputComponent->BindAction("MagicUpgrade", IE_Pressed, this, &ARPGBaseCharacter::MagicUpgrade);
    PlayerInputComponent->BindAction("SwordUpgrade", IE_Pressed, this, &ARPGBaseCharacter::SwordUpgrade);
}

bool ARPGBaseCharacter::IsRunning() const
{
    return WantsToRTun && IsMovingForward && !GetVelocity().IsZero();
}

void ARPGBaseCharacter::ChangeXP(int Value)
{
    RolePlayComponent->SetXP(RolePlayComponent->GetXP() + Value);
}

void ARPGBaseCharacter::ChangeMana(float Value)
{
    Mana = Value;
    ManaTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Mana)));
}

void ARPGBaseCharacter::MoveForwardBack(float Amount)
{
    IsMovingForward = Amount > 0.0f;
    AddMovementInput(GetActorForwardVector(), Amount);
}

void ARPGBaseCharacter::MoveLeftRight(float Amount)
{
    AddMovementInput(GetActorRightVector(), Amount);
}

void ARPGBaseCharacter::OnStartRunning()
{
    WantsToRTun = true;
}

void ARPGBaseCharacter::OnStopRunning()
{
    WantsToRTun = false;
}

void ARPGBaseCharacter::OnDeath()
{
    UE_LOG(LogBaseCharacter, Display, TEXT("Dead"));

    PlayAnimMontage(DeathAnimMontage);

    RolePlayComponent->SpawnXP();

    GetCharacterMovement()->DisableMovement();

    SetLifeSpan(5.0f);

    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void ARPGBaseCharacter::OnHealthChanged(float Health)
{
    HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}

void ARPGBaseCharacter::Healing()
{
    PlayAnimMontage(HealAnimMontage);
    HealthComponent->OnHeal();
}

void ARPGBaseCharacter::LevelUp()
{
    PlayAnimMontage(UpLevelMontage);
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Press SHIFT+M to upgrade Magic"), true,
                                     FVector2D(1.5f, 1.5f));
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Press SHIFT+S to upgrade Sword"), true,
                                     FVector2D(1.5f, 1.5f));
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Your Level Up"), true, FVector2D(2.0f, 2.0f));
    ChangeMana(MaxMana);
}

void ARPGBaseCharacter::OnGroundLanded(const FHitResult &Hit)
{
    const auto FallVelosityZ = -GetVelocity().Z;

    if (FallVelosityZ < LandedDamageVelosity.X)
    {
        return;
    }

    const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelosity, LandedDamage, FallVelosityZ);
    TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
}

void ARPGBaseCharacter::MagicUpgrade()
{
    if (!RolePlayComponent->CanUpScills)
    {
        return;
    }

    RolePlayComponent->MagicSkill += 1;
    if (RolePlayComponent->MagicSkill == 1)
    {
        GEngine->AddOnScreenDebugMessage(-1, 7.0f, FColor::Red, TEXT("Use 1-2 to change attack type"), true,
                                         FVector2D(1.5f, 1.5f));
        GEngine->AddOnScreenDebugMessage(-1, 7.0f, FColor::Red, TEXT("Magic has been learned"), true,
                                         FVector2D(2.0f, 2.0f));
    }
    else
    {
        MaxMana += 20;
        ChangeMana(MaxMana);
        GEngine->AddOnScreenDebugMessage(-1, 7.0f, FColor::Red, TEXT("Mana increased"), true, FVector2D(2.0f, 2.0f));
    }
}

void ARPGBaseCharacter::SwordUpgrade()
{
    if (!RolePlayComponent->CanUpScills)
    {
        return;
    }

    RolePlayComponent->SwordSkill += 1;
    WeaponComponent->SwordUpgrade();
    GEngine->AddOnScreenDebugMessage(-1, 7.0f, FColor::Red, TEXT("Damage increased"), true, FVector2D(2.0f, 2.0f));
}

void ARPGBaseCharacter::ManaUpdate()
{
    ChangeMana(FMath::Min(Mana + ManaModifier, MaxMana));

    if (Mana == MaxMana && GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(ManaTimerHandle);
    }
}

void ARPGBaseCharacter::ChangeAttackTypeOnSword()
{
    SAttack = true;
    MAttack = false;
}

void ARPGBaseCharacter::ChangeAttackTypeOnMagic()
{
    SAttack = false;
    MAttack = true;
}

void ARPGBaseCharacter::Attack()
{
    if (SAttack)
    {
        PlayAnimMontage(SwordAttackMontage, 1.5f);
        WeaponComponent->StartSwordAttack();
        return;
    }
    else if (MAttack && RolePlayComponent->MagicSkill != 0)
    {
        if (Mana < 20)
        {
            GEngine->AddOnScreenDebugMessage(-1, 7.0f, FColor::Red, TEXT("Mana is low"), true, FVector2D(2.0f, 2.0f));
            return;
        }
        MagicAttack();
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Magic has not been learned"), true,
                                         FVector2D(1.5f, 2.0f));
    }
}

void ARPGBaseCharacter::MagicAttack()
{
    PlayAnimMontage(MagicAttackMontage, 1.5f);

    MakeShot();

    GetWorld()->GetTimerManager().ClearTimer(ManaTimerHandle);

    ChangeMana(Mana - 20.0f);

    if (AutoRecovery && GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(ManaTimerHandle, this, &ARPGBaseCharacter::ManaUpdate, ManaUpdateTime,
                                               true, ManaDelay);
    }
}

void ARPGBaseCharacter::MakeShot()
{
    if (!GetWorld())
    {
        return;
    }

    const FTransform SocketTransform = GetMesh()->GetSocketTransform("MagicSocket");
    const FVector TraceStart = SocketTransform.GetLocation();
    const FVector ShootDirection = SocketTransform.GetRotation().GetForwardVector();
    const FVector TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this);
    FHitResult HitResult;

    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility,
                                         CollisionParams);

    const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
    const FVector Direction = (EndPoint - GetMesh()->GetSocketLocation("MagicSocket")).GetSafeNormal();

    const FTransform SpawnTransform(FRotator::ZeroRotator, GetMesh()->GetSocketLocation("MagicSocket"));
    ARPG_AttackSphere *Projectile =
        GetWorld()->SpawnActorDeferred<ARPG_AttackSphere>(AttackSphereClass, SpawnTransform);

    if (Projectile)
    {
        Projectile->SetShotDirection(Direction);
        Projectile->SetOwner(this);
        Projectile->FinishSpawning(SpawnTransform);
    }
}
