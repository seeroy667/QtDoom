/*
Author: Donavan Sirois
Date: Febuary 1, 2026
File name: Weapon.cpp
Goal: Code file for the weapon class.
Modifications:
*/

#include "Weapon.h"

Weapon::Weapon(int dmg, float rng, float rate)
{
	damage = dmg;
	range = rng;
	fireRate = rate;
}

Weapon::~Weapon()
{
}

void Weapon::shoot()
{
	// Shooting logic here
}

int Weapon::getDamage()
{
	return damage;
}

float Weapon::getRange()
{
	return range;
}

float Weapon::getFireRate()
{
	return fireRate;
}