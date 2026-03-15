#include "AudioManager.h"
#include <stdio.h>

/**
 * @file AudioManager.cpp
 * @brief 音频管理类实现
 */

 // 静态成员初始化
bool AudioManager::isBGMPlaying = false;
char AudioManager::currentBGM[64] = "";
static bool mute = false;

void AudioManager::init()
{
    // 初始化音频系统
    // 不需要额外操作
}

void AudioManager::playMenuBGM()
{
    if (mute) return;

    // 停止当前正在播放的背景音乐
    stopBGM();

    // 播放开始前背景音乐（循环）
    char cmd[256];
    sprintf_s(cmd, sizeof(cmd), "open \"resources/sounds/01.开始背景音乐.MP3\" type mpegvideo alias menubgm");
    mciSendString(cmd, NULL, 0, NULL);
    mciSendString("play menubgm repeat", NULL, 0, NULL);

    isBGMPlaying = true;
    strcpy_s(currentBGM, sizeof(currentBGM), "menu");
}

void AudioManager::playGameBGM()
{
    if (mute) return;

    // 停止当前正在播放的背景音乐
    stopBGM();

    // 播放游戏内背景音乐（循环）
    char cmd[256];
    sprintf_s(cmd, sizeof(cmd), "open \"resources/sounds/Uraniwa ni Zombies ga.mp3\" type mpegvideo alias gamebgm");
    mciSendString(cmd, NULL, 0, NULL);
    mciSendString("play gamebgm repeat", NULL, 0, NULL);

    isBGMPlaying = true;
    strcpy_s(currentBGM, sizeof(currentBGM), "game");
}

void AudioManager::stopBGM()
{
    if (isBGMPlaying) {
        if (strcmp(currentBGM, "menu") == 0) {
            mciSendString("stop menubgm", NULL, 0, NULL);
            mciSendString("close menubgm", NULL, 0, NULL);
        }
        else if (strcmp(currentBGM, "game") == 0) {
            mciSendString("stop gamebgm", NULL, 0, NULL);
            mciSendString("close gamebgm", NULL, 0, NULL);
        }
        isBGMPlaying = false;
        currentBGM[0] = '\0';
    }
}

void AudioManager::playSunshineSound()
{
    if (mute) return;

    // 阳光音效用 PlaySound（.wav格式）
    PlaySound("resources/sounds/sunshine.wav", NULL, SND_FILENAME | SND_ASYNC);
}

void AudioManager::playShovelDeleteSound()
{
    if (mute) return;
    mciSendString("close shovel", NULL, 0, NULL);
    printf("播放铲子删除音效\n");  // 添加调试
    mciSendString("open \"resources/sounds/铲子_爱给网_aigei_com.mp3\" alias shovel_delete", NULL, 0, NULL);
    mciSendString("play shovel_delete", NULL, 0, NULL);
}

void AudioManager::playZombieDeadSound()
{
    if (mute) return;
    // 预留僵尸死亡音效
    // PlaySound("resources/sounds/zombie_dead.wav", NULL, SND_FILENAME | SND_ASYNC);
}

void AudioManager::stopAll()
{
    stopBGM();
    // PlaySound(NULL, NULL, 0);  // 停止所有PlaySound音效
}

void AudioManager::setMute(bool muteState)
{
    mute = muteState;
    if (mute) {
        stopAll();
    }
}

bool AudioManager::isMuted()
{
    return mute;
}

void AudioManager::playWinSound()
{
    if (mute) return;
    // 胜利音效（.wav格式，用 PlaySound）
    PlaySound("resources/sounds/植物大战僵尸胜利通关音效_爱给网_aigei_com.wav",
        NULL, SND_FILENAME | SND_ASYNC);
}

void AudioManager::playFailSound()
{
    if (mute) return;
    // 失败音乐（.mp3格式，用 mciSendString）
    mciSendString("open \"resources/sounds/植物大战僵尸失败音乐_爱给网_aigei_com.mp3\" "
        "type mpegvideo alias fail", NULL, 0, NULL);
    mciSendString("play fail", NULL, 0, NULL);
}

void AudioManager::stopFailSound()
{
    mciSendString("stop fail", NULL, 0, NULL);
    mciSendString("close fail", NULL, 0, NULL);
}

void AudioManager::playShovelSound()
{
    if (mute) return;

    mciSendString("close shovel", NULL, 0, NULL);

    mciSendString("open \"resources/sounds/铲子_爱给网_aigei_com.mp3\" alias shovel", NULL, 0, NULL);
    mciSendString("play shovel", NULL, 0, NULL);
}