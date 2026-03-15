#include "PeaBullet.h"
#include "../../utils/EasyXHelper.h"
#include "../Zombie/Zombie.h"
#include <graphics.h>

/**
 * @file PeaBullet.cpp
 * @brief 豌豆子弹类实现
 */

 /**
  * 构造函数
  * @param x 初始X坐标
  * @param y 初始Y坐标
  * @param row 所在行
  */
PeaBullet::PeaBullet(int x, int y, int row)
    : Bullet(x, y, row, 20, 4)  // 伤害20，速度4像素/帧
    , blastFrameIndex(0)
{
    // 加载爆炸原图（用于缩放绘制）
    loadimage(&blastOriginal, "resources/images/Bullet/bullet_blast.png");
}

/**
 * 析构函数
 */
PeaBullet::~PeaBullet()
{
}

/**
 * 更新豌豆子弹状态
 */
void PeaBullet::update()
{
    if (!isAlive()) return;

    if (blast) {
        // 爆炸状态：播放爆炸动画
        blastFrameIndex++;
        if (blastFrameIndex >= BLAST_FRAMES) {
            setAlive(false);  // 动画播放完毕，子弹消失
        }
    }
    else {
        // 飞行状态：向前移动
        x += speed;

        // 超出屏幕则死亡
        if (isOutOfScreen()) {
            setAlive(false);
        }
    }
}

/**
 * 绘制豌豆子弹
 */
void PeaBullet::draw()
{
    if (!isAlive()) return;

    if (blast) {
        // 直接绘制原图，不缩放
        EasyXHelper::putimagePNG(
            x - blastOriginal.getwidth() / 2,
            y - blastOriginal.getheight() / 2,
            &blastOriginal
        );
    }
    else {
        IMAGE normalImg;
        loadimage(&normalImg, "resources/images/Bullet/bullet_normal.png");
        EasyXHelper::putimagePNG(x, y, &normalImg);
    }
}
/**
 * 子弹击中目标时的回调
 */
void PeaBullet::onHit()
{
    if (!blast) {
        blast = true;           // 设置为爆炸状态
        blastFrameIndex = 0;    // 从第一帧开始播放
        speed = 0;              // 停止移动
    }
}