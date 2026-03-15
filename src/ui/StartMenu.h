#pragma once
#include <graphics.h>

/**
 * @file StartMenu.h
 * @brief 开始菜单类
 *
 * 负责显示游戏开始菜单，包括：
 * 1. 主界面背景（menu.png）
 * 2. 冒险模式按钮（menu_0.png / menu_1.png 切换）
 * 3. 鼠标点击检测
 * 4. 播放主菜单背景音乐
 *
 * 不继承GameObject，是独立的界面类
 */
class StartMenu
{
private:
    // === 背景图片 ===
    IMAGE imgMenuBg;           ///< 主菜单背景（menu.png）

    // === 按钮图片 ===
    IMAGE btnAdventureNormal;  ///< 冒险模式按钮暗色版（menu_0.png）
    IMAGE btnAdventureHover;   ///< 冒险模式按钮亮色版（menu_1.png）

    // === 按钮位置 ===
    static const int BTN_X = 474;      ///< 按钮X坐标
    static const int BTN_Y = 75;       ///< 按钮Y坐标
    static const int BTN_WIDTH = 300;  ///< 按钮宽度
    static const int BTN_HEIGHT = 140; ///< 按钮高度

    bool isHovering;           ///< 鼠标是否悬停在按钮上

public:
    /**
     * @brief 构造函数
     * 加载菜单所需的所有图片
     */
    StartMenu();

    /**
     * @brief 析构函数
     */
    ~StartMenu();

    /**
     * @brief 显示菜单并处理交互
     * @return true 点击了开始按钮，false 退出游戏
     *
     * 这是一个阻塞函数，会一直循环直到用户点击开始或退出
     */
    bool show();

private:
    /**
     * @brief 加载所有图片资源
     */
    void loadImages();

    /**
     * @brief 绘制菜单
     */
    void draw();

    /**
     * @brief 处理鼠标事件
     * @return true 点击了开始按钮，false 未点击
     */
    bool handleMouse();
};