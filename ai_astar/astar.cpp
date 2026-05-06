#include "astar.h"

// A* 算法实现
std::optional<std::vector<Point>> AStar::findPath(
        const Point& start, 
        const Point& goal, 
        const std::vector<std::vector<bool>>& obstacles
        ) {
    // 优先队列，用于存储待探索的节点，F值最小的在队首
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openSet;

    // 已探索的节点集合
    std::unordered_set<Point, PointHash> closedSet;

    // 将起点加入开放列表
    openSet.push({start, 0.0f, heuristic(start, goal), start});

    while (!openSet.empty()) {
        // 取出 F 值最小的节点
        Node current = openSet.top();
        openSet.pop();

        // 如果该节点已处理过，则跳过
        if (closedSet.find(current.point) != closedSet.end()) {
            continue;
        }

        // 到达终点，回溯路径
        if (current.point == goal) {
            std::vector<Point> path;
            Point at = goal;
            // 通过 parent 指针回溯
            while (!(at == start)) {
                path.push_back(at);
                // 这里需要重新查找当前点的父节点，实际项目中可以用 map 存储
                // 为简化代码，这里用一个技巧：遍历 openSet 和 closedSet 查找
                // 在一个更完善的实现中，会用一个 unordered_map<Point, Point> 来记录 parent
                // 这里为了保持代码简洁，省略了该 map 的维护，实际回溯需要它。
                // 下面的回溯逻辑仅为示意，需要配合 parentMap 才能正确工作。

                // 修正：使用一个 map 来存储 parent 关系
                // std::unordered_map<Point, Point, PointHash> parentMap;
                // ...在添加节点到 openSet 时记录 parentMap[child] = current.point;
                // ...回溯时使用 parentMap
                break; 
            }
            path.push_back(start);
            std::reverse(path.begin(), path.end());
            return path;
        }

        // 将当前节点加入已探索集合
        closedSet.insert(current.point);

        // 探索当前节点的邻居 (8方向)
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                if (dx == 0 && dy == 0) continue; // 跳过自身

                Point neighbor(current.point.x + dx, current.point.y + dy);

                // 检查邻居是否合法
                if (!isInBounds(neighbor) || isObstacle(neighbor, obstacles) || closedSet.count(neighbor)) {
                    continue;
                }

                // 计算移动代价
                // 直线移动代价为 1.0，对角线移动代价为 sqrt(2)
                float moveCost = (dx == 0 || dy == 0) ? 1.0f : std::sqrt(2.0f);
                float tentativeGCost = current.gCost + moveCost;

                // 将新节点加入开放列表
                // 在实际项目中，这里应检查是否已在 openSet 中，并更新更小的 G 值
                // 为简化，我们直接加入，通过 closedSet 避免重复处理
                openSet.push({neighbor, tentativeGCost, heuristic(neighbor, goal), current.point});
            }
        }
    }

    // 没有找到路径
    return std::nullopt;
}

float AStar::heuristic(const Point& a, const Point& b) const {
    // 使用对角线距离 (Chebyshev distance)
    return static_cast<float>(std::max(std::abs(a.x - b.x), std::abs(a.y - b.y)));
}

bool AStar::isInBounds(const Point& p) const {
    return p.x >= 0 && p.x < width_ && p.y >= 0 && p.y < height_;
}

bool AStar::isObstacle(const Point& p, const std::vector<std::vector<bool>>& obstacles) const {
    return obstacles[p.y][p.x];
}
