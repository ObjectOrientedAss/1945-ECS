#ifndef SCENE_H
#define SCENE_H

#include "ecs.h"

struct MainMenuLevelData
{
    char highScoreToString[5];
};

struct BattleLevelData
{
    struct Entity* player;      //reference to the player entity
    struct Entity* boss;        //reference to the boss entity
    struct TextComponent* scoreUI;     //reference to the score UI text component
    struct TextComponent* powerupUI;     //reference to the score UI text component
    aiv_vector* lives;          //reference to the lives entity in the ui

    double islandSpawnTimer;
    double islandSpawnTimerChanger;

    double enemySpawnTimer;
    double enemySpawnTimerChanger;
    double enemiesShootCooldown;
    float enemyBulletsDamage;
    float enemySpeed;
    float enemyLife;

    float playerMaxLife;
    float playerSpeed;
    float playerBulletsDamage;
    double playerShootCooldown;
    int playerMaxLives;

    float environmentSpeed;
    float bulletSpeed;

    int enemyKilledScore;
    double increaseScoreTimer;
    int increaseScore;
    int score;

    double squadronSpawnTimer;
    int minEnemySquadronSize;
    int maxEnemySquadronSize;
    int enemyOndulationChance;

    float lifePowerupSpawnChance;
    double powerupSpawnTimer;
    double powerupSpawnTimerChanger;
    double powerupDuration;
    int speedPowerupBonus;
    int attackPowerupBonus;
    float playerSuicideDamage;

    double bossSpawnTimer;
    double bossTripleShootInterval;
    double bossDoubleShootInterval;
    double bossDoubleShootSwitch;
    double bossTripleShootSwitch;
    float bossHealth;
    float bossBulletsSpeed;
    float bossBulletsDamage;
    float bossSpeed;

    char scoreToString[5];
};

void LoadMainMenuScene(struct Game *game);
void LoadGameScene(struct Game *game);
void LoadSplashScreenScene(struct Game *game);

#endif //SCENE_H