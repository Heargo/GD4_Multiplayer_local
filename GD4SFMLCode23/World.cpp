//HUGO REY D00262075 :apply friction to the player every update call and load the texture for the player and bg
//Add some parallax effect to background
#include "World.hpp"


World::World(sf::RenderWindow& window, FontHolder& font)
	:m_window(window)
	,m_camera(window.getDefaultView())
	,m_textures()
	,m_fonts(font)
	,m_scenegraph()
	,m_scene_layers()
	,m_world_bounds(0.f,0.f, 2000.f, 2000.f) //make the background image bigger that canvas
	,m_spawn_position()
	,m_scrollspeed(-50.f)
	,m_player_aircraft(nullptr)
{

	//center spawn position
	m_spawn_position.x = m_world_bounds.width / 2.f;
	m_spawn_position.y = m_world_bounds.height / 2.f;
	

	LoadTextures();
	BuildScene();

	m_camera.setCenter(m_spawn_position);
	m_player_aircraft->SetVelocity(0.f, 0.f);
}

void World::Update(sf::Time dt)
{
	//Scroll the world with a parallax effect depending of player 1 position
	float parallax = 0.1f;
	sf::Vector2f velocity = m_player_aircraft->GetVelocity();
	//get length of vector
	float length = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
	
	//move the background if the player goes fast enough
	if(length > 50.f)
		m_camera.move(velocity.x * parallax * dt.asSeconds(), velocity.y * parallax * dt.asSeconds());
	

	//Forward the commands to the scenegraph, sort out velocity
	while (!m_command_queue.IsEmpty())
	{
		m_scenegraph.OnCommand(m_command_queue.Pop(), dt);
	}
	
	//apply friction to the player movement
	AdaptPlayerPosition();
	m_player_aircraft->ApplyFriction();

	m_scenegraph.Update(dt, m_command_queue);
	
}

void World::Draw()
{
	m_window.setView(m_camera);
	m_window.draw(m_scenegraph);
}

CommandQueue& World::GetCommandQueue()
{
	return m_command_queue;
}

void World::LoadTextures()
{
	//load background texture
	m_textures.Load(Texture::kBackground, "Media/Textures/greenNebula.png");
	//load player 1 
	m_textures.Load(Texture::kPlayer1, "Media/Textures/Spaceships/01/Spaceship_01_BLUE.png");
	m_textures.Load(Texture::kPlayer2, "Media/Textures/Spaceships/01/Spaceship_01_RED.png");
	
}

void World::BuildScene()
{
	//Initialize the different layers
	for(std::size_t i=0; i < static_cast<int>(Layers::kLayerCount); ++i)
	{ 
		SceneNode::Ptr layer(new SceneNode());
		m_scene_layers[i] = layer.get();
		m_scenegraph.AttachChild(std::move(layer));
	}

	//Prepare the background
	sf::Texture& texture = m_textures.Get(Texture::kBackground);
	sf::IntRect textureRect(m_world_bounds);
	texture.setRepeated(true);

	//Add the background sprite to the world
	std::unique_ptr<SpriteNode> background_sprite(new SpriteNode(texture, textureRect));
	background_sprite->setPosition(m_world_bounds.left, m_world_bounds.top);
	m_scene_layers[static_cast<int>(Layers::kBackground)]->AttachChild(std::move(background_sprite));

	//Add player's aircraft
	std::unique_ptr<Aircraft> leader(new Aircraft(AircraftType::kEagle, m_textures, m_fonts));
	m_player_aircraft = leader.get();
	m_player_aircraft->setPosition(m_spawn_position);
	m_player_aircraft->SetVelocity(40.f, m_scrollspeed);

	m_scene_layers[static_cast<int>(Layers::kAir)]->AttachChild(std::move(leader));

}

void World::AdaptPlayerPosition()
{
	//Keep the player on the scene 
	sf::FloatRect view_bounds(m_camera.getCenter() - m_camera.getSize() / 2.f, m_camera.getSize());
	const float border_distance = 40.f;

	sf::Vector2f position = m_player_aircraft->getPosition();
	position.x = std::max(position.x, view_bounds.left + border_distance);
	position.x = std::min(position.x, view_bounds.left + view_bounds.width - border_distance);
	position.y = std::max(position.y, view_bounds.top + border_distance);
	position.y = std::min(position.y, view_bounds.top + view_bounds.height - border_distance);
	m_player_aircraft->setPosition(position);
}

void World::AdaptPlayerVelocity()
{
	sf::Vector2f velocity = m_player_aircraft->GetVelocity();

	//If they are moving diagonally divide by root 2
	if (velocity.x != 0.f && velocity.y != 0.f)
	{
		m_player_aircraft->SetVelocity(velocity / std::sqrt(2.f));
	}

	//Add scrolling velocity
	//m_player_aircraft->Accelerate(0.f, m_scrollspeed);
}
