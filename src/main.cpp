#include <graphics.h>
#include <conio.h>
#include "game/GameManager.h"
#include "ui/StartMenu.h"
#include "ui/GameUI.h"
#include "utils/EasyXHelper.h"
#include "utils/AudioManager.h"

/**
 * @file main.cpp
 * @brief 程序主入口（最终整合版）
 */

 // 全局UI对象
GameUI* gameUI = nullptr;

/**
 * @brief 显示游戏结束画面
 * @param isWin true胜利/false失败
 */
void showGameOverScreen(bool isWin)
{

    Sleep(500);

    // 停止当前背景音乐
    AudioManager::stopBGM();

    // 播放结束音效
    if (isWin) {
        PlaySound("resources/sounds/植物大战僵尸胜利通关音效_爱给网_aigei_com.wav",
            NULL, SND_FILENAME | SND_ASYNC);
        IMAGE imgWin;
        loadimage(&imgWin, "resources/images/Home/gameWin.png");
        putimage(0, 0, &imgWin);
    }
    else {
        mciSendString("open \"resources/sounds/植物大战僵尸失败音乐_爱给网_aigei_com.mp3\" "
            "type mpegvideo alias fail", NULL, 0, NULL);
        mciSendString("play fail", NULL, 0, NULL);
        IMAGE imgFail;
        loadimage(&imgFail, "resources/images/Home/gameFail.png");
        putimage(0, 0, &imgFail);
    }

    Sleep(10000);
    mciSendString("stop fail", NULL, 0, NULL);
    mciSendString("close fail", NULL, 0, NULL);
}

/**
 * @brief 统一渲染函数
 */
void render(GameManager* game, GameUI* ui)
{
    BeginBatchDraw();

    // 清屏（用背景图覆盖）
    cleardevice();

    // 绘制UI（背景、工具栏、卡片）
    if (ui) {
        ui->draw(game);
    }

    // 绘制游戏对象（植物、僵尸、子弹等）
    if (game) {
        game->draw();
    }

    EndBatchDraw();
}

/**
 * @brief 程序入口函数
 */
int main()
{
    // 1. 创建游戏窗口
    initgraph(900, 600);

    // 2. 初始化音频系统
    AudioManager::init();

    // 3. 创建游戏UI
    gameUI = new GameUI();

    // 4. 显示开始菜单
    StartMenu menu;
    if (!menu.show()) {
        // 用户退出游戏
        delete gameUI;
        AudioManager::stopAll();
        closegraph();
        return 0;
    }

    // 5. 初始化游戏（播放开场动画，无音乐）
    GameManager* game = GameManager::getInstance();
    game->init(true);  // 开场动画期间无音乐

    // 6. 游戏主循环
    while (!game->isGameOver())
    {
        static int frameCount = 0;
        frameCount++;

        // 处理鼠标消息
        ExMessage msg;
        while (peekmessage(&msg))
        {
            if (msg.message == WM_MOUSEMOVE) {
                game->handleMouseMove(msg.x, msg.y);
            }
            else if (msg.message == WM_LBUTTONDOWN) {
                game->handleClick(msg.x, msg.y);
            }
            else if (msg.message == WM_RBUTTONDOWN) {
                game->handleRightClick();
            }
        }

        game->update();

        render(game, gameUI);

        // 帧率控制
        int delay = EasyXHelper::getDelay();
        if (delay < 16) {
            Sleep(16 - delay);
        }
    }

    // 7. 游戏结束画面
    showGameOverScreen(game->getStatus() == 2);

    // 8. 清理资源
    delete gameUI;
    AudioManager::stopAll();
    closegraph();

    return 0;
}