/*
Author: Donavan Sirois
Date: Febuary 1, 2026
File name: Weapon.cpp
Goal: Code file for the weapon class.
Modifications:
*/

#include "weapon.h"

Weapon::Weapon(int dmg, float rng, float rate)
{
	damage = dmg;
	range = rng;
	fireRate = rate;
      m_shootTimer.start();
}

Weapon::~Weapon()
{
}

bool Weapon::canShoot()
{
    float cd = (1.0f / fireRate) * 1000.0f;
     return m_shootTimer.elapsed() >= cd;
}
void Weapon::shoot()
{
    m_shootTimer.restart();
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
