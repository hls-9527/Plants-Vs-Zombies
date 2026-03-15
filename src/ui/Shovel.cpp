#include "Shovel.h"
#include "../game/GameManager.h"
#include "../utils/AudioManager.h"
#include "../utils/EasyXHelper.h"
#include "../entities/Plant/Plant.h"
#include <graphics.h>

/**
 * @file Shovel.cpp
 * @brief 铲子类实现
 */

/**
 * 构造函数
 */
Shovel::Shovel()
    : GameObject(0, 0)
    , active(false)
    , mouseX(0)
    , mouseY(0)
{
    // 加载时直接缩放到卡片大小
    loadimage(&icon,
        "resources/images/Cards/card_Shovel.png",
        85,   // 目标宽度
        90,   // 目标高度
        true);
}

/**
 * 析构函数
 */
Shovel::~Shovel()
{
}

/**
 * 更新铲子状态
 */
void Shovel::update()
{
}

/**
 * 绘制铲子
 */
void Shovel::draw()
{
    if (!active) return;

    int width = icon.getwidth();
    int height = icon.getheight();

    EasyXHelper::putimagePNG(
        mouseX - width / 2 + 30,
        mouseY - height / 2 - 30,
        &icon
    );
}

/**
 * 激活铲子
 */
void Shovel::activate()
{
    if (!active) {
        active = true;
        AudioManager::playShovelSound();
    }
}

/**
 * 取消激活
 */
void Shovel::deactivate()
{
    active = false;
}

/**
 * 更新鼠标位置
 */
void Shovel::updateMousePos(int x, int y)
{
    mouseX = x;
    mouseY = y;
}

/**
 * 处理点击事件
 */
bool Shovel::handleClick(int mouseX, int mouseY, int row, int col, Plant* plant)
{
    if (!active) return false;

    if (plant != nullptr) {
        plant->setAlive(false);
        AudioManager::playShovelDeleteSound();  // 播放删除植物音效
        deactivate();
        return true;
    }
    else {
        deactivate();
        return false;
    }
}