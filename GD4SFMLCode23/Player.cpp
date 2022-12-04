// HUGO REY D00262075 : changes name of variable in InitializeActions to make it fit what's happening (player increase acceleration)
// add second player to the game (player 2) wich include new actions and new controls


#include "Player.hpp"
#include "CommandQueue.hpp"
#include "Aircraft.hpp"

#include <map>
#include <string>
#include <algorithm>

struct AircraftMover
{
	AircraftMover(float vx, float vy, AircraftType type)
        : velocity(vx, vy)
    {
		this->type = type;
    }
    void operator()(Aircraft& aircraft, sf::Time) const
    {
        if (type == aircraft.GetType())
        {
            aircraft.Accelerate(velocity);
            aircraft.Rotate(velocity);
        };
    }
    sf::Vector2f velocity;
	AircraftType type;
};

Player::Player()
{
    //Set initial key bindings for player 1
    m_key_binding[sf::Keyboard::Left] = Action::kMoveLeft1;
    m_key_binding[sf::Keyboard::Right] = Action::kMoveRight1;
    m_key_binding[sf::Keyboard::Up] = Action::kMoveUp1;
    m_key_binding[sf::Keyboard::Down] = Action::kMoveDown1;

	//player 2
    m_key_binding[sf::Keyboard::A] = Action::kMoveLeft2;
    m_key_binding[sf::Keyboard::D] = Action::kMoveRight2;
    m_key_binding[sf::Keyboard::W] = Action::kMoveUp2;
    m_key_binding[sf::Keyboard::S] = Action::kMoveDown2;

    //Set initial action bindings
    InitializeActions();

    //Assign all categories to a player's aircraft
    for (auto& pair : m_action_binding)
    {
        pair.second.category = static_cast<unsigned int>(ReceiverCategories::kPlayerAircraft);
    }

}


void Player::HandleEvent(const sf::Event& event, CommandQueue& command)
{
    if (event.type == sf::Event::KeyPressed)
    {
        auto found = m_key_binding.find(event.key.code);
        if (found != m_key_binding.end() && !IsRealtimeAction(found->second))
        {
            command.Push(m_action_binding[found->second]);
        }
    }
}

void Player::HandleRealtimeInput(CommandQueue& command)
{
    //Check if any of the key binding keys are pressed
    for (auto pair : m_key_binding)
    {
        if (sf::Keyboard::isKeyPressed(pair.first) && IsRealtimeAction(pair.second))
        {
            command.Push(m_action_binding[pair.second]);
        }
    }
}

void Player::AssignKey(Action action, sf::Keyboard::Key key)
{
    //Remove all keys that are currently bound to the action
    for (auto itr = m_key_binding.begin(); itr != m_key_binding.end();)
    {
        if (itr->second == action)
        {
            m_key_binding.erase(itr++);
        }
        else
        {
            ++itr;
        }
    }
    m_key_binding[key] = action;
}

sf::Keyboard::Key Player::GetAssignedKey(Action action) const
{
    for (auto pair : m_key_binding)
    {
        if (pair.second == action)
        {
            return pair.first;
        }
    }
    return sf::Keyboard::Unknown;
}

void Player::InitializeActions()
{
    //TODO Normalize to avoid faster movement along diagonals
    const float kPlayerAcceleration = 5.f;
    m_action_binding[Action::kMoveLeft1].action = DerivedAction<Aircraft>(AircraftMover(-kPlayerAcceleration, 0.f, AircraftType::kPlayer1));
    m_action_binding[Action::kMoveRight1].action = DerivedAction<Aircraft>(AircraftMover(kPlayerAcceleration, 0.f, AircraftType::kPlayer1));
    m_action_binding[Action::kMoveUp1].action = DerivedAction<Aircraft>(AircraftMover(0.f, -kPlayerAcceleration, AircraftType::kPlayer1));
    m_action_binding[Action::kMoveDown1].action = DerivedAction<Aircraft>(AircraftMover(0.f, kPlayerAcceleration, AircraftType::kPlayer1));

    m_action_binding[Action::kMoveLeft2].action = DerivedAction<Aircraft>(AircraftMover(-kPlayerAcceleration, 0.f, AircraftType::kPlayer2));
    m_action_binding[Action::kMoveRight2].action = DerivedAction<Aircraft>(AircraftMover(kPlayerAcceleration, 0.f, AircraftType::kPlayer2));
    m_action_binding[Action::kMoveUp2].action = DerivedAction<Aircraft>(AircraftMover(0.f, -kPlayerAcceleration, AircraftType::kPlayer2));
    m_action_binding[Action::kMoveDown2].action = DerivedAction<Aircraft>(AircraftMover(0.f, kPlayerAcceleration, AircraftType::kPlayer2));
}

bool Player::IsRealtimeAction(Action action)
{
    switch (action)
    {
    case Action::kMoveDown1:
    case Action::kMoveUp1:
    case Action::kMoveLeft1:
    case Action::kMoveRight1:
    case Action::kMoveDown2:
    case Action::kMoveUp2:
    case Action::kMoveLeft2:
    case Action::kMoveRight2:
        return true;
    default:
        return false;
    }
}
