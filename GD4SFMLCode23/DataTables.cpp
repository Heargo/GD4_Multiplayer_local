// HUGO REY D00262075 : remove useless data (AI) and switch textures

#include "DataTables.hpp"
#include "AircraftType.hpp"
#include "Aircraft.hpp"
#include "PickupType.hpp"
#include "ProjectileType.hpp"

std::vector<AircraftData> InitializeAircraftData()
{
    std::vector<AircraftData> data(static_cast<int>(AircraftType::kAircraftCount));

    data[static_cast<int>(AircraftType::kEagle)].m_hitpoints = 100;
    data[static_cast<int>(AircraftType::kEagle)].m_speed = 200.f;
    data[static_cast<int>(AircraftType::kEagle)].m_fire_interval = sf::seconds(1);
    data[static_cast<int>(AircraftType::kEagle)].m_texture = Texture::kPlayer1;

    return data;
}

std::vector<ProjectileData> InitializeProjectileData()
{
    std::vector<ProjectileData> data(static_cast<int>(ProjectileType::kProjectileCount));

    data[static_cast<int>(ProjectileType::kAlliedBullet)].m_damage = 10;
    data[static_cast<int>(ProjectileType::kAlliedBullet)].m_speed = 300;
    data[static_cast<int>(ProjectileType::kAlliedBullet)].m_texture = Texture::kBullet;

    data[static_cast<int>(ProjectileType::kEnemyBullet)].m_damage = 10;
    data[static_cast<int>(ProjectileType::kEnemyBullet)].m_speed = 300;
    data[static_cast<int>(ProjectileType::kEnemyBullet)].m_texture = Texture::kBullet;

    data[static_cast<int>(ProjectileType::kMissile)].m_damage = 200;
    data[static_cast<int>(ProjectileType::kAlliedBullet)].m_speed = 350;
    data[static_cast<int>(ProjectileType::kMissile)].m_texture = Texture::kMissile;
    return data;
}

std::vector<PickupData> InitializePickupData()
{
    std::vector<PickupData> data(static_cast<int>(PickupType::kPickupCount));

    data[static_cast<int>(PickupType::kHealthRefill)].m_texture = Texture::kHealthRefill;
    data[static_cast<int>(PickupType::kHealthRefill)].m_action = [](Aircraft& a) {a.Repair(25); };

    data[static_cast<int>(PickupType::kMissileRefill)].m_texture = Texture::kMissileRefill;
    data[static_cast<int>(PickupType::kMissileRefill)].m_action = std::bind(&Aircraft::CollectMissiles, std::placeholders::_1, 3);

    data[static_cast<int>(PickupType::kFireSpread)].m_texture = Texture::kFireSpread;
    data[static_cast<int>(PickupType::kFireSpread)].m_action = std::bind(&Aircraft::IncreaseFireSpread, std::placeholders::_1);

    data[static_cast<int>(PickupType::kFireRate)].m_texture = Texture::kFireRate;
    data[static_cast<int>(PickupType::kFireRate)].m_action = std::bind(&Aircraft::IncreaseFireRate, std::placeholders::_1);
    return data;    
}
