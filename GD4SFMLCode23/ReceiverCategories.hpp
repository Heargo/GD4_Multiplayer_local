// HUGO REY D00262075 : remove no necessary enum for the game
#pragma once
enum class ReceiverCategories
{
	kNone = 0,
	kScene = 1 << 0,
	kPlayerAircraft = 1 << 1,
	kEnemyAircraft = 1 << 2,
	kPickup = 1 << 3,
	kAlliedProjectile = 1 << 4,
	kEnemyProjectile = 1 << 5,

	kProjectile = kAlliedProjectile | kEnemyProjectile,


	kAircraft = kPlayerAircraft | kPlayerAircraft | kEnemyAircraft,
};

//A message that would be sent to all aircraft
//unsigned int all_aircraft = ReceiverCategories::kPlayerAircraft | ReceiverCategories::kAlliedAircraft | ReceiverCategories::kEnemyAircraft;