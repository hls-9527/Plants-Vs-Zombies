#pragma once
#include <graphics.h>
#include "../utils/EasyXHelper.h"  

/**
 * @file GameObject.h
 * @brief 游戏对象抽象基类
 *
 * 所有游戏实体的基类（植物、僵尸、子弹、阳光、铲子等）
 * 采用纯虚函数实现多态，是C++面向对象设计的核心
 *
 * 继承体系：
 * GameObject
 * ├── Plant
 * │   ├── Peashooter
 * │   ├── Sunflower
 * │   └── Chomper
 * ├── Zombie
 * │   ├── BasicZombie
 * │   └── ConeheadZombie
 * ├── Bullet
 * │   └── PeaBullet
 * ├── Sunshine
 * └── Shovel（工具类也继承，方便统一管理）
 */
class GameObject
{
protected:
    // === 所有游戏对象共有的属性 ===
    int x;               ///< 对象的屏幕X坐标（像素）
    int y;               ///< 对象的屏幕Y坐标（像素）
    int frameIndex;      ///< 当前动画帧索引（用于帧动画）
    bool aAlive;        ///< 存活状态：true存活，false已死亡（待移除）

public:
    /**
     * @brief 构造函数
     * @param startX 初始X坐标（默认为0）
     * @param startY 初始Y坐标（默认为0）
     */
    GameObject(int startX = 0, int startY = 0);

    /**
     * @brief 虚析构函数
     *
     * 当通过基类指针删除派生类对象时，
     * 必须使用虚析构函数才能正确调用派生类的析构函数
     */
    virtual ~GameObject();

    // === 纯虚函数（接口定义）===
    // 所有派生类必须实现这两个方法

    /**
     * @brief 更新对象状态（纯虚函数）
     *
     * 每帧调用一次，处理：
     * - 位置移动
     * - 计时器更新
     * - 状态变化
     * - 攻击/生产逻辑
     */
    virtual void update() = 0;

    /**
     * @brief 绘制对象（纯虚函数）
     *
     * 每帧调用一次，使用EasyXHelper::putimagePNG
     * 绘制当前帧索引对应的图片
     */
    virtual void draw() = 0;

    // === 公共方法（所有派生类共享）===

    /**
     * @brief 判断对象是否存活
     * @return true存活，false死亡
     */
    bool isAlive() const;

    /**
     * @brief 设置存活状态
     * @param alive true存活/false死亡
     */
    void setAlive(bool alive);

    /**
     * @brief 获取X坐标
     */
    int getX() const;

    /**
     * @brief 获取Y坐标
     */
    int getY() const;

    /**
     * @brief 设置对象位置
     * @param newX 新的X坐标
     * @param newY 新的Y坐标
     */
    void setPosition(int newX, int newY);

    /**
     * @brief 获取当前帧索引
     */
    int getFrameIndex() const;

    /**
     * @brief 设置帧索引
     * @param index 新的帧索引
     */
    void setFrameIndex(int index);

    /**
     * @brief 增加帧索引（用于动画循环）
     * @param delta 增加量（默认为1）
     */
    void addFrameIndex(int delta = 1);
};