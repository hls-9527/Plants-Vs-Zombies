#include "Bullet.h"

/**
 * @file Bullet.cpp
 * @brief 子弹基类实现
 */

 /**
  * 构造函数
  * @param x 初始X坐标
  * @param y 初始Y坐标
  * @param row 所在行
  * @param damage 伤害值
  * @param speed 移动速度
  */
Bullet::Bullet(int x, int y, int row, int damage, int speed)
    : GameObject(x, y)
    , row(row)
    , damage(damage)
    , speed(speed)
    , blast(false)
{
}

/**
 * 析构函数
 */
Bullet::~Bullet()
{
}

/**
 * 检查子弹是否超出屏幕
 * @return true 超出屏幕
 */
bool Bullet::isOutOfScreen() const
{
    // 窗口宽度900，子弹超出右侧屏幕即移除
    return x > 900;
}