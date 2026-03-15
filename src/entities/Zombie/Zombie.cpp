#include "Zombie.h"
#include "../Plant/Plant.h"
#include "../../game/GameManager.h"
#include "../../utils/EasyXHelper.h"
#include <graphics.h>

/**
 * @file Zombie.cpp
 * @brief 僵尸基类实现
 */

 /**
  * 构造函数
  * @param row 所在行
  * @param health 生命值
  * @param speed 移动速度
  * @param attackDamage 攻击力
  */
Zombie::Zombie(int row, int health, float speed, int attackDamage)
    : GameObject(850, 172 + (1 + row) * 100)  // 原C项目中的初始位置
    , exactX(850.0f)
    , currentState(STATE_WALKING)
    , row(row)
    , speed(speed)
    , health(health)
    , maxHealth(health)
    , attackDamage(attackDamage)
    , stateTimer(0)
    , targetPlant(nullptr)
    , animTimer(0)
    , animationFinished(false)
{
}

/**
 * 析构函数
 */
Zombie::~Zombie()
{
    // 释放行走动画帧
    for (auto& frame : walkFrames) {
        if (frame) delete frame;
    }
    walkFrames.clear();

    // 释放吃植物动画帧
    for (auto& frame : eatFrames) {
        if (frame) delete frame;
    }
    eatFrames.clear();

    // 释放死亡动画帧
    for (auto& frame : deadFrames) {
        if (frame) delete frame;
    }
    deadFrames.clear();
}

/**
 * 绘制僵尸
 */
void Zombie::draw()
{
    if (!isAlive() && currentState != STATE_DEAD) {
        return;
    }

    // 根据当前状态选择对应的动画帧列表
    std::vector<IMAGE*>* currentFrames = nullptr;

    switch (currentState)
    {
    case STATE_WALKING:
        currentFrames = &walkFrames;
        break;
    case STATE_EATING:
        currentFrames = &eatFrames;
        break;
    case STATE_DEAD:
        currentFrames = &deadFrames;
        break;
    }

    if (currentFrames && !currentFrames->empty()) {
        IMAGE* img = (*currentFrames)[frameIndex % currentFrames->size()];
        if (img) {
            // 原C项目中：y坐标需要减去图片高度，因为图片锚点在左上角
            EasyXHelper::putimagePNG(x, y - img->getheight(), img);
        }
    }
}

/**
 * 僵尸受到伤害
 * @param damage 伤害值
 */
void Zombie::takeDamage(int damage)
{
    if (currentState == STATE_DEAD) {
        return;
    }

    health -= damage;

    if (health <= 0) {
        printf("  僵尸 %p 生命归零，进入死亡状态\n", this);
        health = 0;
        setAlive(false);
        currentState = STATE_DEAD;
        frameIndex = 0;
        stateTimer = 0;
        speed = 0;  // 停止移动
        animTimer = 0;  // 重置动画计时器
        animationFinished = false;  // 动画还没完
    }
}

/**
 * 开始吃植物
 * @param plant 被吃的植物
 */
void Zombie::startEating(Plant* plant)
{
    if (plant && plant->isAlive()) {
        // 检查是否在同一行
        if (plant->getRow() != this->row) {
            return;
        }
        currentState = STATE_EATING;
        targetPlant = plant;
        frameIndex = 0;
        speed = 0;
        animTimer = 0;
    }
}

/**
 * 停止吃植物
 */
void Zombie::stopEating()
{
    currentState = STATE_WALKING;
    targetPlant = nullptr;
    frameIndex = 0;
    speed = 0.2f;  // 恢复移动速度（由派生类设置具体值）
}

/**
 * 加载行走动画帧
 * @param folderPath 图片文件夹路径
 */
void Zombie::loadWalkFrames(const char* folderPath)
{
    char filename[256];
    for (int i = 1; i <= 50; i++) {  // 最多尝试50张
        sprintf_s(filename, sizeof(filename), "%s/%d.png", folderPath, i);

        FILE* fp = fopen(filename, "r");
        if (fp == NULL) break;
        fclose(fp);

        IMAGE* img = new IMAGE();
        loadimage(img, filename);
        walkFrames.push_back(img);
    }
}

/**
 * 加载吃植物动画帧
 * @param folderPath 图片文件夹路径
 */
void Zombie::loadEatFrames(const char* folderPath)
{
    char filename[256];
    for (int i = 1; i <= 50; i++) {
        sprintf_s(filename, sizeof(filename), "%s/%d.png", folderPath, i);

        FILE* fp = fopen(filename, "r");
        if (fp == NULL) break;
        fclose(fp);

        IMAGE* img = new IMAGE();
        loadimage(img, filename);
        eatFrames.push_back(img);
    }
}

/**
 * 加载死亡动画帧
 * @param folderPath 图片文件夹路径
 */
void Zombie::loadDeadFrames(const char* folderPath)
{
    char filename[256];
    for (int i = 1; i <= 50; i++) {  // 明确20帧
        sprintf_s(filename, sizeof(filename), "%s/%d.png", folderPath, i);
        FILE* fp = fopen(filename, "r");
        if (fp == NULL) break;
        fclose(fp);
        IMAGE* img = new IMAGE();
        loadimage(img, filename);
        deadFrames.push_back(img);
    }
}