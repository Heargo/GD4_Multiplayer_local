//HUGO REY D00262075 : Adding the DetectCollisionAndApplyDamage function to the SceneNode class
//this function will check if the projectile is colliding with an entity in the air layer
//The projectile will be destroyed if it is colliding with an entity (see comment in the function)
//modify the function detectCollisionAndApplyDamage to be called from the world class (no need to get damage and radius in parametter anymore)
//the function only detect collision. It also make the player bounce back and get damage when hitting asteroids 

#include "SceneNode.hpp"
#include "ReceiverCategories.hpp"
#include "Command.hpp"
#include <cassert>
#include <memory>
#include "Aircraft.hpp"
#include <iostream>
#include "ProjectileCustom.hpp"
#include "Asteroid.hpp"
SceneNode::SceneNode():m_children(), m_children_to_remove(), m_parent(nullptr)
{
}

void SceneNode::AttachChild(Ptr child)
{
    child->m_parent = this;
    //TODO Why is emplace_back more efficient than push_back
    m_children.emplace_back(std::move(child));
}

SceneNode::Ptr SceneNode::DetachChild(const SceneNode& node)
{
    auto found = std::find_if(m_children.begin(), m_children.end(), [&](Ptr& p) {return p.get() == &node; });
    assert(found != m_children.end());

    Ptr result = std::move(*found);
    result->m_parent = nullptr;
    m_children.erase(found);


    return result;
}

void SceneNode::Update(sf::Time dt, CommandQueue& commands)
{
    UpdateCurrent(dt, commands);
    UpdateChildren(dt, commands);
}

sf::Vector2f SceneNode::GetWorldPosition() const
{
    return GetWorldTransform() * sf::Vector2f();
}

sf::Transform SceneNode::GetWorldTransform() const
{
    sf::Transform transform = sf::Transform::Identity;
    for (const SceneNode* node = this; node != nullptr; node = node->m_parent)
    {
        transform = node->getTransform() * transform;
    }
    return transform;
}

void SceneNode::UpdateCurrent(sf::Time dt, CommandQueue& commands)
{
    //Do nothing here
}

void SceneNode::UpdateChildren(sf::Time dt, CommandQueue& commands)
{
    for (Ptr& child : m_children)
    {
        child->Update(dt, commands);
    }
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    //Apply the transform of the current node
    states.transform *= getTransform();
    //Draw the node and children with changed transform
    DrawCurrent(target, states);
    DrawChildren(target, states);
}

void SceneNode::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    //Do nothing here
}

void SceneNode::DrawChildren(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (const Ptr& child : m_children)
    {
        child->draw(target, states);
    }
}

unsigned int SceneNode::GetCategory() const
{
    return static_cast<unsigned int>(ReceiverCategories::kScene);
}

void SceneNode::OnCommand(const Command& command, sf::Time dt)
{
    //Is this command for me. If it is execute. Regardless of the answer forward it on to all of my children
    if (command.category & GetCategory())
    {
        command.action(*this, dt);
    }

    //Pass it on to the children
    for (Ptr& child : m_children)
    {
        child->OnCommand(command, dt);
    }
}

void SceneNode::DetectCollisionAndApplyDamage()
{
    //std::vector<Ptr&> childrenToRemove={};
	
	//for all children check if this node is colliding with the position and radius
	for (Ptr& child : m_children)
	{
		//skip if null
		if (child == nullptr)
			continue;

        // skip if not aircraft
        if (child->GetCategory() != static_cast<unsigned int>(ReceiverCategories::kPlayerAircraft))
            continue;

        Aircraft* aircraft = static_cast<Aircraft*>(child.get());

        //detect collision with other object in the same layer
		for (Ptr& child2 : m_children)
		{
            //skip if null
            if (child2 == nullptr)
                continue;
            float distance = sqrt(pow((child2->GetWorldPosition().x - aircraft->GetWorldPosition().x), 2) + pow((child2->GetWorldPosition().y - aircraft->GetWorldPosition().y), 2));

            //Projectile collision
            if (child2->GetCategory() == static_cast<unsigned int>(ReceiverCategories::kProjectile))
            {
                //cast child2 to projectile
                ProjectileCustom* projectile = static_cast<ProjectileCustom*>(child2.get());
                //calculate distance between child2 and aircraft
                if (distance <= projectile->getRadius())
                {
                    if (!aircraft->IsDestroyed())
                    {
                        aircraft->ApplyDamage(projectile->getDamage());
						
                        //I know this is not the best way to do this but I tried to use a list of ptr to remove but it did not work
                        DetachChild(*child2);
                        //childrenToRemove.push_back(child2);
                    }
                }

            }

            //Asteroid collision
            else if (child2->GetCategory() == static_cast<unsigned int>(ReceiverCategories::kAsteroid))
            {
                //cast to asteroid
                Asteroid* asteroid = static_cast<Asteroid*>(child2.get());
                if (distance <= asteroid->GetRadius())
                {
                    if (!aircraft->IsDestroyed())
					{
						
                        //calculate and apply damage to inflict to player depending on player velocity
                        float velocityFactor = 1.5 * (abs(aircraft->GetVelocity().x) + abs(aircraft->GetVelocity().y));
                        float sizeFactor = 0.1 * asteroid->GetRadius();
                        float damageToApply = 0.005 * velocityFactor * sizeFactor;
                        //print damages
						std::cout << "dmg :" << damageToApply << std::endl;
                        
                        aircraft->ApplyDamage(damageToApply);

					    //move the aircraft back 5 px in the opposite direction of normalized velocity
					    sf::Vector2f moveBack = aircraft->GetVelocity();
                        moveBack.x = moveBack.x / sqrt(pow(moveBack.x, 2) + pow(moveBack.y, 2)) * 5;
                        moveBack.y = moveBack.y / sqrt(pow(moveBack.x, 2) + pow(moveBack.y, 2)) * 5;
					
                        aircraft->setPosition(aircraft->getPosition() - moveBack);
					
					    //apply bounced velocity
					    aircraft->SetVelocity(aircraft->GetVelocity().x *-1, aircraft->GetVelocity().y * -1);
					}
				}
			}
			
            //detach aircraft if destroyed 
            if (aircraft->IsDestroyed())
            {
                DetachChild(*aircraft);
                //childrenToRemove.push_back(child);
                break;
            }
		}
		
	}
	
    //remove all projectiles from the children list
   /* for (Ptr& child : m_children_to_remove)
    {
        DetachChild(*child);
    }*/
}
