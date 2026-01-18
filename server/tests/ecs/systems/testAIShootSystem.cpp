/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** testAIShootSystem
*/

#include <gtest/gtest.h>

#include <cmath>
#include <numbers>
#include <vector>
#include <type_traits>

#include "AIShootSystem.hpp"

#include "Events.hpp"
#include "EventsRegistry.hpp"
#include "IGameWorld.hpp"
#include "Registry.hpp"

#include "AIBrain.hpp"
#include "AIShoot.hpp"
#include "Position.hpp"
#include "WeaponConfig.hpp"

namespace test_adapt
{

    template <class R>
    Ecs::Entity createEntity(R &reg)
    {
        if constexpr (requires { reg.spawn_entity(); }) {
            return reg.spawn_entity();
        } else if constexpr (requires { reg.create_entity(); }) {
            return reg.create_entity();
        } else if constexpr (requires { reg.createEntity(); }) {
            return reg.createEntity();
        } else {
            static_assert(sizeof(R) == 0, "Registry has no known entity creation method");
        }
    }

    template <class R, class C>
    void add(R &reg, Ecs::Entity e, C &&c)
    {
        using T = std::remove_cvref_t<C>;

        if constexpr (requires { reg.template addComponent<T>(e, std::forward<C>(c)); }) {
            (void) reg.template addComponent<T>(e, std::forward<C>(c));
        } else if constexpr (requires { reg.template add_component<T>(e, std::forward<C>(c)); }) {
            (void) reg.template add_component<T>(e, std::forward<C>(c));
        } else if constexpr (requires { reg.template emplace_component<T>(e, std::forward<C>(c)); }) {
            (void) reg.template emplace_component<T>(e, std::forward<C>(c));
        } else if constexpr (requires { reg.template emplaceComponent<T>(e, std::forward<C>(c)); }) {
            (void) reg.template emplaceComponent<T>(e, std::forward<C>(c));
        } else if constexpr (requires { reg.template emplace<T>(e, std::forward<C>(c)); }) {
            (void) reg.template emplace<T>(e, std::forward<C>(c));
        } else {
            static_assert(sizeof(R) == 0, "Registry has no known add/emplace component method");
        }
    }

    template <class R, class T>
    T *getPtr(R &reg, Ecs::Entity e)
    {
        if constexpr (requires { reg.template get<T>(e); }) {
            return &reg.template get<T>(e);
        } else if constexpr (requires { reg.template get_component<T>(e); }) {
            return &reg.template get_component<T>(e);
        } else if constexpr (requires { reg.template getComponent<T>(e); }) {
            return &reg.template getComponent<T>(e);
        } else {
            return nullptr;
        }
    }

} // namespace test_adapt

class TestWorld final : public Game::IGameWorld {
  public:
    Ecs::Registry &registry() override
    {
        return _reg;
    }

    Ecs::EventsRegistry &events() override
    {
        return _events;
    }

    Ecs::Entity createEntity() override
    {
        return test_adapt::createEntity(_reg);
    }

    Ecs::Entity createPlayer(int sessionId, size_t index) override
    {
        return Ecs::Entity{};
    }

    void destroyEntity(Ecs::Entity ent) override
    {
    }

    void copyFrom(IGameWorld &other) override
    {
    }

  private:
    Ecs::Registry _reg;
    Ecs::EventsRegistry _events;
};

namespace
{

    static Ecs::Entity spawnAI(
        TestWorld &w, const Ecs::AIShoot &shoot, const Ecs::Position &pos, const Ecs::WeaponConfig &weapon)
    {
        auto &reg = w.registry();
        const Ecs::Entity e = test_adapt::createEntity(reg);

        test_adapt::add(reg, e, Ecs::AIBrain{});
        test_adapt::add(reg, e, shoot);
        test_adapt::add(reg, e, pos);
        test_adapt::add(reg, e, weapon);

        return e;
    }

    static float deg2rad(float deg)
    {
        return deg * (std::numbers::pi_v<float> / 180.f);
    }

} // namespace

TEST(AIShootSystem, DoesNotShootBeforeCooldown)
{
    TestWorld w;
    Game::AIShootSystem sys;

    std::vector<ShootEvent> captured;
    w.events().subscribe<ShootEvent>([&](const ShootEvent &ev) {
        captured.push_back(ev);
    });

    Ecs::AIShoot shoot{};
    shoot.cooldown = 1.0f;
    shoot.timer = 0.0f;
    shoot.projectileSpeed = 100.0f;
    shoot.damage = 5;
    shoot.muzzle = {0.f, 0.f};
    shoot.type = Ecs::AIShoot::Type::Straight;

    Ecs::Position pos{10.f, 20.f};
    Ecs::WeaponConfig weapon{};
    weapon.projectileSpriteId = 42;

    (void) spawnAI(w, shoot, pos, weapon);

    sys.update(w, 0.99f);
    w.events().process();
    EXPECT_TRUE(captured.empty());

    sys.update(w, 0.02f);
    w.events().process();
    EXPECT_EQ(captured.size(), 1u);
}

TEST(AIShootSystem, StraightShootsOneProjectileWithMuzzleOffsetAndResetsTimerIfReadable)
{
    TestWorld w;
    Game::AIShootSystem sys;

    std::vector<ShootEvent> captured;
    w.events().subscribe<ShootEvent>([&](const ShootEvent &ev) {
        captured.push_back(ev);
    });

    Ecs::AIShoot shoot{};
    shoot.cooldown = 0.5f;
    shoot.timer = 0.49f;
    shoot.projectileSpeed = 120.0f;
    shoot.damage = 7;
    shoot.muzzle = {3.f, -4.f};
    shoot.type = Ecs::AIShoot::Type::Straight;

    Ecs::Position pos{10.f, 20.f};
    Ecs::WeaponConfig weapon{};
    weapon.projectileSpriteId = 9;

    const auto e = spawnAI(w, shoot, pos, weapon);

    sys.update(w, 0.02f);
    w.events().process();

    ASSERT_EQ(captured.size(), 1u);
    const auto &ev = captured[0];

    EXPECT_FLOAT_EQ(ev.x, pos.x + shoot.muzzle.first);
    EXPECT_FLOAT_EQ(ev.y, pos.y + shoot.muzzle.second);
    EXPECT_FLOAT_EQ(ev.vx, -shoot.projectileSpeed);
    EXPECT_FLOAT_EQ(ev.vy, 0.f);
    EXPECT_EQ(ev.damage, shoot.damage);
    EXPECT_EQ(ev.shooter, static_cast<size_t>(e));
    EXPECT_EQ(ev.spriteId, weapon.projectileSpriteId);

    if (auto *shootRef = test_adapt::getPtr<Ecs::Registry, Ecs::AIShoot>(w.registry(), e)) {
        EXPECT_FLOAT_EQ(shootRef->timer, 0.f);
    }
}

TEST(AIShootSystem, HomingUsesSameStraightVelocityRule)
{
    TestWorld w;
    Game::AIShootSystem sys;

    std::vector<ShootEvent> captured;
    w.events().subscribe<ShootEvent>([&](const ShootEvent &ev) {
        captured.push_back(ev);
    });

    Ecs::AIShoot shoot{};
    shoot.cooldown = 0.0f;
    shoot.timer = 0.0f;
    shoot.projectileSpeed = 80.0f;
    shoot.damage = 1;
    shoot.muzzle = {0.f, 0.f};
    shoot.type = Ecs::AIShoot::Type::Homing;

    Ecs::Position pos{0.f, 0.f};
    Ecs::WeaponConfig weapon{};
    weapon.projectileSpriteId = 1;

    (void) spawnAI(w, shoot, pos, weapon);

    sys.update(w, 0.01f);
    w.events().process();

    ASSERT_EQ(captured.size(), 1u);
    EXPECT_FLOAT_EQ(captured[0].vx, -shoot.projectileSpeed);
    EXPECT_FLOAT_EQ(captured[0].vy, 0.f);
}

TEST(AIShootSystem, SpreadEmitsBulletsNbrWithExpectedYPattern)
{
    TestWorld w;
    Game::AIShootSystem sys;

    std::vector<ShootEvent> captured;
    w.events().subscribe<ShootEvent>([&](const ShootEvent &ev) {
        captured.push_back(ev);
    });

    Ecs::AIShoot shoot{};
    shoot.cooldown = 0.0f;
    shoot.timer = 0.0f;
    shoot.projectileSpeed = 50.0f;
    shoot.damage = 3;
    shoot.muzzle = {0.f, 0.f};
    shoot.type = Ecs::AIShoot::Type::Spread;
    shoot.bulletsNbr = 3;

    Ecs::Position pos{100.f, 200.f};
    Ecs::WeaponConfig weapon{};
    weapon.projectileSpriteId = 77;

    const auto e = spawnAI(w, shoot, pos, weapon);

    sys.update(w, 0.01f);
    w.events().process();

    ASSERT_EQ(captured.size(), static_cast<size_t>(shoot.bulletsNbr));

    const float posX = pos.x + shoot.muzzle.first;
    const float posY = pos.y + shoot.muzzle.second;

    const float startY = posY - (static_cast<float>(shoot.bulletsNbr) - 1.f) * 10.f;
    for (int i = 0; i < shoot.bulletsNbr; ++i) {
        const float expectedY = startY + static_cast<float>(i) * 50.f;
        const auto &ev = captured[static_cast<size_t>(i)];

        EXPECT_FLOAT_EQ(ev.x, posX);
        EXPECT_FLOAT_EQ(ev.y, expectedY);
        EXPECT_FLOAT_EQ(ev.vx, -shoot.projectileSpeed);
        EXPECT_FLOAT_EQ(ev.vy, 0.f);
        EXPECT_EQ(ev.shooter, static_cast<size_t>(e));
        EXPECT_EQ(ev.spriteId, weapon.projectileSpriteId);
    }
}

TEST(AIShootSystem, AnglesEmitsOnePerAngleWithCosSinVelocity)
{
    TestWorld w;
    Game::AIShootSystem sys;

    std::vector<ShootEvent> captured;
    w.events().subscribe<ShootEvent>([&](const ShootEvent &ev) {
        captured.push_back(ev);
    });

    Ecs::AIShoot shoot{};
    shoot.cooldown = 0.0f;
    shoot.timer = 0.0f;
    shoot.projectileSpeed = 100.0f;
    shoot.damage = 2;
    shoot.muzzle = {5.f, 6.f};

    shoot.type = Ecs::AIShoot::Type::Diagonal;
    shoot.angles = {0.f, 30.f, 90.f};

    Ecs::Position pos{10.f, 20.f};
    Ecs::WeaponConfig weapon{};
    weapon.projectileSpriteId = 3;

    (void) spawnAI(w, shoot, pos, weapon);

    sys.update(w, 0.01f);
    w.events().process();

    ASSERT_EQ(captured.size(), shoot.angles.size());

    const float ex = pos.x + shoot.muzzle.first;
    const float ey = pos.y + shoot.muzzle.second;

    for (size_t i = 0; i < shoot.angles.size(); ++i) {
        const float a = deg2rad(shoot.angles[i]);
        const float vx = -shoot.projectileSpeed * std::cos(a);
        const float vy = -shoot.projectileSpeed * std::sin(a);

        const auto &ev = captured[i];
        EXPECT_FLOAT_EQ(ev.x, ex);
        EXPECT_FLOAT_EQ(ev.y, ey);
        EXPECT_NEAR(ev.vx, vx, 1e-4f);
        EXPECT_NEAR(ev.vy, vy, 1e-4f);
    }
}
