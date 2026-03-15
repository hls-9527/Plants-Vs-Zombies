#include "Plant.h"
#include <easyx.h>
#include <graphics.h>
#include "../../utils/EasyXHelper.h"

/**
 * @file Plant.cpp
 * @brief 植物基类实现
 */

 /**
  * 构造函数
  * @param row 行索引
  * @param col 列索引
  * @param health 初始生命值
  */
Plant::Plant(int row, int col, int health)
    : GameObject()
    , row(row)
    , col(col)
    , health(health)
    , maxHealth(health)
    , timer(0)
    , animTimer(0)  // 添加这一行
    , frameCount(0)
{
    // 根据行列计算屏幕坐标（完全保留原C项目的计算公式）
    updateScreenPosition();
}

/**
 * 析构函数
 * 释放所有动画帧图片
 */
Plant::~Plant()
{
    // 释放待机动画帧
    for (auto& frame : idleFrames) {
        if (frame) {
            delete frame;
            frame = nullptr;
        }
    }
    idleFrames.clear();

    // 释放攻击动画帧
    for (auto& frame : attackFrames) {
        if (frame) {
            delete frame;
            frame = nullptr;
        }
    }
    attackFrames.clear();
}

/**
 * 绘制植物
 * 使用当前帧索引绘制对应的动画帧
 */
void Plant::draw()
{
    if (!isAlive()) return;

    // 根据当前状态选择合适的帧列表
    std::vector<IMAGE*>* currentFrames = &idleFrames;

    // 如果没有动画帧，直接返回
    if (currentFrames->empty()) {
        printf("错误：植物动画帧为空！\n");  // 添加调试
        return;
    }

    // 确保 frameIndex 在有效范围内
    if (frameIndex < 0 || frameIndex >= (int)currentFrames->size()) {
        frameIndex = 0;  // 重置为0
    }

    // 获取当前帧图片
    IMAGE* img = (*currentFrames)[frameIndex];
    if (img) {
        EasyXHelper::putimagePNG(x, y, img);
    }
}

/**
 * 植物受到伤害
 * @param damage 伤害值
 */
void Plant::takeDamage(int damage)
{
    health -= damage;
    if (health <= 0) {
        printf("    plant %p: 生命为0，死亡\n", this);
        health = 0;
        setAlive(false);
    }
}

/**
 * 加载动画帧（用于所有植物，图片命名为 1.png, 2.png, 3.png...）
 * @param folderPath 图片文件夹路径（如 "resources/images/pt/Peashooter"）
 * @param frames 目标容器
 * @param maxCount 最大尝试次数（防止无限循环）
 */
void Plant::loadFrames(const char* folderPath, std::vector<IMAGE*>& frames, int maxCount)
{
    // 清空原有帧
    for (auto& frame : frames) {
        if (frame) delete frame;
    }
    frames.clear();

    // 从1开始尝试加载，直到文件不存在
    for (int i = 1; i <= maxCount; i++) {
        char filename[256];
        sprintf_s(filename, sizeof(filename), "%s/%d.png", folderPath, i);

        // 检查文件是否存在
        FILE* fp = fopen(filename, "r");
        if (fp == NULL) {
            break;  // 文件不存在，停止加载
        }
        fclose(fp);

        // 加载图片
        IMAGE* img = new IMAGE();
        loadimage(img, filename);
        frames.push_back(img);
    }
}
/**
 * 计算植物在屏幕上的位置
 * 完全保留原C项目的计算公式：
 * x = 256 - 112 + col * 81
 * y = 179 + row * 102 + 7
 */
void Plant::updateScreenPosition()
{
    x = 256 - 112 + col * 81;
    y = 179 + row * 102 + 7;
}

void Plant::loadFrames(const char* folderPath, std::vector<IMAGE*>& frames, int targetWidth, int targetHeight, int maxCount)
{
    // 清空原有帧
    for (auto& frame : frames) {
        if (frame) delete frame;
    }
    frames.clear();

    // 从1开始尝试加载，直到文件不存在
    for (int i = 1; i <= maxCount; i++) {
        char filename[256];
        sprintf_s(filename, sizeof(filename), "%s/%d.png", folderPath, i);

        // 检查文件是否存在
        FILE* fp = fopen(filename, "r");
        if (fp == NULL) break;
        fclose(fp);

        // 加载图片并缩放到指定大小
        IMAGE* img = new IMAGE();
        loadimage(img, filename, targetWidth, targetHeight, true);
        frames.push_back(img);
    }
}