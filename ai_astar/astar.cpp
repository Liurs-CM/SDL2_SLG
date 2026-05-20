// astar.cpp (修正版)

#include "astar.h"
#include <unordered_map> // 需要包含这个头文件
#include <limits> // 别忘了这个头文件

// --- 成员函数定义必须在全局作用域 ---
float AStar::heuristic(const Point& a, const Point& b) const {
    return static_cast<float>(std::max(std::abs(a.x - b.x), std::abs(a.y - b.y)));
}

float AStar::oldHeuristic(const Point& a, const Point& b) const {
    int vec_x = a.x - b.x;
    int vec_y = a.y - b.y;
    int vec_1_0 = std::abs(vec_x - vec_y);
    int vec_1_1 = std::abs(vec_y);
    float distance = 2 * vec_1_0 + 3 * vec_1_1;
    return distance;
}

bool AStar::isInBounds(const Point& p) const {
    return p.x >= 0 && p.x < width_ && p.y >= 0 && p.y < height_;
}

bool AStar::isObstacle(const Point& p, const std::vector<std::vector<bool>>& obstacles) const {
    return obstacles[p.y][p.x];
}


std::optional<std::vector<Point>> AStar::findPath(
        const Point& start, 
        const Point& goal, 
        const std::vector<std::vector<bool>>& obstacles
        ) {
    // 如果起点或终点是障碍物，直接返回无路径
    if (isObstacle(start, obstacles) || isObstacle(goal, obstacles)) {
        return std::nullopt;
    }

    // 如果起点就是终点
    if (start == goal) {
        return std::vector<Point>{start};
    }

    // 优先队列
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openSet;

    // 已探索集合
    std::unordered_set<Point, PointHash> closedSet;

    // **关键修复：用于记录每个点的父节点**
    std::unordered_map<Point, Point, PointHash> parentMap;

    // G值记录，用于更新更优路径
    std::unordered_map<Point, float, PointHash> gScore;
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            gScore[Point(x, y)] = std::numeric_limits<float>::max();
        }
    }
    gScore[start] = 0.0f;

    openSet.push({start, 0.0f, heuristic(start, goal), start});

    while (!openSet.empty()) {
        Node current = openSet.top();
        openSet.pop();

        if (closedSet.find(current.point) != closedSet.end()) {
            continue;
        }

        if (current.point == goal) {
            // **正确回溯路径**
            std::vector<Point> path;
            Point current = goal;
            while (current != start) {
                path.push_back(current);
                current = parentMap[current]; // 从 map 中获取父节点
            }
            path.push_back(start);
            std::reverse(path.begin(), path.end());
            return path;
        }

        closedSet.insert(current.point);

        // 探索8个邻居
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                if (dx == 0 && dy == 0) continue;

                Point neighbor(current.point.x + dx, current.point.y + dy);

                if (!isInBounds(neighbor) || isObstacle(neighbor, obstacles)) {
                    continue;
                }

                float tentativeGScore = gScore[current.point] + ((dx == 0 || dy == 0) ? 1.0f : std::sqrt(2.0f));

                // **只有当找到更短的路径时才更新**
                if (tentativeGScore < gScore[neighbor]) {
                    parentMap[neighbor] = current.point; // 记录父节点
                    gScore[neighbor] = tentativeGScore;
                    float fScore = tentativeGScore + heuristic(neighbor, goal);
                    openSet.push({neighbor, tentativeGScore, heuristic(neighbor, goal), current.point});
                }
            }
        }
    }

    return std::nullopt; // 未找到路径
}
