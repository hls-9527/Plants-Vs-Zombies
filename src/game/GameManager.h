#pragma once
#include <vector>
#include <graphics.h>
#include "../entities/GameObject.h"

// 前向声明（避免循环包含）
class Plant;
class Zombie;
class Bullet;
class Sunshine;
class Shovel;

/**
 * @file GameManager.h
 * @brief 游戏核心管理器类
 *
 * 这是游戏的大脑，负责：
 * 1. 管理所有游戏对象（植物、僵尸、子弹、阳光）
 * 2. 驱动游戏主循环
 * 3. 处理鼠标点击事件
 * 4. 碰撞检测
 * 5. 游戏状态控制
 *
 * 采用单例模式，确保全局只有一个游戏管理器
 */
class GameManager
{
private:
    // 私有构造函数（单例模式）
    GameManager();
    ~GameManager();

    // 禁止拷贝
    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;

    // === 游戏状态枚举 ===
    enum GameStatus
    {
        MENU,       ///< 主菜单界面
        PLAYING,    ///< 游戏进行中
        WIN,        ///< 通关胜利
        FAIL        ///< 游戏失败
    };

    // === 核心属性 ===
    GameStatus currentStatus;           ///< 当前游戏状态
    int sunshineCount;                  ///< 当前阳光数量
    int currentLevel;                    ///< 当前关卡
    int frameCount;                      ///< 帧计数器（用于控制生成频率）
    static const int WIN_CONDITION = 100;  ///< 胜利所需击杀僵尸数量
    int killCount;                         ///< 当前击杀僵尸数量（需要在构造函数初始化）
    static const int TOTAL_ZOMBIES = 100;  ///< 总僵尸数量
    int zombieCount;                       ///< 已生成的僵尸数量（需要在构造函数初始化）
    IMAGE* followImage;     ///< 鼠标跟随的植物图片
    int followX, followY;   ///< 跟随图片的绘制位置
    bool showGoal;  ///< 是否显示通关目标（第一只僵尸出现后设为true）

    // === 对象容器 ===
    // 植物网格：3行9列，存储植物指针
    std::vector<std::vector<Plant*>> plantGrid;

    // 动态对象容器（使用基类指针实现多态）
    std::vector<Zombie*> zombies;        ///< 所有僵尸
    std::vector<Bullet*> bullets;        ///< 所有子弹
    std::vector<Sunshine*> sunshines;    ///< 所有阳光

    // 铲子（单独管理）
    Shovel* shovel;                       ///< 铲子对象

public:
    /**
     * @brief 获取单例实例
     * @return GameManager单例指针
     */
    static GameManager* getInstance();

    /**
     * @brief 初始化游戏
     * @param fromMenu 是否从菜单进入（true:播放开场动画，false:直接开始）
     */
    void init(bool fromMenu = true);

    /**
     * @brief 更新所有游戏对象
     * 主循环中每帧调用一次
     */
    void update();

    /**
     * @brief 绘制所有游戏对象
     * 主循环中每帧调用一次
     */
    void draw();

    /**
     * @brief 处理鼠标点击事件
     * @param mouseX 鼠标X坐标
     * @param mouseY 鼠标Y坐标
     */
    void handleClick(int mouseX, int mouseY);

    /**
     * @brief 处理鼠标移动事件（用于铲子跟随）
     * @param mouseX 鼠标X坐标
     * @param mouseY 鼠标Y坐标
     */
    void handleMouseMove(int mouseX, int mouseY);

    /**
     * @brief 处理鼠标右键点击（用于取消铲子）
     */
    void handleRightClick();

    // === 游戏状态查询 ===
    bool isGameOver() const { return currentStatus == WIN || currentStatus == FAIL; }
    bool isPlaying() const { return currentStatus == PLAYING; }
    int getStatus() const { return currentStatus; }
    int getKillCount() const { return killCount; }

    // === 阳光系统 ===
    int getSunshine() const { return sunshineCount; }
    void addSunshine(int amount);
    bool consumeSunshine(int amount);
    void addSunshine(Sunshine* sunshine);

    // === 子弹系统 ===
    void addBullet(Bullet* bullet);

    // === 植物种植 ===
    /**
     * @brief 在指定位置种植植物
     * @param row 行(0-2)
     * @param col 列(0-8)
     * @param plantType 植物类型（0豌豆，1向日葵，2大嘴花）
     * @return 是否种植成功
     */
    bool plantAt(int row, int col, int plantType);

    // === 植物查询 ===
    /**
     * @brief 获取指定位置的植物指针
     * @param row 行索引
     * @param col 列索引
     * @return 植物指针，如果没有植物返回nullptr
     */
    Plant* getPlantAt(int row, int col) const;

    // === 僵尸查询 ===
    const std::vector<Zombie*>& getZombies() const { return zombies; }

    // 安全获取植物指针（带边界检查）
    Plant* getPlantAtSafe(int row, int col) const {
        if (row < 0 || row >= 3 || col < 0 || col >= 9) {
            return nullptr;
        }
        return plantGrid[row][col];
    }

    // 安全设置植物指针（带边界检查）
    bool setPlantAtSafe(int row, int col, Plant* plant) {
        if (row < 0 || row >= 3 || col < 0 || col >= 9) {
            return false;
        }
        plantGrid[row][col] = plant;
        return true;
    }

private:
    // === 私有方法（内部逻辑）===

    int selectedPlantType;  ///< 当前选中的植物类型：-1未选中，0豌豆，1向日葵，2大嘴花

    /**
     * @brief 创建新僵尸
     * 根据随机数和计时器决定是否生成
     */
    void createZombie();

    /**
     * @brief 创建阳光（自然掉落）
     */
    void createSunshine();

    /**
     * @brief 检查所有碰撞
     * 包括：子弹vs僵尸、僵尸vs植物
     */
    void checkCollisions();

    /**
     * @brief 移除所有死亡对象
     * 遍历容器，删除标记为死亡的对象
     */
    void removeDeadObjects();

    /**
     * @brief 播放开场动画
     * 包括：背景滚动、僵尸站立动画、卡片栏下落
     */
    void playIntroAnimation();

    /**
     * @brief 根据坐标计算网格位置
     * @param x 鼠标X坐标
     * @param y 鼠标Y坐标
     * @param row 输出：行索引
     * @param col 输出：列索引
     * @return 是否在网格范围内
     */
    bool getGridIndexFromPos(int x, int y, int& row, int& col);
};