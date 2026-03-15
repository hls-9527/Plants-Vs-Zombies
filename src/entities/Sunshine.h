#pragma once
#include "GameObject.h"
#include <vector>

/**
 * @file Sunshine.h
 * @brief 阳光实体类
 *
 * 继承自GameObject，实现阳光的特有行为：
 * - 从产生点飘落到地面
 * - 可以被鼠标点击收集
 * - 有动画效果
 * - 存在一段时间后自动消失
 *
 * 原C项目中的阳光系统完全保留：
 * - 贝塞尔曲线飘落轨迹
 * - 收集动画
 * - 自动消失计时
 */
class Sunshine : public GameObject
{
private:
    // === 阳光状态枚举 ===
    enum SunshineState
    {
        STATE_FALLING,     ///< 飘落中
        STATE_GROUND,      ///< 停留在地面
        STATE_COLLECTING   ///< 被收集（飞向阳光计数区）
    };

    SunshineState currentState;  ///< 当前状态
    int row;                     ///< 所在行（如果是向日葵生产的，用于定位）
    int timer;                   ///< 通用计时器
    int groundTimer;             ///< 地面停留计时器
    static const int GROUND_DURATION = 500;  ///< 地面停留时间（帧数）

    // === 贝塞尔曲线相关（用于飘落轨迹）===
    float t;                     ///< 贝塞尔曲线参数 0~1
    int startX, startY;          ///< 起点坐标
    int targetX, targetY;        ///< 终点坐标
    int controlX1, controlY1;    ///< 控制点1
    int controlX2, controlY2;    ///< 控制点2
    float speed;                 ///< 移动速度

    // === 动画相关 ===
    std::vector<IMAGE*> frames;  ///< 阳光动画帧
    static const int ANIMATION_FRAMES = 29;  ///< 阳光动画总帧数（原C项目是29帧）

public:
    /**
     * @brief 构造函数（自然掉落的阳光）
     * @param startX 起始X坐标
     * @param startY 起始Y坐标（通常为60）
     */
    Sunshine(int startX, int startY);

    /**
     * @brief 构造函数（向日葵生产的阳光）
     * @param plantX 向日葵X坐标
     * @param plantY 向日葵Y坐标
     * @param row 所在行
     */
    Sunshine(int plantX, int plantY, int row);

    /**
     * @brief 析构函数
     */
    virtual ~Sunshine();

    /**
     * @brief 更新阳光状态
     * 每帧调用，处理飘落、停留、收集逻辑
     */
    virtual void update() override;

    /**
     * @brief 绘制阳光
     */
    virtual void draw() override;

    /**
     * @brief 判断鼠标是否点击到阳光
     * @param mouseX 鼠标X坐标
     * @param mouseY 鼠标Y坐标
     * @return true 点击到阳光
     */
    bool contains(int mouseX, int mouseY) const;

    /**
     * @brief 收集阳光（被鼠标点击时调用）
     */
    void collect();

    /**
     * @brief 获取阳光所在行（用于碰撞检测）
     * @return 行索引，-1表示自然掉落的阳光
     */
    int getRow() const { return row; }

private:
    /**
     * @brief 加载阳光动画帧
     * 原C项目阳光有29帧动画
     */
    void loadFrames();

    /**
     * @brief 初始化贝塞尔曲线（自然掉落）
     */
    void initFallingCurve();

    /**
     * @brief 初始化贝塞尔曲线（向日葵生产）
     */
    void initProductCurve();

    /**
     * @brief 计算贝塞尔曲线上的点
     * @param t 参数 0~1
     */
    void updateBezierPoint(float t);
};