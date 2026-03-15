#include "BasicZombie.h"
#include "../../game/GameManager.h"
#include "../Plant/Plant.h"

/**
 * @file BasicZombie.cpp
 * @brief 普通僵尸类实现
 */

 /**
  * 构造函数
  * @param row 所在行
  */
BasicZombie::BasicZombie(int row, float speed)
    : Zombie(row, 100, speed, 10)
{
    loadAllFrames();
}

/**
 * 析构函数
 */
BasicZombie::~BasicZombie()
{
}

/**
 * 加载所有动画帧
 */
void BasicZombie::loadAllFrames()
{
    // 加载行走动画帧（zb_walk/1.png ~ 22.png）
    loadWalkFrames("resources/images/zb/zb_walk");

    // 加载吃植物动画帧（zb_eat/1.png ~ 21.png）
    loadEatFrames("resources/images/zb/zb_eat");

    // 加载死亡动画帧（zb_dead/1.png ~ 20.png）
    loadDeadFrames("resources/images/zb/zb_dead");
}

/**
 * 更新普通僵尸状态
 */
void BasicZombie::update()
{
    //if (!isAlive()) return;

    animTimer++;

    GameManager* game = nullptr;
    Plant* plantInFront = nullptr;

    switch (currentState)
    {
    case STATE_WALKING:
        // 动画控制
        if (animTimer >= 10) {
            animTimer = 0;
            frameIndex++;
            if (frameIndex >= (int)walkFrames.size()) {
                frameIndex = 0;
            }
        }

        // 移动逻辑
        exactX -= speed;
        x = (int)exactX;
        // 碰撞检测（由GameManager统一管理）
        /*
        game = GameManager::getInstance();
        plantInFront = nullptr;

        for (int col = 0; col < 9; col++) {
            Plant* plant = game->getPlantAt(row, col);
            if (plant && plant->isAlive()) {
                int distance = x - plant->getX();
                // 只有当僵尸非常接近植物时才吃（例如距离小于10像素）
                if (distance > 0 && distance < 10) {
                    plantInFront = plant;
                    break;
                }
            }
        }

        if (plantInFront) {
            startEating(plantInFront);
        }
        */
        break;

    case STATE_EATING:
        if (animTimer >= 10) {
            animTimer = 0;
            frameIndex++;
            if (frameIndex >= (int)eatFrames.size()) {
                frameIndex = 0;
            }
        }

        stateTimer++;
        if (stateTimer >= 30) {
            stateTimer = 0;

            // 安全检查
            if (targetPlant == nullptr) {
                stopEating();
                break;
            }

            // 检查植物是否还在网格中（通过 GameManager 重新获取）
            GameManager* game = GameManager::getInstance();
            Plant* currentPlant = game->getPlantAt(targetPlant->getRow(), targetPlant->getCol());

            if (currentPlant == targetPlant && targetPlant->isAlive()) {
                targetPlant->takeDamage(attackDamage);
            }
            else {
                stopEating();
            }
        }
        break;

    case STATE_DEAD:
        animTimer++;

        // 使用 animTimer 控制动画速度（每10帧更新一次）
        if (animTimer >= 10) {
            animTimer = 0;
            frameIndex++;
        }

        if (frameIndex >= (int)deadFrames.size()) {
            animationFinished = true;  // 动画播放完
        }

        // 保险：如果已经在最后一帧停留太久（超过100帧），强制移除
        static int lastFrameCounter = 0;
        if (frameIndex >= (int)deadFrames.size() - 1) {
            lastFrameCounter++;
            if (lastFrameCounter > 100) {
                printf("    zombie %p: 最后一帧停留太久，强制移除\n", this);
                setAlive(false);
            }
        }
        else {
            lastFrameCounter = 0;
        }
        break;
    }
}