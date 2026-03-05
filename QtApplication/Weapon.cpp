/*
Author: Donavan Sirois
Date: Febuary 1, 2026
File name: Weapon.cpp
Goal: Code file for the weapon class.
Modifications:
*/

#include "weapon.h"

Weapon::Weapon(int dmg, float rng, float rate, int maxAmmo, float reloadTime)
{
    damage = dmg;
    range = rng;
    fireRate = rate;
    m_maxAmmo = maxAmmo;
    m_currentAmmo = maxAmmo;
    m_reloadTime  = reloadTime;

    m_shootTimer.start();
    m_reloadTimer.start();
}

Weapon::~Weapon()
{
}

bool Weapon::canShoot()
{
    isReloading();
    if (m_isReloading)   return false;
    if (m_currentAmmo <= 0) return false;

    float cd = (1.0f / fireRate) * 1000.0f;
     return m_shootTimer.elapsed() >= cd;
}
void Weapon::shoot()
{
    m_currentAmmo--;
    m_shootTimer.restart();

    if(m_currentAmmo ==0)
    {
        reload();
    }
}

void Weapon::reload()
{
    if (m_isReloading)        return;
    if (m_currentAmmo == m_maxAmmo) return;

    m_isReloading = true;
    m_reloadTimer.restart();
    m_shootTimer.restart();
}

bool Weapon::isReloading()
{
    if (m_isReloading)
    {
        if (m_reloadTimer.elapsed() >= m_reloadTime * 1000.0f)
        {
            m_isReloading = false;
            m_currentAmmo = m_maxAmmo;
            m_shootTimer.restart();
        }
    }
    return m_isReloading;
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

void Weapon::restartShootTimer()
{
    m_shootTimer.restart();
}

bool    Weapon::isEmpty()        { return m_currentAmmo <= 0; }
int     Weapon::getCurrentAmmo() { return m_currentAmmo;      }
int     Weapon::getMaxAmmo()     { return m_maxAmmo;          }
