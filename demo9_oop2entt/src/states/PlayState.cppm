

//InputHandler::poll() → 采集原始事件
//InputSystem::update(reg, input) → 写入 Navigation/Animation
//MovementSystem::update(reg, dt) → 更新 Transform.position
//AnimationSystem::update(reg, dt) → 推进帧索引，更新 Sprite.texture_id
//RenderSystem::update(reg, render_ctx, camera) → 提交 SDL 绘制调用
//LifecycleSystem::update(reg) → 清理标记实体
//RenderContext::present() → 交换缓冲区

