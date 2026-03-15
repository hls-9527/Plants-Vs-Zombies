#include "GameUI.h"
#include "../game/GameManager.h"
#include "../utils/EasyXHelper.h"
#include <graphics.h>

/**
 * @file GameUI.cpp
 * @brief 游戏界面类实现
 */

 /**
  * 构造函数
  */
GameUI::GameUI()
{
    loadImages();
}

/**
 * 析构函数
 */
GameUI::~GameUI()
{
    // 图片资源会自动释放
}

/**
 * 加载所有图片资源
 */
void GameUI::loadImages()
{
    // 加载背景图片
    loadimage(&imgBg, "resources/images/Home/background.jpg");
    loadimage(&imgBar, "resources/images/Menu/bar.png");

    // 加载卡片图片（正常加载）
    loadimage(&cardPeashooter, "resources/images/Cards/2.png");
    loadimage(&cardSunflower, "resources/images/Cards/1.png");
    loadimage(&cardChomper, "resources/images/Cards/3.png");
    // 在 loadImages() 中加载铲子时指定缩放
    loadimage(&cardShovel,
        "resources/images/Cards/card_Shovel.png",
        CARD_WIDTH+20,     // 指定目标宽度
        CARD_HEIGHT,    // 指定目标高度
        true);          // 是否缩放
}

/**
 * 绘制所有UI元素
 * @param gameManager 游戏管理器指针
 */
void GameUI::draw(GameManager* gameManager)
{
    if (!gameManager) return;

    // 1. 绘制背景（原C项目中的偏移量 -112）
    putimage(-112, 0, &imgBg);

    // 2. 绘制工具栏
    EasyXHelper::putimagePNG(150, 0, &imgBar);

    // 3. 绘制卡片
    drawCards();

    // 4. 绘制阳光数量
    drawSunshine(gameManager->getSunshine());
}

/**
 * 绘制卡片栏
 */
void GameUI::drawCards()
{
    // 绘制植物卡片（原大小）
    EasyXHelper::putimagePNG(CARD_START_X + 0 * CARD_WIDTH, CARD_Y, &cardSunflower);  // 向日葵
    EasyXHelper::putimagePNG(CARD_START_X + 1 * CARD_WIDTH, CARD_Y, &cardPeashooter); // 豌豆射手
    EasyXHelper::putimagePNG(CARD_START_X + 2 * CARD_WIDTH, CARD_Y, &cardChomper);    // 大嘴花

    EasyXHelper::putimagePNG(
        CARD_START_X + 8 * CARD_WIDTH,
        CARD_Y,
        &cardShovel
    );
}

/**
 * 绘制阳光数量
 * @param sunshine 当前阳光数量
 */
void GameUI::drawSunshine(int sunshine)
{
    // 设置文字样式
    settextcolor(WHITE);
    settextstyle(30, 0, "Segoe UI Black");
    setbkmode(TRANSPARENT);

    // 显示阳光数量
    char text[16];
    sprintf_s(text, sizeof(text), "%d", sunshine);
    outtextxy(SUNSHINE_X, SUNSHINE_Y, text);
}

/**
 * 判断点击是否在卡片区域
 * @param x 鼠标X坐标
 * @param y 鼠标Y坐标
 * @return 卡片类型：0向日葵，1豌豆，2大嘴花，3铲子，-1未点击到卡片
 */
int GameUI::getCardIndexAt(int x, int y)
{
    // 检查Y坐标是否在卡片高度范围内
    if (y < CARD_Y || y > CARD_Y + CARD_HEIGHT) {
        return -1;
    }

    // 检查X坐标在哪个卡片范围内
    for (int i = 0; i < 9; i++) {
        int cardX = CARD_START_X + i * CARD_WIDTH;
        if (x >= cardX && x <= cardX + CARD_WIDTH) {
            if (i == 0) return 0;  // 向日葵
            if (i == 1) return 1;  // 豌豆射手
            if (i == 2) return 2;  // 大嘴花
            if (i == 8) return 3;  // 铲子
            return -1;
        }
    }

    return -1;
}