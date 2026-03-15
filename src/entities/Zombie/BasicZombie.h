#pragma once
#include "Zombie.h"

/**
 * @file BasicZombie.h
 * @brief 普通僵尸类
 *
 * 继承自Zombie，实现普通僵尸的特有行为：
 * - 行走动画（22帧）
 * - 吃植物动画（21帧）
 * - 死亡动画（20帧）
 * - 血量100，攻击力20，移动速度1
 *
 * 原C项目中的普通僵尸完全保留
 */
class BasicZombie : public Zombie
{
public:
    /**
     * @brief 构造函数
     * @param row 所在行
     */
    BasicZombie(int row, float speed = 0.1f);

    /**
     * @brief 析构函数
     */
    virtual ~BasicZombie();

    /**
     * @brief 更新普通僵尸状态
     * 每帧调用，处理移动、攻击、死亡逻辑
     */
    virtual void update() override;

private:
    /**
     * @brief 加载所有动画帧
     */
    void loadAllFrames();
};