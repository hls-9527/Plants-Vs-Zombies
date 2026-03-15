#pragma once
#include "../GameObject.h"

/**
 * @file Bullet.h
 * @brief 子弹基类
 *
 * 所有子弹的基类，继承自GameObject
 * 定义子弹的通用属性和行为
 *
 * 派生类：
 * - PeaBullet (豌豆子弹)
 */
class Bullet : public GameObject
{
protected:
    int row;            ///< 所在行（用于碰撞检测）
    int damage;         ///< 伤害值
    int speed;          ///< 移动速度（像素/帧）
    bool blast;       ///< 是否爆炸（用于子弹消失效果）

public:
    /**
     * @brief 构造函数
     * @param x 初始X坐标
     * @param y 初始Y坐标
     * @param row 所在行
     * @param damage 伤害值
     * @param speed 移动速度
     */
    Bullet(int x, int y, int row, int damage, int speed);

    /**
     * @brief 虚析构函数
     */
    virtual ~Bullet();

    /**
     * @brief 更新子弹状态（纯虚函数）
     * 每帧调用，处理移动和碰撞检测
     */
    virtual void update() override = 0;

    /**
     * @brief 绘制子弹（纯虚函数）
     */
    virtual void draw() override = 0;

    /**
     * @brief 获取子弹所在行
     * @return 行索引
     */
    int getRow() const { return row; }

    /**
     * @brief 获取伤害值
     * @return 伤害值
     */
    int getDamage() const { return damage; }

    /**
     * @brief 判断子弹是否爆炸
     * @return true 已爆炸
     */
    bool isBlast() const { return blast; }

    /**
     * @brief 设置爆炸状态
     * @param blast 爆炸状态
     */
    void setBlast(bool b) { blast = b; }

    /**
     * @brief 子弹击中目标时的回调
     * @param target 被击中的对象（由派生类具体实现）
     */
    virtual void onHit() = 0;

    /**
     * @brief 检查子弹是否超出屏幕
     * @return true 超出屏幕
     */
    bool isOutOfScreen() const;
};