//HUGO REY D00262075 : Adding the DetectCollisionAndApplyDamage function to the SceneNode class
//this function will check if the projectile is colliding with an entity in the air layer
//The projectile will be destroyed if it is colliding with an entity (see comment in the function)

#include "SceneNode.hpp"
#include "ReceiverCategories.hpp"
#include "Command.hpp"
#include <cassert>
#include <memory>
#include "Aircraft.hpp"
#include <iostream>
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

void SceneNode::DetectCollisionAndApplyDamage(sf::Vector2f position, float radius,float damage)
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

        //if there is a ProjectileCustom then apply damage to child 
		for (Ptr& child2 : m_children)
		{
            if (child2->GetCategory() == static_cast<unsigned int>(ReceiverCategories::kProjectile))
            {
				//calculate distance between child2 and aircraft
				float distance = sqrt(pow((child2->GetWorldPosition().x - aircraft->GetWorldPosition().x), 2) + pow((child2->GetWorldPosition().y - aircraft->GetWorldPosition().y), 2));
                if (distance <= radius)
                {
                    aircraft->ApplyDamage(damage);
                    //I know this is not the best way to do this but I tried to use a list of ptr to remove but it did not work
                    DetachChild(*child2);
                }
                
            }
		}
		
	}
	
    //remove all projectiles from the children list
   /* for (Ptr& child : m_children_to_remove)
    {
        DetachChild(*child);
    }*/
}
