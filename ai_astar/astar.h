#ifndef ASTAR_H
#define ASTAR_H

#include <vector>
#include <queue>
#include <unordered_set>
#include <optional>
#include <cmath>
#include <algorithm>

// 定义地图上的一个点
struct Point {
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}

    // 重载 == 运算符，以便在 unordered_set 中使用
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
    // --- 新增：重载 != 运算符 ---
    bool operator!=(const Point& other) const {
        return !(*this == other); // 直接复用 operator==
    }
};

// 为 Point 定义哈希函数，使其能作为 unordered_set 的键
struct PointHash {
    std::size_t operator()(const Point& p) const {
        return std::hash<int>()(p.x) ^ std::hash<int>()(p.y);
    }
};

// A* 算法类
class AStar {
    public:
        // 构造函数，传入地图的宽和高
        AStar(int width, int height) : width_(width), height_(height) {}

        // 寻路主函数
        // 返回一个包含路径点的 vector，如果找不到路径则返回 empty optional
        std::optional<std::vector<Point>> findPath(
                const Point& start, 
                const Point& goal, 
                const std::vector<std::vector<bool>>& obstacles
                );

    private:
        // 内部节点结构，用于优先队列
        struct Node {
            Point point;
            float gCost; // 从起点到当前点的实际代价
            float hCost; // 从当前点到终点的启发式代价
            Point parent; // 父节点，用于回溯路径

            // 重载 > 运算符，使优先队列成为最小堆
            bool operator>(const Node& other) const {
                return (gCost + hCost) > (other.gCost + other.hCost);
            }
        };

        int width_, height_;

        // 计算启发式代价 H (使用对角线距离)
        float heuristic(const Point& a, const Point& b) const;

        // 检查点是否在地图范围内
        bool isInBounds(const Point& p) const;

        // 检查点是否是障碍物
        bool isObstacle(const Point& p, const std::vector<std::vector<bool>>& obstacles) const;
};

#endif // ASTAR_H
