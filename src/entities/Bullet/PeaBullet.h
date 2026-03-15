#pragma once
#include "Bullet.h"
#include <graphics.h>

/**
 * @file PeaBullet.h
 * @brief 豌豆子弹类
 *
 * 继承自Bullet，实现豌豆子弹的特有行为：
 * - 直线向前移动
 * - 击中僵尸后播放爆炸动画（用一张图缩放实现）
 * - 爆炸后消失
 */
class PeaBullet : public Bullet
{
private:
    IMAGE blastOriginal;         ///< 爆炸原图（用于缩放绘制）
    int blastFrameIndex;         ///< 爆炸动画当前帧
    static const int BLAST_FRAMES = 4; ///< 爆炸动画总帧数

public:
    PeaBullet(int x, int y, int row);
    virtual ~PeaBullet();

    virtual void update() override;
    virtual void draw() override;
    virtual void onHit() override;
};