#include "GameObject.h"

/**
 * @file GameObject.cpp
 * @brief 游戏对象基类的实现
 */

 /**
  * @brief 构造函数
  * @param startX 初始X坐标
  * @param startY 初始Y坐标
  *
  * 注意：使用 this-> 明确指定成员变量，避免参数同名冲突
  */
GameObject::GameObject(int startX, int startY)
{
    this->x = startX;           // 使用 this-> 明确是成员变量
    this->y = startY;
    this->frameIndex = 0;
    this->aAlive = true;
}

/**
 * @brief 虚析构函数
 *
 * 基类析构函数，由派生类负责具体资源释放
 */
GameObject::~GameObject()
{
    // 基类无需释放资源，但需要虚析构保证多态删除正确
}

/**
 * @brief 判断对象是否存活
 * @return true存活，false死亡
 */
bool GameObject::isAlive() const
{
    return aAlive;  // 这里没问题，因为没有参数同名
}

/**
 * @brief 设置存活状态
 * @param alive true存活/false死亡
 */
void GameObject::setAlive(bool alive)
{
    this->aAlive = alive;  // 加上 this->
}

/**
 * @brief 获取X坐标
 * @return X坐标值
 */
int GameObject::getX() const
{
    return x;
}

/**
 * @brief 获取Y坐标
 * @return Y坐标值
 */
int GameObject::getY() const
{
    return y;
}

/**
 * @brief 设置对象位置
 * @param newX 新的X坐标
 * @param newY 新的Y坐标
 */
void GameObject::setPosition(int newX, int newY)
{
    this->x = newX;  // 加上 this->
    this->y = newY;
}

/**
 * @brief 获取当前帧索引
 * @return 帧索引值
 */
int GameObject::getFrameIndex() const
{
    return frameIndex;
}

/**
 * @brief 设置帧索引
 * @param index 新的帧索引
 */
void GameObject::setFrameIndex(int index)
{
    this->frameIndex = index;  // 加上 this->
}

/**
 * @brief 增加帧索引（用于动画循环）
 * @param delta 增加量（默认为1）
 */
void GameObject::addFrameIndex(int delta)
{
    this->frameIndex += delta;  // 加上 this->
}