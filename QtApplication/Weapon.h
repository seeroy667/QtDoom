/*
Author: Donavan Sirois
Date: Febuary 1, 2026
File name: Weapon.h
Goal: Header file for the Weapon class.
Modifications:
*/

#ifndef WEAPON_H
#define WEAPON_H
#include <QElapsedTimer>

class Weapon {
private:
	int damage;
	float range;
	float fireRate;
    QElapsedTimer m_shootTimer;
    QElapsedTimer m_reloadTimer;

    int   m_maxAmmo;
    int   m_currentAmmo;
    float m_reloadTime;
    bool  m_isReloading = false;

public:
    Weapon(int dmg, float rng, float rate, int maxAmmo,float reloadTime);
	~Weapon();
	void shoot();
	int getDamage();
	float getRange();
	float getFireRate();
    int getCurrentAmmo();
    int getMaxAmmo();



    bool canShoot();
    void reload();
    bool isReloading();
    bool isEmpty();

    void restartShootTimer();
};

#endif
