/*
Author: Donavan Sirois
Date: February 1, 2026
File name: Weapon.cpp
Goal: Code file for the weapon class.
Modifications:
*/

#include "weapon.h"

Weapon::Weapon(int dmg, float rng, float rate, int maxAmmo, float reloadTime)
{
    // Base weapon stats
    damage         = dmg;        // Damage per shot
    range          = rng;        // Maximum range
    fireRate       = rate;       // Shots per second
    m_baseFireRate = rate;       // Stored base fire rate (used for power-up reset)

    // Ammo management
    m_maxAmmo      = maxAmmo;    // Maximum ammo capacity
    m_baseMaxAmmo  = maxAmmo;    // Base ammo (used for power-up reset)
    m_currentAmmo  = maxAmmo;    // Start fully loaded

    // Reload timing
    m_reloadTime   = reloadTime;

    // Initialize timers
    m_shootTimer.start();
    m_reloadTimer.start();
    m_powerUpCdTimer.start();
}

Weapon::~Weapon()
{
}

// --------------------------------------------------
// Shooting logic
// --------------------------------------------------

// Check if weapon can shoot (ammo + cooldown + not reloading)
bool Weapon::canShoot()
{
    isReloading(); // Update reload state

    if (m_isReloading)     return false;
    if (m_currentAmmo <= 0) return false;

    float cd = (1000.0f / fireRate); // Cooldown in ms
    return m_shootTimer.elapsed() >= cd;
}

// Check only cooldown (used when ammo is not relevant)
bool Weapon::cooldownReady()
{
    isReloading();

    if (m_isReloading) return false;

    float cd = (1000.0f / fireRate);
    return m_shootTimer.elapsed() >= cd;
}

// Perform a shot
void Weapon::shoot()
{
    m_currentAmmo--;          // Consume one bullet
    updateBalles();           // Notify UI
    m_shootTimer.restart();   // Reset cooldown
}

// --------------------------------------------------
// Reload logic
// --------------------------------------------------

void Weapon::reload()
{
    // Ignore if already reloading or full ammo
    if (m_isReloading) return;
    if (m_currentAmmo == m_maxAmmo) return;

    m_isReloading = true;
    m_reloadTimer.restart();
    m_shootTimer.restart(); // Prevent shooting during reload

    isReloading(); // Force update
}

// Update reload state and finish reload if time passed
bool Weapon::isReloading()
{
    if (m_isReloading)
    {
        if (m_reloadTimer.elapsed() >= m_reloadTime * 1000.0f)
        {
            m_isReloading = false;
            m_currentAmmo = m_maxAmmo; // Refill ammo
            updateBalles();
            m_shootTimer.restart();
        }
    }
    return m_isReloading;
}

// --------------------------------------------------
// Getters
// --------------------------------------------------

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

// Restart shoot cooldown manually
void Weapon::restartShootTimer()
{
    m_shootTimer.restart();
}

// --------------------------------------------------
// Power-up system
// --------------------------------------------------

// Activate power-up (boost fire rate and ammo)
void Weapon::powerUp()
{
    // Prevent reactivation if already active or still in cooldown
    if (m_isPoweredUp) return;
    if (m_powerUpCdTimer.elapsed() < powerUpCd) return;

    qDebug("POWER UP activated");

    m_isPoweredUp = true;

    // Boost stats
    m_maxAmmo     = m_baseMaxAmmo  * 4;
    fireRate      = m_baseFireRate * 2;

    // Refill ammo to new max
    m_currentAmmo = m_maxAmmo;

    m_powerUpTimer.restart();
}

// Update power-up state (called every frame)
void Weapon::updatePowerUp()
{
    if (!m_isPoweredUp) return;

    // If duration expired → revert to base stats
    if (m_powerUpTimer.elapsed() >= powerUpDuration)
    {
        qDebug("POWER UP ended — cooldown started");

        m_isPoweredUp = false;

        // Restore base stats
        m_maxAmmo = m_baseMaxAmmo;
        fireRate  = m_baseFireRate;

        // Clamp ammo if needed
        if (m_currentAmmo > m_maxAmmo)
            m_currentAmmo = m_maxAmmo;

        // Start cooldown timer
        m_powerUpCdTimer.restart();
    }
}

// Check if power-up is active
bool Weapon::isPoweredUp()
{
    return m_isPoweredUp;
}

// --------------------------------------------------
// Ammo helpers
// --------------------------------------------------

bool Weapon::isEmpty()
{
    return m_currentAmmo <= 0;
}

int Weapon::getCurrentAmmo()
{
    return m_currentAmmo;
}

int Weapon::getMaxAmmo()
{
    return m_maxAmmo;
}

// Notify UI about ammo changes
void Weapon::updateBalles()
{
    int balles = getCurrentAmmo();
    emit sigUpdateBalles(balles);
}

// Reset ammo (used when restarting game)
void Weapon::resetGameAmmo()
{
    m_currentAmmo = m_maxAmmo;
    updateBalles();
}

// Force set ammo (used by Arduino input)
void Weapon::setCurrentAmmo(int value)
{
    m_currentAmmo = value;
}

// --------------------------------------------------
// Power-up progress
// --------------------------------------------------

float Weapon::getPowerUpProgress() const
{
    if (m_isPoweredUp)
    {

        float elapsed = m_powerUpTimer.elapsed();
        return 1.0f - (elapsed / powerUpDuration);
    }
    else
    {

        float elapsed = m_powerUpCdTimer.elapsed();
        return std::min(1.0f, elapsed / powerUpCd);
    }
}
