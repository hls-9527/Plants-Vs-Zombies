#pragma once
#include "../GameObject.h"
#include <vector>

class Plant : public GameObject
{
protected:
    int row;
    int col;
    int health;
    int maxHealth;
    int timer;
    int animTimer;  // 添加这一行

    std::vector<IMAGE*> idleFrames;     ///< 待机动画帧
    std::vector<IMAGE*> attackFrames;   ///< 攻击动画帧
    int frameCount;

public:
    Plant(int row, int col, int health);
    virtual ~Plant();

    virtual void update() override = 0;
    virtual void draw() override;

    int getRow() const { return row; }
    int getCol() const { return col; }
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }

    virtual void takeDamage(int damage);
    bool isDead() const { return health <= 0; }

    void loadFrames(const char* folderPath, std::vector<IMAGE*>& frames, int targetWidth, int targetHeight, int maxCount = 50);

    /**
     * @brief 加载动画帧（图片命名为 1.png, 2.png, 3.png...）
     * @param folderPath 图片文件夹路径
     * @param frames 目标容器
     * @param maxCount 最大尝试次数
     */
    void loadFrames(const char* folderPath, std::vector<IMAGE*>& frames, int maxCount = 50);

protected:
    void updateScreenPosition();
};