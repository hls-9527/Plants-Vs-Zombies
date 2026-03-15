#include "Sunshine.h"
#include "../game/GameManager.h"
#include "../utils/EasyXHelper.h"
#include <graphics.h>
#include <cstdlib>
#include <cmath>

/**
 * @file Sunshine.cpp
 * @brief 阳光实体类实现
 */

 /**
  * 构造函数（自然掉落的阳光）
  * @param startX 起始X坐标
  * @param startY 起始Y坐标（通常为60）
  */
Sunshine::Sunshine(int startX, int startY)
    : GameObject(startX, startY)
    , currentState(STATE_FALLING)
    , row(-1)
    , timer(0)
    , groundTimer(0)
    , t(0)
    , startX(startX)
    , startY(startY)
    , speed(0.002f)
{
    // 加载动画帧
    loadFrames();

    // 初始化贝塞尔曲线（自然掉落）
    initFallingCurve();
}

/**
 * 构造函数（向日葵生产的阳光）
 * @param plantX 向日葵X坐标
 * @param plantY 向日葵Y坐标
 * @param row 所在行
 */
Sunshine::Sunshine(int plantX, int plantY, int row)
    : GameObject(plantX, plantY)
    , currentState(STATE_FALLING)
    , row(row)
    , timer(0)
    , groundTimer(0)
    , t(0)
    , startX(plantX)
    , startY(plantY)
    , speed(0.0f)
{
    // 加载动画帧
    loadFrames();

    // 初始化贝塞尔曲线（向日葵生产）
    initProductCurve();
}

/**
 * 析构函数
 */
Sunshine::~Sunshine()
{
    // 释放动画帧
    for (auto& frame : frames) {
        if (frame) delete frame;
    }
    frames.clear();
}

/**
 * 加载阳光动画帧
 * 原C项目阳光有29帧动画
 */
void Sunshine::loadFrames()
{
    char filename[256];
    for (int i = 1; i <= ANIMATION_FRAMES; i++) {
        sprintf_s(filename, sizeof(filename), "resources/images/sunshine/%d.png", i);

        // 检查文件是否存在
        FILE* fp = fopen(filename, "r");
        if (fp == NULL) {
            break;
        }
        fclose(fp);

        IMAGE* img = new IMAGE();
        loadimage(img, filename);
        frames.push_back(img);

    }
}

/**
 * 初始化贝塞尔曲线（自然掉落）
 * 原C项目：p1 = (x, 60), p4 = (x, 200 + rand()%4*90)
 */
void Sunshine::initFallingCurve()
{
    // 限制起点在屏幕可视范围内
    if (startX < 100) startX = 100;
    if (startX > 800) startX = 800;

    targetX = startX;           // x坐标不变
    targetY = 520;              // 掉到屏幕底部

    // 贝塞尔曲线控制点（简单直线）
    controlX1 = startX;
    controlY1 = startY + (targetY - startY) / 3;
    controlX2 = targetX;
    controlY2 = targetY - (targetY - startY) / 3;
}

/**
 * 初始化贝塞尔曲线（向日葵生产）
 * 原C项目：有弯曲的轨迹，左右随机偏移
 */
void Sunshine::initProductCurve()
{
    // 随机选择方向：0左上角，1右上角
    int direction = rand() % 2;

    if (direction == 0) {
        // 左上角
        targetX = startX - 40;
    }
    else {
        // 右上角
        targetX = startX + 40;
    }

    targetY = startY - 40;  // 植物上方

    t = 1.0f;
    updateBezierPoint(t);
    currentState = STATE_GROUND;
}

/**
 * 计算贝塞尔曲线上的点
 * 三阶贝塞尔曲线公式：B(t) = (1-t)^3*P0 + 3(1-t)^2*t*P1 + 3(1-t)*t^2*P2 + t^3*P3
 */
void Sunshine::updateBezierPoint(float t)
{
    float u = 1 - t;
    float tt = t * t;
    float uu = u * u;
    float uuu = uu * u;
    float ttt = tt * t;

    x = (int)(uuu * startX + 3 * uu * t * controlX1 + 3 * u * tt * controlX2 + ttt * targetX);
    y = (int)(uuu * startY + 3 * uu * t * controlY1 + 3 * u * tt * controlY2 + ttt * targetY);
}

/**
 * 更新阳光状态
 */
void Sunshine::update()
{
    if (!isAlive()) return;

    // 添加空帧检查
    if (frames.empty()) {
        setAlive(false);
        return;
    }

    // 更新动画帧
    frameIndex++;
    if (frameIndex >= (int)frames.size()) {
        frameIndex = 0;
    }

    // 状态机
    switch (currentState)
    {
    case STATE_FALLING:
        // 飘落状态：沿贝塞尔曲线移动
        t += speed;
        if (t >= 1.0f) {
            t = 1.0f;
            updateBezierPoint(t);
            currentState = STATE_GROUND;
            groundTimer = 0;
        }
        else {
            updateBezierPoint(t);
        }
        break;

    case STATE_GROUND:
        // 地面停留状态：等待被收集或超时消失
        groundTimer++;
        if (groundTimer >= GROUND_DURATION) {
            setAlive(false);  // 超时消失
        }
        break;

    case STATE_COLLECTING:
        // 收集状态：飞向阳光计数区（162, 0）
        t += 0.03f;  // 飞得更快
        if (t >= 1.0f) {
            // 到达阳光计数区，增加阳光并消失
            GameManager* game = GameManager::getInstance();
            game->addSunshine(25);
            setAlive(false);
        }
        else {
            // 飞向目标点
            startX = x;
            startY = y;
            targetX = 162;
            targetY = 0;
            updateBezierPoint(t);
        }
        break;
    }
}

/**
 * 绘制阳光
 */
void Sunshine::draw()
{
    if (!isAlive()) return;

    if (frames.empty()) return;  // 添加这一行

    if (!frames.empty()) {
        IMAGE* img = frames[frameIndex % frames.size()];
        if (img) {
            EasyXHelper::putimagePNG(x, y, img);
        }
    }
}

/**
 * 判断鼠标是否点击到阳光
 * @param mouseX 鼠标X坐标
 * @param mouseY 鼠标Y坐标
 * @return true 点击到阳光
 */
bool Sunshine::contains(int mouseX, int mouseY) const
{
    if (!isAlive() || frames.empty()) return false;

    IMAGE* img = frames[0];
    int width = img->getwidth();
    int height = img->getheight();

    return (mouseX >= x && mouseX <= x + width &&
        mouseY >= y && mouseY <= y + height);
}

/**
 * 收集阳光（被鼠标点击时调用）
 */
void Sunshine::collect()
{
    if (currentState == STATE_GROUND || currentState == STATE_FALLING) {
        currentState = STATE_COLLECTING;
        t = 0;
        startX = x;
        startY = y;
        targetX = 162;
        targetY = 0;
    }
}