#pragma once
#include "Plant.h"

/**
 * @file Peashooter.h
 * @brief 豌豆射手类
 *
 * 继承自Plant，实现豌豆射手的特有行为：
 * - 向前方发射豌豆子弹
 * - 有射击间隔
 * - 检测前方是否有僵尸
 */
class Peashooter : public Plant
{
private:
    int shootTimer;           ///< 射击计时器
    static const int SHOOT_INTERVAL = 200;  ///< 射击间隔（帧数）

public:
    /**
     * @brief 构造函数
     * @param row 行索引
     * @param col 列索引
     */
    Peashooter(int row, int col);

    /**
     * @brief 析构函数
     */
    virtual ~Peashooter();

    /**
     * @brief 更新豌豆射手状态
     * 每帧调用，处理射击逻辑
     */
    virtual void update() override;

    /**
     * @brief 绘制豌豆射手
     */
    virtual void draw() override;

    /**
     * @brief 发射子弹
     * 创建一颗豌豆子弹并添加到游戏管理器的子弹列表
     */
    void shoot();

private:
    /**
     * @brief 检查前方是否有僵尸
     * @return true 前方有僵尸，可以射击
     */
    bool hasZombieInFront() const;
};