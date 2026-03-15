#pragma once
#include "Plant.h"

/**
 * @file Chomper.h
 * @brief 大嘴花类
 *
 * 继承自Plant，实现大嘴花的特有行为：
 * - 检测前方近距离的僵尸
 * - 攻击时播放攻击动画，僵尸消失
 * - 进入咀嚼状态，播放咀嚼动画
 * - 咀嚼一段时间后恢复待机状态
 */
class Chomper : public Plant
{
private:
    // === 状态枚举 ===
    enum ChomperState
    {
        STATE_IDLE,      ///< 待机状态
        STATE_ATTACK,    ///< 攻击状态
        STATE_CHEW       ///< 咀嚼状态
    };

    ChomperState currentState;  ///< 当前状态
    int stateTimer;             ///< 状态计时器

    std::vector<IMAGE*> scaledAttackFrames;  ///< 缩放后的攻击动画帧

    // === 动画帧 ===
    std::vector<IMAGE*> attackFrames;  ///< 攻击动画帧
    std::vector<IMAGE*> chewFrames;    ///< 咀嚼动画帧

    static const int ATTACK_DURATION = 10;   ///< 攻击动画持续时间（帧数）
    static const int CHEW_DURATION = 3000;    ///< 咀嚼持续时间（帧数）
    static const int ATTACK_RANGE = 40;       ///< 攻击范围（像素）

public:
    /**
     * @brief 构造函数
     * @param row 行索引
     * @param col 列索引
     */
    Chomper(int row, int col);

    /**
     * @brief 析构函数
     */
    virtual ~Chomper();

    /**
     * @brief 更新大嘴花状态
     * 每帧调用，处理攻击和状态切换逻辑
     */
    virtual void update() override;

    /**
     * @brief 绘制大嘴花
     */
    virtual void draw() override;

    /**
     * @brief 检查前方是否有僵尸在攻击范围内
     * @return true 有僵尸可以攻击
     */
    bool hasZombieInRange() const;

    /**
     * @brief 攻击前方僵尸
     * 播放攻击动画，僵尸消失
     */
    void attack();

private:
    /**
     * @brief 加载所有动画帧
     */
    void loadAllFrames();
};