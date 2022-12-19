// HUGO REY D00262075 : remove no necessary enum for the game
#pragma once
enum class ReceiverCategories
{
	kNone = 0,
	kScene = 1 << 0,
	kPlayerAircraft = 1 << 1,
	kPlayer1Aircraft = 1 << 2,
	kPlayer2Aircraft = 1 << 3,
	kPickup = 1 << 4,
	kPlayer1Projectile = 1 << 5,
	kPlayer2Projectile = 1 << 6,

	kProjectile = kPlayer1Projectile | kPlayer2Projectile,


	kAircraft = kPlayerAircraft | kPlayer1Aircraft | kPlayer2Aircraft,
};

//A message that would be sent to all aircraft
//unsigned int all_aircraft = ReceiverCategories::kPlayerAircraft | ReceiverCategories::kPlayer1Aircraft | ReceiverCategories::kPlayer2Aircraft;