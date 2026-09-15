# UI设计

## UI设计主题1
### 表现层 View -- 绘制
- 面板
- 按钮
- 文本
- 图标
- 高亮
- 动画
### 交互事件 Input -- 输入
- 鼠标点击/悬停
- 键盘快捷键
- 拖拽
- 焦点管理
- 输入路由
### 数据层 Object -- 对象
- 当前选中实体
- 当前悬停实体
- 地图格子状态
- 单位行动点
- 资源变化
- 事件通知
### 符号化 Status -- 单位状态
- 生命值: 血条
- 行动点: 小点/条
- 状态异常: debuff图标
- 选中单位: 描边/光圈
- 可攻击: 攻击图标
- 可移动: 范围高亮
[>!NOTE] 高频信息使用视觉符号, 低频细节放到面板
### 顶部资源栏
- 金钱
- 食物
- 木材
- 石材
- 能量
- 人口
- 回合/日期
### 右侧单位详情栏
- 单位头像/立绘
- 名称/职业/阵营
- 生命/护甲/行动点/视野
- 装备/状态
- 当前任务/行为队列
### 底部行动栏
- 移动
- 攻击
- 技能
- 等待
- 取消
- 快捷键提示
### 地图悬停提示
- 格子类型
- 资源
- 通行成本
- 危险程度
- 当前占有者
- 可能结果预估
### 战斗浮字/反馈特效
- 伤害数字-HP
- 闪避Miss
- 暴击Critical
- 治疗+HP
- 资源获取+Items
- 任务完成+Exp

## 优先实现的最小可用 UI 套件
### 地图视图
### 格子高亮
### 单位选择框
### 右侧详情面板
### 底部上下文操作栏
### 顶部资源条
### 悬停提示框
### 事件日志
### 确认弹窗
### 快捷键提示
### 路径预览
### 迷你地图

## 优先级划分
### Phase1 核心可玩
- 地图视图
- 选择/悬停
- 单位详情
- 操作栏
- 资源条
### Phase2 可读性和反馈
- 路径预览
- 战斗浮字
- 事件日志
- 快捷键提示
- 状态图标
### Phase3 复杂系统支持
- 科技树
- 生产队列
- 贸易面板
- 任务追踪
- 过滤搜索
### Phase4 高级体验
- 迷你地图
- 批量操作
- 拖拽编组
- 高级筛选
- 可配置HUD

## UI架构
### 基本需求
- UI 与游戏模拟解耦
- 基于事件驱动，不要每帧全量刷新
- 支持高性能局部更新
- 支持复杂策略游戏的多面板布局
- 方便后续扩展、调试、热更新
### 整体架构图 -- 5层
#### L1 Player Input
- mouse
- keyboard
- controller
- hotkeys
#### L2 UI Input Router
- hit test: 鼠标落在哪个控件
- foucus: 当前键盘的焦点在哪
- capture: 拖拽/按住时由谁独占输入
- modal routing: 弹窗打开时, 其他UI是否被阻断
#### L3 UI State Layer
- 选择: 单位
- 悬浮: 对象
- 拖拽: 激活面板
- 模态: 打开菜单
- 面板状态
- 脏标记
- 缓存布局
- 动画状态
#### L4 ECS/Game State/UI Events
UI Model/View Model模块
职责: 将ECS中复杂数据整理成UI能直接使用的结构
选中单位后生成: 名称,头像,当前生命/最大生命,当前行动点,状态列表,可用动作列表,预估伤害,详细描述文本
UI Widgets模块
常见控件: Button, Label, Panel, TabBar, Tooltip, ListView, ProgressBar, Icon, GridOverlay, Minimap, ContextActionBar, StatusIconStrip
控件尽量具备: Update(), Render(), HandleEvent(), SetVisible(), SetDirty(), Layout()
- EnTT registry
- dispatcher
- signals
- componets
- selection changed
- hover changed
- status changed
#### L5 Rendering Layer
建议分层: 地图层, 单位层, 高亮层, UI 面板层, Tooltip / 弹窗层, 特效层
- SDL_Renderer
- texture cache
- text cache
- atlas
- map render
- UI render
- effects render

### EnTT的作用
- 游戏实体层
- UI相关实体层
- 事件系统

