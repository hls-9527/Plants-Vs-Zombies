#pragma once
#include <graphics.h>

/**
 * @file EasyXHelper.h
 * @brief EasyX图形库辅助工具类
 *
 * 封装底层图形操作，提供透明贴图、帧率控制等功能
 * 所有方法均为静态，无需实例化
 */
class EasyXHelper {
public:
    /**
     * @brief 输出带透明通道的PNG图片
     * @param x 目标X坐标
     * @param y 目标Y坐标
     * @param picture 要输出的图片对象
     */
    static void putimagePNG(int x, int y, IMAGE* picture);

    /**
     * @brief 获取两帧之间的时间间隔（毫秒）
     * @return 距离上次调用经过的毫秒数
     */
    static int getDelay();

private:
    /**
     * @brief 内部实现：透明贴图核心算法
     */
    static void putimagePNGInternal(int x, int y, IMAGE* picture);
};