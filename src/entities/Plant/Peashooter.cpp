#include "Peashooter.h"
#include "../../game/GameManager.h"
#include "../Bullet/PeaBullet.h"
#include "../Zombie/Zombie.h"

/**
 * @file Peashooter.cpp
 * @brief 豌豆射手类实现
 */

 /**
  * 构造函数
  * @param row 行索引
  * @param col 列索引
  */
Peashooter::Peashooter(int row, int col)
    : Plant(row, col, 150)  // 豌豆射手生命值150
    , shootTimer(0)
{
    // 加载豌豆射手的待机动画帧
    // 图片路径：resources/images/pt/Peashooter/1.png, 2.png, 3.png...
    loadFrames("resources/images/pt/Peashooter", idleFrames);
}

/**
 * 析构函数
 */
Peashooter::~Peashooter()
{
}

/**
 * 更新豌豆射手状态
 * 每帧调用，处理射击逻辑
 */
void Peashooter::update()
{
    if (!isAlive()) return;
    animTimer++;
    if (animTimer >= 10) {  // 每10帧更新一次动画
        animTimer = 0;
        frameIndex++;
        if (frameIndex >= (int)idleFrames.size()) {
            frameIndex = 0;
        }
    }

    // 射击逻辑：每20帧检查一次并发射子弹
    shootTimer++;
    if (shootTimer >= SHOOT_INTERVAL) {
        shootTimer = 0;

        // 检查前方是否有僵尸
        if (hasZombieInFront()) {
            shoot();
        }
    }
}

/**
 * 绘制豌豆射手
 */
void Peashooter::draw()
{
    if (!isAlive()) return;

    // 使用基类的绘制方法
    Plant::draw();
}

/**
 * 发射子弹
 * 创建一颗豌豆子弹并添加到游戏管理器的子弹列表
 */
void Peashooter::shoot()
{
    // 获取游戏管理器单例
    GameManager* game = GameManager::getInstance();

    // 创建豌豆子弹
    PeaBullet* bullet = new PeaBullet(x + 50, y + 5, row);

    // 添加到游戏管理器
    game->addBullet(bullet);
}

/**
 * 检查前方是否有僵尸
 * @return true 前方有僵尸，可以射击
 */
bool Peashooter::hasZombieInFront() const
{
    GameManager* game = GameManager::getInstance();

    // 获取当前行的所有僵尸
    const std::vector<Zombie*>& zombies = game->getZombies();

    for (auto& zombie : zombies) {
        if (!zombie || !zombie->isAlive()) continue;

        // 检查是否在同一行
        if (zombie->getRow() != row) continue;

        // 检查僵尸是否在豌豆射手前方（右边）
        // 植物x坐标 + 50 是子弹发射点，僵尸在更右边
        if (zombie->getX() > x - 20) {
            return true;  // 前方有僵尸
        }
    }

    return false;  // 没有僵尸
}