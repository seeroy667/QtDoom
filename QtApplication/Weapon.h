/*
Author: Donavan Sirois
Date: Febuary 1, 2026
File name: Weapon.h
Goal: Header file for the Weapon class.
Modifications:
*/

#ifndef WEAPON_H
#define WEAPON_H

class Weapon {
private:
	int damage;
	float range;
	float fireRate;
public:
	Weapon(int dmg, float rng, float rate);
	~Weapon();
	void shoot();
	int getDamage();
	float getRange();
	float getFireRate();
};

#endif
