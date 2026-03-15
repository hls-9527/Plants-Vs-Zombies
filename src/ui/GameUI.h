#pragma once
#include <graphics.h>
#include <vector>

// 前向声明
class GameManager;

/**
 * @file GameUI.h
 * @brief 游戏界面类
 *
 * 负责绘制游戏的所有UI元素：
 * 1. 背景图片
 * 2. 工具栏（阳光显示、卡片栏）
 * 3. 植物卡片（豌豆、向日葵、大嘴花）
 * 4. 铲子卡片
 * 5. 阳光数量显示
 *
 * 不继承GameObject，是独立的UI管理类
 */
class GameUI
{
private:
    // === 背景图片 ===
    IMAGE imgBg;           ///< 游戏背景
    IMAGE imgBar;          ///< 工具栏

    // === 卡片图片 ===
    IMAGE cardPeashooter;  ///< 豌豆射手卡片
    IMAGE cardSunflower;   ///< 向日葵卡片
    IMAGE cardChomper;     ///< 大嘴花卡片
    IMAGE cardShovel;      ///< 铲子卡片

    // === 卡片位置 ===
    static const int CARD_START_X = 238;  ///< 卡片起始X坐标
    static const int CARD_Y = 6;           ///< 卡片Y坐标
    static const int CARD_WIDTH = 65;      ///< 卡片宽度
    static const int CARD_HEIGHT = 90;     ///< 卡片高度（估算）

    // === 阳光显示 ===
    static const int SUNSHINE_X = 182;     ///< 阳光数量显示X坐标
    static const int SUNSHINE_Y = 67;      ///< 阳光数量显示Y坐标

public:
    /**
     * @brief 构造函数
     * 加载所有UI图片
     */
    GameUI();

    /**
     * @brief 析构函数
     */
    ~GameUI();

    /**
     * @brief 绘制所有UI元素
     * @param gameManager 游戏管理器指针（用于获取阳光数量等）
     */
    void draw(GameManager* gameManager);

    /**
     * @brief 判断点击是否在卡片区域
     * @param x 鼠标X坐标
     * @param y 鼠标Y坐标
     * @return 卡片类型：0豌豆，1向日葵，2大嘴花，3铲子，-1未点击到卡片
     */
    int getCardIndexAt(int x, int y);

private:
    /**
     * @brief 加载所有图片资源
     */
    void loadImages();

    /**
     * @brief 绘制阳光数量
     * @param sunshine 当前阳光数量
     */
    void drawSunshine(int sunshine);

    /**
     * @brief 绘制卡片栏
     */
    void drawCards();
};