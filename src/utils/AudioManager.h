#pragma once
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

/**
 * @file AudioManager.h
 * @brief 音频管理类
 *
 * 功能：
 * 1. 播放背景音乐（支持.mp3，循环播放）
 * 2. 播放音效（支持.wav，一次性）
 * 3. 支持两段不同的背景音乐切换：
 *    - 主菜单背景音乐
 *    - 游戏内背景音乐
 *
 * 使用方式：
 * AudioManager::playMenuBGM();     // 播放主菜单音乐
 * AudioManager::playGameBGM();     // 播放游戏内音乐
 * AudioManager::playSunshineSound(); // 播放阳光收集音效
 * AudioManager::stopAll();          // 停止所有音乐
 */
class AudioManager
{
private:
    static bool isBGMPlaying;      ///< 当前是否有背景音乐在播放
    static char currentBGM[64];    ///< 当前播放的音乐标识（用于切换时停止）

public:
    /**
     * @brief 初始化音频系统
     * 在程序开始时调用一次
     */
    static void init();

    static void playShovelDeleteSound();  // 铲子删除植物音效

    /**
     * @brief 播放主菜单背景音乐
     * 循环播放，直到被切换或停止
     */
    static void playMenuBGM();

    /**
     * @brief 播放游戏内背景音乐
     * 循环播放，直到被切换或停止
     */
    static void playGameBGM();

    /**
     * @brief 停止当前背景音乐
     */
    static void stopBGM();

    /**
     * @brief 播放阳光收集音效
     * 一次性播放，不循环
     */
    static void playSunshineSound();

    /**
     * @brief 播放铲子使用音效（预留）
     */
    static void playShovelSound();

    /**
     * @brief 播放僵尸死亡音效（预留）
     */
    static void playZombieDeadSound();

    /**
     * @brief 停止所有声音（包括背景音乐和音效）
     */
    static void stopAll();

    /**
     * @brief 设置是否静音
     * @param mute true静音/false取消静音
     */
    static void setMute(bool mute);

    /**
     * @brief 获取当前静音状态
     * @return true静音/false正常
     */
    static bool isMuted();

    // 在 public 部分添加
    static void playWinSound();   // 播放胜利音效
    static void playFailSound();  // 播放失败音效
    static void stopFailSound();  // 停止失败音效（用于切换时）

};