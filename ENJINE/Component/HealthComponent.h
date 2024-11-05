#pragma once
#include "Component.h"

class HealthComponent : public Component
{
public:
	float max_health;
	float current_health;

	HealthComponent(float max_h = 100, float current_h = 100) : 
		max_health(max_h),
		current_health(current_h)
	{
		setType(Component::HealthComponent);
	}

	//void	setHealth(float val)		{ current_health = val; }
	//float	getHealth()					{ return current_health; }

	//void	setMaxHealth(float val)		{ max_health = val; }
	//float	getMaxHealth()				{ return max_health; }
};

// NETWORK
sf::Packet& operator <<(sf::Packet& packet, HealthComponent& c)
{
	return packet << c.current_health << c.max_health;
}
sf::Packet& operator >>(sf::Packet& packet, HealthComponent& c)
{
	return packet >> c.current_health >> c.max_health;
}

