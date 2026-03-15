#include "Chomper.h"
#include "../../game/GameManager.h"
#include "../Zombie/Zombie.h"

/**
 * @file Chomper.cpp
 * @brief 大嘴花类实现
 */

 /**
  * 构造函数
  * @param row 行索引
  * @param col 列索引
  */
Chomper::Chomper(int row, int col)
    : Plant(row, col, 200)  // 大嘴花生命值200
    , currentState(STATE_IDLE)
    , stateTimer(0)
{
    // 加载所有动画帧
    loadAllFrames();

    // 调整Y坐标向上移动10像素
    y -= 20;
}

/**
 * 析构函数
 */
Chomper::~Chomper()
{
    // 释放攻击动画帧
    for (auto& frame : attackFrames) {
        if (frame) delete frame;
    }
    attackFrames.clear();

    // 释放咀嚼动画帧
    for (auto& frame : chewFrames) {
        if (frame) delete frame;
    }
    chewFrames.clear();

    for (auto& frame : scaledAttackFrames) {
        if (frame) delete frame;
    }
    scaledAttackFrames.clear();
}

/**
 * 加载所有动画帧
 */
void Chomper::loadAllFrames()
{
    int targetWidth = 101;
    int targetHeight = 95;
    float attackScale = 0.5f;

    // 加载待机和咀嚼动画
    loadFrames("resources/images/pt/Chomper/Chomper_stand", idleFrames, targetWidth, targetHeight);
    loadFrames("resources/images/pt/Chomper/Chomper_chew", chewFrames, targetWidth-20, targetHeight-5);

    // 加载攻击动画原图（不用 loadFrames）
    char filename[256];
    for (int i = 1; i <= 50; i++) {
        sprintf_s(filename, sizeof(filename), "resources/images/pt/Chomper/Chomper_eat/%d.png", i);

        FILE* fp = fopen(filename, "r");
        if (fp == NULL) break;
        fclose(fp);

        IMAGE* img = new IMAGE();
        loadimage(img, filename);
        attackFrames.push_back(img);
    }

    // 缩放攻击动画
    for (auto& frame : attackFrames) {
        if (frame) {
            int newWidth = (int)(frame->getwidth() * attackScale);
            int newHeight = (int)(frame->getheight() * attackScale);

            IMAGE* scaledImg = new IMAGE(newWidth, newHeight);

            DWORD* src = GetImageBuffer(frame);
            DWORD* dst = GetImageBuffer(scaledImg);

            float scaleX = (float)frame->getwidth() / newWidth;
            float scaleY = (float)frame->getheight() / newHeight;

            for (int y = 0; y < newHeight; y++) {
                for (int x = 0; x < newWidth; x++) {
                    int srcX = (int)(x * scaleX);
                    int srcY = (int)(y * scaleY);
                    dst[y * newWidth + x] = src[srcY * frame->getwidth() + srcX];
                }
            }

            scaledAttackFrames.push_back(scaledImg);
        }
    }
}

/**
 * 更新大嘴花状态
 */
void Chomper::update()
{
    if (!isAlive()) return;
    animTimer++;

    // 状态机
    switch (currentState)
    {
    case STATE_IDLE:
        if (animTimer >= 10) {  // 每10帧更新一次动画
            animTimer = 0;
            frameIndex++;
            if (frameIndex >= (int)idleFrames.size()) {
                frameIndex = 0;
            }
        }

        // 检查前方是否有僵尸
        if (hasZombieInRange()) {
            currentState = STATE_ATTACK;
            stateTimer = 0;
            frameIndex = 0;
        }
        break;

    case STATE_ATTACK:
        // 每20帧更新一次动画帧
        if (animTimer >= 5) {
            animTimer = 0;
            frameIndex++;
        }

        // 当播放完最后一帧时触发攻击
        if (frameIndex >= (int)scaledAttackFrames.size()) {
            attack();
            currentState = STATE_CHEW;
            stateTimer = 0;
            frameIndex = 0;
        }
        break;

    case STATE_CHEW:
        // 咀嚼状态：播放咀嚼动画
        if (animTimer >= 15) {
            animTimer = 0;
            frameIndex++;
            if (frameIndex >= (int)chewFrames.size()) {
                frameIndex = 0;
            }
        }

        stateTimer++;
        // 咀嚼一段时间后恢复待机状态
        if (stateTimer >= CHEW_DURATION) {
            currentState = STATE_IDLE;
            stateTimer = 0;
            frameIndex = 0;
        }
        break;
    }
}

/**
 * 绘制大嘴花
 */
void Chomper::draw()
{
    if (!isAlive()) return;

    // 根据当前状态选择对应的动画帧列表
    std::vector<IMAGE*>* currentFrames = nullptr;
    int offsetY = 0;  // Y轴偏移量

    switch (currentState)
    {
    case STATE_IDLE:
        currentFrames = &idleFrames;
        break;
    case STATE_ATTACK:
        currentFrames = &scaledAttackFrames;
        offsetY = -20;  // 攻击动画向上偏移20像素
        break;
    case STATE_CHEW:
        currentFrames = &chewFrames;
        break;
    }

    if (currentFrames && !currentFrames->empty()) {
        IMAGE* img = (*currentFrames)[frameIndex % currentFrames->size()];
        if (img) {
            EasyXHelper::putimagePNG(x, y + offsetY, img);  // Y坐标加上偏移量
        }
    }
}

/**
 * 检查前方是否有僵尸在攻击范围内
 */
bool Chomper::hasZombieInRange() const
{
    GameManager* game = GameManager::getInstance();

    // 获取当前行的所有僵尸
    const std::vector<Zombie*>& zombies = game->getZombies();

    for (auto& zombie : zombies) {
        if (!zombie || !zombie->isAlive()) continue;

        // 检查是否在同一行
        if (zombie->getRow() != row) continue;

        // 检查僵尸是否在攻击范围内
        int zombieX = zombie->getX();
        if (zombieX >= x - ATTACK_RANGE && zombieX <= x + ATTACK_RANGE) {
            return true;
        }
    }

    return false;
}

/**
 * 攻击前方僵尸
 */
void Chomper::attack()
{
    GameManager* game = GameManager::getInstance();

    // 获取当前行的所有僵尸
    const std::vector<Zombie*>& zombies = game->getZombies();

    for (auto& zombie : zombies) {
        if (!zombie || !zombie->isAlive()) continue;

        // 检查是否在同一行
        if (zombie->getRow() != row) continue;

        // 检查僵尸是否在攻击范围内
        int zombieX = zombie->getX();
        if (zombieX >= x - ATTACK_RANGE && zombieX <= x + ATTACK_RANGE) {
            // 僵尸直接死亡
            zombie->setAlive(false);
            break;
        }
    }
}