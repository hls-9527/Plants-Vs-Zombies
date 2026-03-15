#include "GameManager.h"
#include "../entities/Plant/Plant.h"
#include "../entities/Plant/Peashooter.h"
#include "../entities/Plant/Sunflower.h"
#include "../entities/Plant/Chomper.h"
#include "../entities/Zombie/Zombie.h"
#include "../entities/Zombie/BasicZombie.h"
#include "../entities/Bullet/Bullet.h"
#include "../entities/Bullet/PeaBullet.h"
#include "../entities/Sunshine.h"
#include "../ui/Shovel.h"
#include "../utils/AudioManager.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>

/**
 * @file GameManager.cpp
 * @brief 游戏核心管理器实现
 */

 // 静态单例实例
static GameManager* instance = nullptr;
/**
 * 构造函数
 * 初始化所有成员变量
 */
GameManager::GameManager()
    : currentStatus(MENU)
    , sunshineCount(50)        // 初始阳光50
    , currentLevel(1)
    , frameCount(0)
    , shovel(nullptr)
    , killCount(0)
    , zombieCount(0)
    , selectedPlantType(-1)
    , followImage(nullptr)
    , followX(0)
    , followY(0)
    , showGoal(false)
{
    // 初始化随机种子
    srand(static_cast<unsigned>(time(nullptr)));

    // 初始化植物网格：3行9列，全部置空
    plantGrid.resize(3, std::vector<Plant*>(9, nullptr));
}

/**
 * 析构函数
 * 清理所有动态分配的对象
 */
GameManager::~GameManager()
{
    // 清理植物网格
    for (auto& row : plantGrid) {
        for (auto& plant : row) {
            if (plant) {
                delete plant;
                plant = nullptr;
            }
        }
    }

    // 清理所有僵尸
    for (auto& zombie : zombies) {
        delete zombie;
    }
    zombies.clear();

    // 清理所有子弹
    for (auto& bullet : bullets) {
        delete bullet;
    }
    bullets.clear();

    // 清理所有阳光
    for (auto& sunshine : sunshines) {
        delete sunshine;
    }
    sunshines.clear();

    // 清理铲子
    if (shovel) {
        delete shovel;
        shovel = nullptr;
    }

    instance = nullptr;
}

/**
 * 获取单例实例
 */
GameManager* GameManager::getInstance()
{
    if (instance == nullptr) {
        instance = new GameManager();
    }
    return instance;
}

/**
 * 初始化游戏
 * @param fromMenu 是否从菜单进入
 */
void GameManager::init(bool fromMenu)
{
    // 重置游戏状态
    currentStatus = PLAYING;
    sunshineCount = 50;
    frameCount = 0;
    zombieCount = 0;
    showGoal = false;  // 重置目标显示

    // 清空植物网格
    for (auto& row : plantGrid) {
        for (auto& plant : row) {
            if (plant) {
                delete plant;
                plant = nullptr;
            }
        }
    }

    // 清空所有容器
    for (auto& zombie : zombies) delete zombie;
    zombies.clear();

    for (auto& bullet : bullets) delete bullet;
    bullets.clear();

    for (auto& sunshine : sunshines) delete sunshine;
    sunshines.clear();

    // 创建铲子
    if (shovel) delete shovel;
    shovel = new Shovel();

    // 如果从菜单进入，播放开场动画
    if (fromMenu) {
        playIntroAnimation();
    }
}

/**
 * 播放开场动画
 */
void GameManager::playIntroAnimation()
{
    printf("播放开场动画...\n");

    // 加载僵尸站立动画帧
    std::vector<IMAGE*> standFrames;
    char filename[256];
    for (int i = 1; i <= 11; i++) {
        sprintf_s(filename, sizeof(filename), "resources/images/zb/zb_stand/%d.png", i);
        FILE* fp = fopen(filename, "r");
        if (fp == NULL) break;
        fclose(fp);

        IMAGE* img = new IMAGE();
        loadimage(img, filename);
        standFrames.push_back(img);
    }

    // 获取背景图片宽度
    IMAGE imgBg;
    loadimage(&imgBg, "resources/images/Home/background.jpg");
    int bgWidth = imgBg.getwidth();
    int bgHeight = imgBg.getheight();

    // 定义基准X坐标
    int baseX = 600;

    // 预先决定每个僵尸的方向（0左，1右）
    bool directions[11];
    for (int d = 0; d < 11; d++) {
        directions[d] = (rand() % 2 == 0);
    }

    // 1. 背景滚动（从右向左）
    int xMin = 900 - bgWidth;
    for (int x = 0; x >= xMin; x -= 2) {
        BeginBatchDraw();
        cleardevice();
        putimage(x, 0, &imgBg);

        for (int k = 0; k < 11; k++) {
            if (!standFrames.empty()) {
                int frameIndex = (k + frameCount) % standFrames.size();
                int zx, zy;

                if (k < 5) {
                    // 上面的5个僵尸
                    if (directions[k]) {
                        zx = baseX - 2 * 25;
                    }
                    else {
                        zx = baseX + 2 * 25;
                    }
                    zy = 400 - (5 - k) * 35;
                }
                else if (k == 5) {
                    // 顶点
                    zx = baseX;
                    zy = 400;
                }
                else {
                    // 下面的5个僵尸
                    if (directions[k]) {
                        zx = (baseX + 20) - 3 * 20;
                    }
                    else {
                        zx = (baseX + 20) + 3 * 20;
                    }
                    zy = 400 + (k - 5) * 35;
                }

                // 边界检查
                int imgHeight = standFrames[frameIndex]->getheight();
                if (zy + imgHeight > 850) zy = 850 - imgHeight;
                if (zy < 50) zy = 50;

                int drawX = zx - xMin + x;
                EasyXHelper::putimagePNG(drawX, zy - imgHeight, standFrames[frameIndex]);
            }
        }
        EndBatchDraw();
        Sleep(10);
    }

    // 2. 停留动画（使用同样的方向数组）
    for (int i = 0; i < 100; i++) {
        BeginBatchDraw();
        cleardevice();
        putimage(xMin, 0, &imgBg);

        for (int k = 0; k < 11; k++) {
            if (!standFrames.empty()) {
                int frameIndex = (k + frameCount + i / 5) % standFrames.size();
                int zx, zy;

                if (k < 5) {
                    // 上面的5个僵尸
                    if (directions[k]) {
                        zx = baseX - 2 * 25;
                    }
                    else {
                        zx = baseX + 2 * 25;
                    }
                    zy = 400 - (5 - k) * 35;
                }
                else if (k == 5) {
                    // 顶点
                    zx = baseX;
                    zy = 400;
                }
                else {
                    // 下面的5个僵尸
                    if (directions[k]) {
                        zx = (baseX + 20) - 3 * 20;
                    }
                    else {
                        zx = (baseX + 20) + 3 * 20;
                    }
                    zy = 400 + (k - 5) * 35;
                }

                int imgHeight = standFrames[frameIndex]->getheight();
                if (zy + imgHeight > 850) zy = 850 - imgHeight;
                if (zy < 50) zy = 50;

                EasyXHelper::putimagePNG(zx, zy - imgHeight, standFrames[frameIndex]);
            }
        }
        EndBatchDraw();
        Sleep(30);
    }

    // 3. 卡片栏下落（使用同样的方向数组）
    IMAGE imgBar;
    loadimage(&imgBar, "resources/images/Menu/bar.png");
    int barHeight = imgBar.getheight();

    for (int y = -barHeight; y <= 0; y += 2) {
        BeginBatchDraw();
        cleardevice();
        putimage(xMin, 0, &imgBg);

        for (int k = 0; k < 11; k++) {
            if (!standFrames.empty()) {
                int frameIndex = (k + frameCount) % standFrames.size();
                int zx, zy;

                if (k < 5) {
                    // 上面的5个僵尸
                    if (directions[k]) {
                        zx = baseX - 2 * 25;
                    }
                    else {
                        zx = baseX + 2 * 25;
                    }
                    zy = 400 - (5 - k) * 35;
                }
                else if (k == 5) {
                    // 顶点
                    zx = baseX;
                    zy = 400;
                }
                else {
                    // 下面的5个僵尸
                    if (directions[k]) {
                        zx = (baseX + 20) - 3 * 20;
                    }
                    else {
                        zx = (baseX + 20) + 3 * 20;
                    }
                    zy = 400 + (k - 5) * 35;
                }

                int imgHeight = standFrames[frameIndex]->getheight();
                if (zy + imgHeight > 850) {
                    zy = 850 - imgHeight;
                }
                if (zy < 50) zy = 50;

                EasyXHelper::putimagePNG(zx, zy - imgHeight, standFrames[frameIndex]);
            }
        }

        EasyXHelper::putimagePNG(150, y, &imgBar);
        EndBatchDraw();
        Sleep(5);
    }

    // 清理站立动画帧
    for (auto& frame : standFrames) {
        delete frame;
    }
    standFrames.clear();

    printf("开场动画结束\n");
    AudioManager::playGameBGM();
}


// ==================== 更新逻辑 ====================

/**
 * 更新所有游戏对象
 * 主循环中每帧调用一次
 */
void GameManager::update()
{
    if (currentStatus != PLAYING) return;

    frameCount++;

    // 1. 更新所有植物
    for (auto& row : plantGrid) {
        for (auto& plant : row) {
            if (plant && plant->isAlive()) {
                plant->update();
            }
        }
    }

    // 2. 生成新阳光（自然掉落）
    createSunshine();

    // 3. 更新所有阳光
    for (auto& sunshine : sunshines) {
        if (sunshine && sunshine->isAlive()) {
            sunshine->update();
        }
    }

    // 4. 生成新僵尸
    createZombie();

    // 5. 更新所有僵尸
    for (auto& zombie : zombies) {
        if (zombie) {
            zombie->update();
        }
    }

    // 6. 更新所有子弹
    for (auto& bullet : bullets) {
        if (bullet && bullet->isAlive()) {
            bullet->update();
        }
    }

    // 7. 碰撞检测
    checkCollisions();

    // 8. 移除死亡对象
    removeDeadObjects();

    // 9. 更新铲子
    if (shovel) {
        shovel->update();
    }

    // 10. 检查游戏是否胜利/失败
// 失败条件：有僵尸到达房子
    for (auto& zombie : zombies) {
        if (zombie && zombie->isAlive() && zombie->isAtHouse()) {
            currentStatus = FAIL;
            break;
        }
    }

    // 胜利条件：击杀数量达到要求
    if (currentStatus == PLAYING && killCount >= WIN_CONDITION) {
        currentStatus = WIN;
    }
}

/**
 * 绘制所有游戏对象
 */
void GameManager::draw()
{

    // 4. 绘制所有植物
    for (auto& row : plantGrid) {
        for (auto& plant : row) {
            if (plant && plant->isAlive()) {
                plant->draw();
            }
        }
    }

    // 5. 绘制所有阳光
    for (auto& sunshine : sunshines) {
        if (sunshine && sunshine->isAlive()) {
            sunshine->draw();
        }
    }

    // 6. 绘制所有僵尸
    for (auto& zombie : zombies) {
        if (zombie) {
            zombie->draw();
        }
    }

    // 7. 绘制所有子弹
    for (auto& bullet : bullets) {
        if (bullet && bullet->isAlive()) {
            bullet->draw();
        }
    }

    // 8. 绘制铲子（最后绘制，显示在最上层）
    if (shovel) {
        shovel->draw();
    }

    // 9. 绘制阳光数量
    setcolor(WHITE);
    settextstyle(30, 0, "Segoe UI Black");
    char scoreText[8];
    sprintf_s(scoreText, "%d", sunshineCount);
    outtextxy(182, 67, scoreText);


    // 10. 绘制鼠标跟随的植物图片
    if (selectedPlantType != -1 && followImage) {
        int w = followImage->getwidth();
        int h = followImage->getheight();
        EasyXHelper::putimagePNG(followX - w / 2, followY - h / 2, followImage);
    }

    // 11. 绘制通关目标（如果已开启）
    if (showGoal) {
        setcolor(RGB(255, 100, 100));  // 浅红色
        settextstyle(40, 0, "Segoe UI Black");  // 字体扩大一倍
        char goalText[32];
        sprintf_s(goalText, sizeof(goalText), "通关目标： %d/%d", killCount, WIN_CONDITION);
        outtextxy(650, 520, goalText);
    }
}


// ==================== 交互逻辑 ====================

/**
 * 处理鼠标点击
 */
void GameManager::handleClick(int mouseX, int mouseY)
{
    if (currentStatus != PLAYING) return;

    // 1. 先让铲子处理点击（如果铲子激活）
    if (shovel && shovel->isActive()) {
        // 计算点击的网格位置
        int row, col;
        if (getGridIndexFromPos(mouseX, mouseY, row, col)) {
            // 在网格内点击，让铲子处理
            if (shovel->handleClick(mouseX, mouseY, row, col, plantGrid[row][col])) {
                return;  // 铲子处理完成
            }
        }
        else {
            // 点击在网格外，取消铲子
            shovel->deactivate();
        }
        return;
    }

    // 2. 检查是否点击了铲子卡片（第9个位置，索引8）
    int shovelCardX = 238 + 8 * 65;  // 758
    if (mouseX > shovelCardX && mouseX < shovelCardX + 65 && mouseY > 6 && mouseY < 96) {
        if (shovel) {
            shovel->activate();
            printf("铲子激活\n");  // 添加调试
        }
        return;
    }

    // 3. 检查是否点击了植物卡片（种植）
    for (int i = 0; i < 3; i++) {
        int cardX = 238 + i * 65;
        if (mouseX > cardX && mouseX < cardX + 65 && mouseY > 6 && mouseY < 96) {

            // 检查阳光是否足够
            int cost = 0;
            switch (i) {
            case 0: cost = 50; break;  // 向日葵
            case 1: cost = 100; break;  // 豌豆射手
            case 2: cost = 150; break; // 大嘴花
            }

            if (sunshineCount < cost) {
                return;  // 阳光不足，不选中
            }

            // 记录当前选中的植物类型
            selectedPlantType = i;

            // 加载对应的待机图片作为跟随图片
            if (followImage) delete followImage;
            followImage = new IMAGE();

            char path[256];
            switch (i) {
            case 0:  // 向日葵
                sprintf_s(path, "resources/images/pt/Sunflower/1.png");
                break;
            case 1:  // 豌豆射手
                sprintf_s(path, "resources/images/pt/Peashooter/1.png");
                break;
            case 2:  // 大嘴花
                sprintf_s(path, "resources/images/pt/Chomper/Chomper_stand/1.png");
                break;
            }
            loadimage(followImage, path);

            return;
        }
    }

    // 4. 检查是否点击了阳光
    for (auto& sunshine : sunshines) {
        if (sunshine && sunshine->isAlive()) {
            if (sunshine->contains(mouseX, mouseY)) {
                sunshine->collect();
                AudioManager::playSunshineSound();  // 播放收集音效
                break;  // 一次只收集一个阳光
            }
        }
    }

    // 5. 检查是否点击了网格（种植）
    int row, col;
    if (getGridIndexFromPos(mouseX, mouseY, row, col)) {
        // 如果有选中的植物类型，尝试种植
        if (selectedPlantType != -1) {
            if (plantAt(row, col, selectedPlantType)) {
                // 种植成功，清除选中状态和跟随图片
                selectedPlantType = -1;
                if (followImage) {
                    delete followImage;
                    followImage = nullptr;
                }
            }
        }
    }
}

/**
 * 处理鼠标移动
 */
void GameManager::handleMouseMove(int mouseX, int mouseY)
{
    if (shovel) {
        shovel->updateMousePos(mouseX, mouseY);
    }

    // 更新跟随图片位置
    followX = mouseX;
    followY = mouseY;
}

/**
 * 处理鼠标右键点击
 */
void GameManager::handleRightClick()
{
    // 1. 如果铲子激活，取消铲子
    if (shovel && shovel->isActive()) {
        shovel->deactivate();
        return;
    }

    // 2. 如果选中了植物，取消选中
    if (selectedPlantType != -1) {
        selectedPlantType = -1;
        if (followImage) {
            delete followImage;
            followImage = nullptr;
        }
    }
}

/**
 * 根据坐标获取网格索引
 */
bool GameManager::getGridIndexFromPos(int x, int y, int& row, int& col)
{
    // 网格区域范围
    int gridLeft = 256 - 112;  // 144
    int gridTop = 179;
    int gridRight = gridLeft + 9 * 81;  // 144 + 729 = 873
    int gridBottom = gridTop + 3 * 102;  // 179 + 306 = 485

    if (x < gridLeft || x > gridRight || y < gridTop || y > gridBottom) {
        return false;
    }

    col = (x - gridLeft) / 81;
    row = (y - gridTop) / 102;

    return true;
}



// ==================== 阳光系统 ====================

/**
 * 增加阳光
 */
void GameManager::addSunshine(int amount)
{
    sunshineCount += amount;
}

/**
 * 消耗阳光
 */
bool GameManager::consumeSunshine(int amount)
{
    if (sunshineCount >= amount) {
        sunshineCount -= amount;
        return true;
    }
    return false;
}

/**
 * 创建自然掉落的阳光
 */
void GameManager::createSunshine()
{
    static int sunshineTimer = 0;
    sunshineTimer++;

    // 每300-500帧生成一个阳光
    if (sunshineTimer > 1000 + rand() % 500) {
        sunshineTimer = 0;

        // 创建阳光对象
        // 随机X坐标：160 ~ 900
        int x = rand() % (900 - 160) + 160;
        Sunshine* sun = new Sunshine(x, 60);  // 从y=60开始飘落
        sunshines.push_back(sun);
    }
}

// ==================== 僵尸系统 ====================

/**
 * 创建新僵尸
 */
void GameManager::createZombie()
{
    // 如果已经生成够了，直接返回
    if (zombieCount >= TOTAL_ZOMBIES) {
        return;
    }
    static int zombieTimer = 0;
    zombieTimer++;

    // 根据已生成僵尸数量决定刷新间隔和速度
    int interval = 0;
    float zombieSpeed = 0.1f;

    if (zombieCount == 0) {
        // 第一只
        interval = 5000 + rand() % 100;
        zombieSpeed = 0.15f;
    }
    else if (zombieCount <= TOTAL_ZOMBIES * 0.05) {
        // 前1/4
        interval = 1500 + rand() % 500;
        zombieSpeed = 0.2f;
    }
    else if (zombieCount <= TOTAL_ZOMBIES * 0.1) {
        // 中前1/4
        interval = 1000 + rand() % 500;
        zombieSpeed = 0.25f;
    }
    else if (zombieCount <= TOTAL_ZOMBIES * 0.25) {
        // 中前1/4
        interval = 500 + rand() % 100;
        zombieSpeed = 0.25f;
    }
    else if (zombieCount <= TOTAL_ZOMBIES * 0.5) {
        // 中后1/4
        interval = 100;
        zombieSpeed = 0.3f;
    }
    else if (zombieCount <= TOTAL_ZOMBIES * 0.75) {
        // 中后1/4
        interval = 50;
        zombieSpeed = 0.35f;
    }
    else {
        // 后半期
        interval = 10;
        zombieSpeed = 0.4f;
    }

    if (zombieTimer > interval) {
        zombieTimer = 0;

        // 再次检查，防止多个僵尸同时生成
        if (zombieCount >= TOTAL_ZOMBIES) {
            return;
        }

        if (zombieCount == 0) {
            showGoal = true;  // 只有真正生成第一只僵尸时才显示
        }

        int row = rand() % 3;

        // 创建僵尸时传入速度
        Zombie* zombie = new BasicZombie(row, zombieSpeed);
        zombies.push_back(zombie);
        zombieCount++;
    }
}

// ==================== 碰撞检测 ====================

/**
 * 检查所有碰撞
 */
void GameManager::checkCollisions()
{
    // 1. 子弹与僵尸碰撞
    for (auto& bullet : bullets) {
        if (!bullet || !bullet->isAlive() || bullet->isBlast()) continue;

        for (auto& zombie : zombies) {
            if (!zombie || !zombie->isAlive()) continue;

            // 检查是否在同一行
            if (bullet->getRow() != zombie->getRow()) continue;

            // 获取子弹和僵尸的位置
            int bulletX = bullet->getX();
            int zombieX = zombie->getX();

            // 碰撞检测：子弹碰到僵尸的身体范围
            // 僵尸图片宽度约80-100像素，取中间范围
            if (bulletX >= zombieX + 70 && bulletX <= zombieX + 100) {
                // 子弹击中僵尸
                bullet->onHit();
                zombie->takeDamage(bullet->getDamage());
                break;  // 一颗子弹只能击中一个僵尸
            }
        }
    }

    // 2. 僵尸与植物碰撞
    for (auto& zombie : zombies) {
        if (!zombie || !zombie->isAlive() || zombie->isEating()) continue;

        int row = zombie->getRow();
        int zombieX = zombie->getX();

        // 检查当前行的所有植物
        for (int col = 0; col < 9; col++) {
            Plant* plant = plantGrid[row][col];
            if (!plant || !plant->isAlive()) continue;

            int plantX = plant->getX();
            int plantWidth = 30;  // 植物的大概宽度

            // 检测僵尸是否碰到植物
            // 僵尸的嘴巴位置大约在 x+200 左右
            if (plantX >= zombieX + 20 && plantX <= zombieX + 100) {
                // 僵尸碰到植物，开始吃
                zombie->startEating(plant);
                break;  // 一个僵尸只吃一个植物
            }
        }
    }
}



// ==================== 对象管理 ====================

/**
 * 移除所有死亡对象
 */
void GameManager::removeDeadObjects()
{
    // 移除死亡的植物（从网格中清除）
    for (auto& row : plantGrid) {
        for (auto& plant : row) {
            if (plant && !plant->isAlive()) {
                delete plant;
                plant = nullptr;
            }
        }
    }

    // 移除死亡的僵尸 - 先收集再删除
    std::vector<Zombie*> deadZombies;
    for (auto it = zombies.begin(); it != zombies.end();) {
        if (!(*it)->isAlive() && (*it)->animationFinished) {
            deadZombies.push_back(*it);
            it = zombies.erase(it);
        }
        else {
            ++it;
        }
    }
    for (auto& z : deadZombies) {
        killCount++;  // 每删除一个死亡的僵尸，击杀数+1
        delete z;
    }

    // 移除死亡的子弹 - 先收集再删除
    std::vector<Bullet*> deadBullets;
    for (auto it = bullets.begin(); it != bullets.end();) {
        if (!(*it)->isAlive()) {
            deadBullets.push_back(*it);
            it = bullets.erase(it);
        }
        else {
            ++it;
        }
    }
    for (auto& b : deadBullets) delete b;

    // 移除已收集或消失的阳光 - 先收集再删除
    std::vector<Sunshine*> deadSunshines;
    for (auto it = sunshines.begin(); it != sunshines.end();) {
        if (!(*it)->isAlive()) {
            deadSunshines.push_back(*it);
            it = sunshines.erase(it);
        }
        else {
            ++it;
        }
    }
    for (auto& s : deadSunshines) delete s;
}

// ==================== 种植系统 ====================

/**
 * 在指定位置种植植物
 * @param row 行(0-2)
 * @param col 列(0-8)
 * @param plantType 植物类型（0豌豆，1向日葵，2大嘴花）
 * @return 是否种植成功
 */
bool GameManager::plantAt(int row, int col, int plantType)
{
    // 检查是否在网格范围内
    if (row < 0 || row >= 3 || col < 0 || col >= 9) {
        return false;
    }

    // 检查该位置是否已有植物
    if (plantGrid[row][col] != nullptr) {
        return false;
    }

    // 检查阳光是否足够
    int cost = 0;
    switch (plantType) {
    case 0: cost = 50; break;  // 向日葵
    case 1: cost = 100; break;  // 豌豆射手
    case 2: cost = 150; break; // 大嘴花
    default: return false;
    }

    if (!consumeSunshine(cost)) {
        return false;
    }

    // 创建对应类型的植物
    Plant* plant = nullptr;
    switch (plantType) {
    case 0:
        plant = new Sunflower(row, col);
        break;
    case 1:
        plant = new Peashooter(row, col);
        break;
    case 2:
        plant = new Chomper(row, col);
        break;
    }

    if (plant) {
        plantGrid[row][col] = plant;
        return true;
    }

    return false;
}

// ==================== 植物查询 ====================

Plant* GameManager::getPlantAt(int row, int col) const
{
    if (row < 0 || row >= 3 || col < 0 || col >= 9) {
        return nullptr;
    }
    return plantGrid[row][col];
}

// ==================== 子弹系统 ====================

void GameManager::addBullet(Bullet* bullet)
{
    if (bullet) {
        bullets.push_back(bullet);
    }
}

// ==================== 阳光系统 ====================

void GameManager::addSunshine(Sunshine* sunshine)
{
    if (sunshine) {
        sunshines.push_back(sunshine);
    }
}