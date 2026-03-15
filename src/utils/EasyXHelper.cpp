#include "EasyXHelper.h"
#include <windows.h>     // 已经加了
#include <sysinfoapi.h>  // 加上这一行
#include <cmath>

// 静态变量：记录上一次调用getDelay的时间（用于帧率计算）
static unsigned long long lastTime = 0;

/**
 * 透明贴图核心算法
 *
 * 逐像素处理，实现PNG透明通道：
 * 新颜色 = 前景色 * alpha + 背景色 * (1 - alpha)
 */
void EasyXHelper::putimagePNGInternal(int x, int y, IMAGE* picture)
{
    // 获取各绘图设备的显存指针
    DWORD* dst = GetImageBuffer();                  // 目标缓冲区（窗口显存）
    DWORD* src = GetImageBuffer(picture);           // 图片缓冲区（源图片显存）

    int picture_width = picture->getwidth();        // 图片宽度
    int picture_height = picture->getheight();      // 图片高度
    int graphWidth = getwidth();                     // 窗口宽度
    int graphHeight = getheight();                   // 窗口高度

    int dstX = 0;  // 目标缓冲区中的像素索引

    // 逐像素处理
    for (int iy = 0; iy < picture_height; iy++)
    {
        for (int ix = 0; ix < picture_width; ix++)
        {
            int srcX = ix + iy * picture_width;      // 图片中的像素索引

            // 提取图片像素的ARGB分量
            int sa = ((src[srcX] & 0xff000000) >> 24); // Alpha透明度
            int sr = ((src[srcX] & 0xff0000) >> 16);   // 红
            int sg = ((src[srcX] & 0xff00) >> 8);      // 绿
            int sb = src[srcX] & 0xff;                  // 蓝

            // 检查像素是否在窗口范围内
            if (ix >= 0 && ix <= graphWidth &&
                iy >= 0 && iy <= graphHeight &&
                dstX <= graphWidth * graphHeight)
            {
                // 计算目标位置在显存中的索引
                dstX = (ix + x) + (iy + y) * graphWidth;

                // 提取目标位置的背景色分量
                int dr = ((dst[dstX] & 0xff0000) >> 16);
                int dg = ((dst[dstX] & 0xff00) >> 8);
                int db = dst[dstX] & 0xff;

                // Alpha混合公式
                int newR = (sr * sa / 255 + dr * (255 - sa) / 255);
                int newG = (sg * sa / 255 + dg * (255 - sa) / 255);
                int newB = (sb * sa / 255 + db * (255 - sa) / 255);

                // 组合成新像素
                dst[dstX] = (0xff000000)              // 完全不透明
                    | (newR << 16)
                    | (newG << 8)
                    | newB;
            }
        }
    }
}

/**
 * 透明贴图主函数（处理各种边界情况）
 */
void EasyXHelper::putimagePNG(int x, int y, IMAGE* picture)
{
    IMAGE imgTmp, imgTmp2, imgTmp3;
    int winWidth = getwidth();
    int winHeight = getheight();

    // 处理图片顶部超出屏幕（y < 0）
    if (y < 0) {
        SetWorkingImage(picture);
        getimage(&imgTmp, 0, -y, picture->getwidth(), picture->getheight() + y);
        SetWorkingImage();
        y = 0;
        picture = &imgTmp;
    }
    // 图片完全超出屏幕，直接返回
    else if (y >= getheight() || x >= getwidth()) {
        return;
    }
    // 处理图片底部超出屏幕
    else if (y + picture->getheight() > winHeight) {
        SetWorkingImage(picture);
        getimage(&imgTmp, x, y, picture->getwidth(), winHeight - y);
        SetWorkingImage();
        picture = &imgTmp;
    }

    // 处理图片左侧超出屏幕（x < 0）
    if (x < 0) {
        SetWorkingImage(picture);
        getimage(&imgTmp2, -x, 0, picture->getwidth() + x, picture->getheight());
        SetWorkingImage();
        x = 0;
        picture = &imgTmp2;
    }

    // 处理图片右侧超出屏幕
    if (x > winWidth - picture->getwidth()) {
        SetWorkingImage(picture);
        getimage(&imgTmp3, 0, 0, winWidth - x, picture->getheight());
        SetWorkingImage();
        picture = &imgTmp3;
    }

    // 调用内部透明贴图函数
    putimagePNGInternal(x, y, picture);
}

/**
 * 获取两帧之间的时间间隔（毫秒）
 * 使用 GetTickCount64 避免 49 天溢出问题
 */
int EasyXHelper::getDelay()
{
    static unsigned long long lastTime = 0;  // 改成 64 位
    unsigned long long currentTime = GetTickCount64();  // 用 64 位版本

    if (lastTime == 0) {
        lastTime = currentTime;
        return 0;
    }
    else {
        unsigned long long ret = currentTime - lastTime;  // 64 位减法
        lastTime = currentTime;
        return (int)ret;  // 强制转换回 int（游戏里够用）
    }
}