// HUGO REY D00262075 : fix the maximum size of aircraft to 100px*100px
// Add Fire() to shoot projectiles.

#include "Aircraft.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include "ResourceHolder.hpp"
#include "ReceiverCategories.hpp"
#include "Texture.hpp"
#include "DataTables.hpp"
#include "Utility.hpp"
#include <iostream>
#include "ProjectileType.hpp"
#include "Layers.hpp"

namespace
{
	const std::vector<AircraftData> Table = InitializeAircraftData();
}

Texture ToTextureID(AircraftType type)
{
	switch (type)
	{
	case AircraftType::kPlayer1:
		return Texture::kPlayer1;
		break;
	case AircraftType::kPlayer2:
		return Texture::kPlayer2;
		break;
	}
	return Texture::kPlayer1;
}

Aircraft::Aircraft(AircraftType type, const TextureHolder& textures, const FontHolder& fonts, SceneNode* m_air_layer)
	: Entity(Table[static_cast<int>(type)].m_hitpoints)
	, m_type(type) 
	, m_sprite(textures.Get(ToTextureID(type)))
	, m_fire_rate(1)
	, m_spread_level(1)
	, m_missile_ammo(2)
	, m_health_display(nullptr)
	, m_missile_display(nullptr)
	, m_travelled_distance(0.f)
	, m_directions_index(0)
	, m_textures(textures)
	, m_air_layer(m_air_layer)
{
	sf::FloatRect bounds = m_sprite.getLocalBounds();
	m_sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	std::string empty_string = "";

	//fix max size of sprite to 100x100
	m_sprite.setScale(100.f / bounds.width, 100.f / bounds.height);

	std::unique_ptr<TextNode> health_display(new TextNode(fonts, empty_string));
	m_health_display = health_display.get();
	AttachChild(std::move(health_display));

	if (GetCategory() == static_cast<int>(ReceiverCategories::kPlayerAircraft))
	{
		std::unique_ptr<TextNode> missile_display(new TextNode(fonts, empty_string));
		missile_display->setPosition(0, 70);
		m_missile_display = missile_display.get();
		AttachChild(std::move(missile_display));
	}
	UpdateTexts();

}

unsigned int Aircraft::GetCategory() const
{
	switch (m_type)
	{
	case AircraftType::kPlayer1:
		return static_cast<unsigned int>(ReceiverCategories::kPlayerAircraft);
		
	case AircraftType::kPlayer2:
		return static_cast<unsigned int>(ReceiverCategories::kPlayerAircraft);
		
	default:
		return static_cast<unsigned int>(ReceiverCategories::kEnemyAircraft);

	}
}

void Aircraft::IncreaseFireRate()
{
	if (m_fire_rate < 5)
	{
		++m_fire_rate;
	}
}

void Aircraft::IncreaseFireSpread()
{
	if (m_spread_level < 3)
	{
		++m_spread_level;
	}
}

void Aircraft::CollectMissiles(unsigned int count)
{
	m_missile_ammo += count;
}

void Aircraft::UpdateTexts()
{
	m_health_display->SetString(std::to_string(GetHitPoints()) + "HP");
	m_health_display->setPosition(0.f, 50.f);
	m_health_display->setRotation(-getRotation());

	if (m_missile_ammo)
	{
		if (m_missile_ammo == 0)
		{
			m_missile_display->SetString("");
		}
		else
		{
			m_missile_display->SetString("M: " + std::to_string(m_missile_ammo));
		}
	}
}

/* tried to add shooting to Aircraft class
void::Aircraft(Type type, const TextureHolder& textures)
{
	m_fire_command.category = Category::SceneAirLayer;
	m_fire_command.action = [this, &textures](SceneNode& node, sf::Time)
	{
		createBullets(node, textures);
	};
	m_missile_command.category = Category::SceneAirLayer;
	m_missile_command.action = [this, &textures](SceneNode& node, sf::Time)
	{
		create_projectile(node, Projectile::Missile, 0.f, 0.5f, textures);
	};
}

void Aircarft::createBullets(SceneNode& node, const TextureHolder& textures) const
{
	Projectile::Type type = isAllied() ? Projectile::AlliedBullet : Projectile::EnemyBullet;
	switch (mSpreadLevel)
	{
	case 1:
		createProjectile(node, type, 0.0f, 0.5f, textures);
		break;
	case 2:
		createProjectile(node, type, -0.33f, 0.33f, textures);
		createProjectile(node, type, 0.33f, 0.33f, textures);
		break;
	case 3:
		createProjectile(node, type, -0.5f, 0.33f, textures);
		createProjectile(node, type, 0.0f, 0.5f, textures);
		createProjectile(node, type, 0.5f, 0.33f, textures);
		break;
	}
}

void Aircraft::createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const
{
	std::unique_ptr<Projectile> projectile(new Projectile(type, textures));
	sf::Vector2f offset(xOffset * mSprite.getGlobalBounds().width, yOffset * mSprite.getGlobalBounds().height);
	sf::Vector2f velocity(0, projectile->getMaxSpeed());
	float sign = isAllied() ? -1.f : +1.f;
	projectile->setPosition(getWorldPosition() + offset * sign);
	projectile->setVelocity(velocity * sign);
	node.attachChild(std::move(projectile));
}
*/




void Aircraft::UpdateMovementPattern(sf::Time dt)
{
	//Enemy AI
	const std::vector<Direction>& directions = Table[static_cast<int>(m_type)].m_directions;
	if (!directions.empty())
	{
		//Move along the current direction, then change direction
		if (m_travelled_distance > directions[m_directions_index].m_distance)
		{
			m_directions_index = (m_directions_index + 1) % directions.size();
			m_travelled_distance = 0;
		}

		//Compute velocity
		double radians = Utility::ToRadians(directions[m_directions_index].m_angle + 90.f);
		float vx = GetMaxSpeed() * std::cos(radians);
		float vy = GetMaxSpeed() * std::sin(radians);

		SetVelocity(vx, vy);
		m_travelled_distance += GetMaxSpeed() * dt.asSeconds();



	}
}

float Aircraft::GetMaxSpeed() const
{
	return Table[static_cast<int>(m_type)].m_speed;
}

void Aircraft::Fire()
{
	//print to console to check if firing
	std::cout << "Firing" << std::endl;

	//get the direction of the aircraft
	float rotation = getRotation();
	
	//create a bullet
	ProjectileType::Type bulletType;
	switch (m_type)
	{
	case AircraftType::kPlayer1:
		bulletType = ProjectileType::Type::kPlayer1Bullet;
		break;
	case AircraftType::kPlayer2:
		bulletType = ProjectileType::Type::kPlayer2Bullet;
		break;

	default:
		bulletType = ProjectileType::Type::kPlayer1Bullet;
		break;
	}

	std::unique_ptr<ProjectileType> bullet(new ProjectileType(bulletType, m_textures, m_air_layer));
	bullet->setPosition(getPosition());
	bullet->setRotation(rotation);
	//set the velocity of the bullet depending on the rotation of the aircraft
	switch ((int)rotation)
	{
		case 0:
			bullet->SetVelocity(0, -1000);
			break;
		case 90:
			bullet->SetVelocity(1000, 0);
			break;
		case 180:
			bullet->SetVelocity(0, 1000);
			break;
		case 270:
			bullet->SetVelocity(-1000, 0);
			break;
		default:
			bullet->SetVelocity(0, -1000);
			break;
	}
	
	//add bullet to air layout
	m_air_layer->AttachChild(std::move(bullet));
	
}

AircraftType Aircraft::GetType()
{
	return m_type;
}

void Aircraft::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
}

void Aircraft::UpdateCurrent(sf::Time dt, CommandQueue& commands)
{
	UpdateTexts();
	Entity::UpdateCurrent(dt, commands);
}
