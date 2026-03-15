#include "Sunflower.h"
#include "../../game/GameManager.h"
#include "../Sunshine.h"

/**
 * @file Sunflower.cpp
 * @brief 向日葵类实现
 */

 /**
  * 构造函数
  * @param row 行索引
  * @param col 列索引
  */
Sunflower::Sunflower(int row, int col)
    : Plant(row, col, 100)  // 向日葵生命值100
    , produceTimer(0)
{
    // 加载向日葵的待机动画帧
    // 图片路径：resources/images/pt/Sunflower/1.png, 2.png, 3.png...
    loadFrames("resources/images/pt/Sunflower", idleFrames);
}

/**
 * 析构函数
 */
Sunflower::~Sunflower()
{
}

/**
 * 更新向日葵状态
 * 每帧调用，处理生产阳光逻辑
 */
void Sunflower::update()
{
    if (!isAlive()) return;
    animTimer++;
    if (animTimer >= 10) {
        animTimer = 0;

        // 确保 idleFrames 不为空
        if (idleFrames.empty()) {
            return;
        }

        frameIndex++;
        if (frameIndex >= (int)idleFrames.size()) {
            frameIndex = 0;
        }
    }

    produceTimer++;
    if (produceTimer >= PRODUCE_INTERVAL) {
        produceTimer = 0;
        produceSunshine();
    }
}

/**
 * 绘制向日葵
 */
void Sunflower::draw()
{
    if (!isAlive()) return;

    // 使用基类的绘制方法
    Plant::draw();
}

/**
 * 生产阳光
 * 创建一个阳光对象并添加到游戏管理器的阳光列表
 */
void Sunflower::produceSunshine()
{
    // 获取游戏管理器单例
    GameManager* game = GameManager::getInstance();

    // 创建阳光对象（向日葵生产的阳光会直接出现在植物右上方）
    Sunshine* sunshine = new Sunshine(x, y, row);

    // 添加到游戏管理器的阳光列表
    // 注意：GameManager中已有 sunshines 容器，需要添加addSunshine方法
    game->addSunshine(sunshine);
}