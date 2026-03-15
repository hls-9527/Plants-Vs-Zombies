#pragma once
#include "Plant.h"

/**
 * @file Sunflower.h
 * @brief 向日葵类
 *
 * 继承自Plant，实现向日葵的特有行为：
 * - 定期生产阳光
 * - 有生产间隔
 * - 阳光从向日葵位置产生并飘落
 */
class Sunflower : public Plant
{
private:
    int produceTimer;           ///< 生产阳光计时器
    static const int PRODUCE_INTERVAL = 2000;  ///< 生产间隔（帧数）

public:
    /**
     * @brief 构造函数
     * @param row 行索引
     * @param col 列索引
     */
    Sunflower(int row, int col);

    /**
     * @brief 析构函数
     */
    virtual ~Sunflower();

    /**
     * @brief 更新向日葵状态
     * 每帧调用，处理生产阳光逻辑
     */
    virtual void update() override;

    /**
     * @brief 绘制向日葵
     */
    virtual void draw() override;

    /**
     * @brief 生产阳光
     * 创建一个阳光对象并添加到游戏管理器的阳光列表
     */
    void produceSunshine();
};