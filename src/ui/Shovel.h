#pragma once
#include "../entities/GameObject.h"
#include <graphics.h>

class Plant;

/**
 * @file Shovel.h
 * @brief 铲子类
 *
 * 继承自GameObject，实现铲子的完整功能：
 * 1. 点击铲子卡片后激活，图标跟随鼠标
 * 2. 左键点击植物 → 删除植物，铲子归位，播放音效
 * 3. 左键点击空地 → 铲子归位，取消状态
 * 4. 右键点击 → 铲子归位，取消状态
 *
 * 铲子只有一张图片，不需要动画帧
 */
class Shovel : public GameObject
{
private:
    bool active;           ///< 铲子是否处于激活状态（跟随鼠标）
    int mouseX, mouseY;    ///< 当前鼠标位置（用于绘制）
    IMAGE icon;            ///< 铲子图标图片

public:
    /**
     * @brief 构造函数
     * 加载铲子图片，初始状态为未激活
     */
    Shovel();

    /**
     * @brief 析构函数
     */
    virtual ~Shovel();

    /**
     * @brief 更新铲子状态
     * 铲子本身不需要每帧更新逻辑，但为了继承GameObject需要实现
     */
    virtual void update() override;

    /**
     * @brief 绘制铲子
     * 只有在激活状态时才绘制，且绘制在鼠标位置
     */
    virtual void draw() override;

    /**
     * @brief 激活铲子（点击铲子卡片时调用）
     */
    void activate();

    /**
     * @brief 取消激活（铲子归位）
     */
    void deactivate();

    /**
     * @brief 更新鼠标位置（在鼠标移动事件中调用）
     * @param x 鼠标X坐标
     * @param y 鼠标Y坐标
     */
    void updateMousePos(int x, int y);

    /**
     * @brief 判断铲子是否处于激活状态
     * @return true 激活状态，false 未激活
     */
    bool isActive() const { return active; }

    /**
     * @brief 处理点击事件
     * @param mouseX 鼠标点击X坐标
     * @param mouseY 鼠标点击Y坐标
     * @param row 点击的行（如果是网格内）
     * @param col 点击的列（如果是网格内）
     * @param plant 点击位置的植物指针（可能为nullptr）
     * @return true 铲子处理了点击（不再继续传递）
     */
    bool handleClick(int mouseX, int mouseY, int row, int col, Plant* plant);
};