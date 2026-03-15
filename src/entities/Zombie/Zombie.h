#pragma once
#include "../GameObject.h"
#include <vector>

// 前向声明
class Plant;

/**
 * @file Zombie.h
 * @brief 僵尸基类
 *
 * 所有僵尸的基类，继承自GameObject
 * 定义僵尸的通用属性和行为
 *
 * 派生类：
 * - BasicZombie (普通僵尸)
 * - ConeheadZombie (路障僵尸)
 *
 * 原C项目中的僵尸系统完全保留：
 * - 移动、攻击、死亡动画
 * - 吃植物机制
 * - 血量系统
 */
class Zombie : public GameObject
{
protected:
    // === 僵尸状态枚举 ===
    enum ZombieState
    {
        STATE_WALKING,    ///< 行走状态
        STATE_EATING,     ///< 吃植物状态
        STATE_DEAD        ///< 死亡状态
    };

    ZombieState currentState;  ///< 当前状态
    int row;                    ///< 所在行
    float speed;                  ///< 移动速度（像素/帧）
    int health;                 ///< 当前生命值
    int maxHealth;              ///< 最大生命值
    int attackDamage;           ///< 攻击力（对植物的伤害）
    float exactX;  ///< 精确的X坐标（浮点）

    // === 动画帧 ===
    std::vector<IMAGE*> walkFrames;   ///< 行走动画帧
    std::vector<IMAGE*> eatFrames;    ///< 吃植物动画帧
    std::vector<IMAGE*> deadFrames;   ///< 死亡动画帧

    int stateTimer;                   ///< 状态计时器
    Plant* targetPlant;               ///< 正在吃的植物
    int animTimer;  ///< 动画计时器（用于控制动画速度）

public:

    bool animationFinished;
    /**
     * @brief 构造函数
     * @param row 所在行
     * @param health 生命值
     * @param speed 移动速度
     * @param attackDamage 攻击力
     */
    Zombie(int row, int health, float speed, int attackDamage);

    /**
     * @brief 虚析构函数
     */
    virtual ~Zombie();

    /**
     * @brief 更新僵尸状态（纯虚函数）
     * 每帧调用，由派生类实现具体行为
     */
    virtual void update() override = 0;

    /**
     * @brief 绘制僵尸
     */
    virtual void draw() override;

    // === 属性访问 ===
    int getRow() const { return row; }
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
    float getSpeed() const { return speed; }

    /**
     * @brief 僵尸受到伤害
     * @param damage 伤害值
     */
    void takeDamage(int damage);

    /**
     * @brief 判断僵尸是否死亡
     * @return true 已死亡
     */
    bool isDead() const { return health <= 0; }

    /**
     * @brief 开始吃植物
     * @param plant 被吃的植物
     */
    void startEating(Plant* plant);

    /**
     * @brief 停止吃植物
     */
    void stopEating();

    /**
     * @brief 检查是否正在吃植物
     * @return true 正在吃
     */
    bool isEating() const { return currentState == STATE_EATING; }

    /**
     * @brief 检查僵尸是否到达房子（游戏失败条件）
     * @return true 已到达房子
     */
    bool isAtHouse() const { return x < 56; }  // 原C项目中的房子位置

protected:
    /**
     * @brief 加载行走动画帧
     * @param folderPath 图片文件夹路径
     */
    void loadWalkFrames(const char* folderPath);

    /**
     * @brief 加载吃植物动画帧
     * @param folderPath 图片文件夹路径
     */
    void loadEatFrames(const char* folderPath);

    /**
     * @brief 加载死亡动画帧
     * @param folderPath 图片文件夹路径
     */
    void loadDeadFrames(const char* folderPath);
};