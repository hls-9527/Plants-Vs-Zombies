#include "StartMenu.h"
#include "../utils/EasyXHelper.h"
#include "../utils/AudioManager.h"
#include <graphics.h>

/**
 * @file StartMenu.cpp
 * @brief 开始菜单类实现
 */

 /**
  * 构造函数
  */
StartMenu::StartMenu()
    : isHovering(false)
{
    loadImages();
}

/**
 * 析构函数
 */
StartMenu::~StartMenu()
{
    // 图片资源会自动释放
}

/**
 * 加载所有图片资源
 */
void StartMenu::loadImages()
{
    // 加载主菜单背景
    loadimage(&imgMenuBg, "resources/images/Home/menu.png");

    // 加载按钮图片
    loadimage(&btnAdventureNormal, "resources/images/Home/menu_0.png");
    loadimage(&btnAdventureHover, "resources/images/Home/menu_1.png");
}

/**
 * 绘制菜单
 */
void StartMenu::draw()
{
    BeginBatchDraw();

    // 绘制背景
    putimage(0, 0, &imgMenuBg);

    // 根据鼠标悬停状态绘制对应的按钮图片
    if (isHovering) {
        EasyXHelper::putimagePNG(BTN_X, BTN_Y, &btnAdventureHover);
    }
    else {
        EasyXHelper::putimagePNG(BTN_X, BTN_Y, &btnAdventureNormal);
    }

    EndBatchDraw();
}

/**
 * 处理鼠标事件
 * @return true 点击了开始按钮，false 未点击
 */
bool StartMenu::handleMouse()
{
    ExMessage msg;
    static bool isPressed = false;  // 记录鼠标是否按下（用于点击检测）

    while (peekmessage(&msg)) {
        switch (msg.message) {
        case WM_MOUSEMOVE:
            // 检查鼠标是否在按钮区域内
            if (msg.x >= BTN_X && msg.x <= BTN_X + BTN_WIDTH &&
                msg.y >= BTN_Y && msg.y <= BTN_Y + BTN_HEIGHT) {
                isHovering = true;
            }
            else {
                isHovering = false;
            }
            break;

        case WM_LBUTTONDOWN:
            // 鼠标按下时，如果在按钮区域内，记录按下状态
            if (msg.x >= BTN_X && msg.x <= BTN_X + BTN_WIDTH &&
                msg.y >= BTN_Y && msg.y <= BTN_Y + BTN_HEIGHT) {
                isPressed = true;
            }
            break;

        case WM_LBUTTONUP:
            // 鼠标抬起时，如果之前在按钮内按下，触发点击
            if (isPressed &&
                msg.x >= BTN_X && msg.x <= BTN_X + BTN_WIDTH &&
                msg.y >= BTN_Y && msg.y <= BTN_Y + BTN_HEIGHT) {
                isPressed = false;
                return true;  // 点击了开始按钮
            }
            isPressed = false;
            break;
        }
    }

    return false;
}

/**
 * 显示菜单并处理交互
 * @return true 点击了开始按钮，false 退出游戏
 */
bool StartMenu::show()
{
    // 播放主菜单背景音乐
    AudioManager::playMenuBGM();

    // 主循环
    while (true) {
        // 处理鼠标事件
        if (handleMouse()) {
            // 点击了开始按钮，停止当前音乐，返回true
            AudioManager::stopBGM();
            return true;
        }

        // 绘制菜单
        draw();

        // 简单的帧率控制
        Sleep(10);

        // 检查是否按下ESC退出
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            AudioManager::stopBGM();
            return false;  // 退出游戏
        }
    }
}