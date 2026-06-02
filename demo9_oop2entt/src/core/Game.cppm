export module core.game;
import core.render_context;
import core.input_handler;

export namespace core {
    class Game {
        public:
            bool init(const char* title, int w, int h) {
                if (!render_ctx_.init(title, w, h)) return false;
                running_ = true;
                return true;
            }
            void run() {
                while (running_) {
                    handle_events();
                    update();
                    render();
                }
            }
            void shutdown() { render_ctx_.shutdown(); }

        private:
            RenderContext render_ctx_;
            InputHandler  input_;
            bool running_ = false;

            void handle_events() {
                input_.update();
                if (input_.should_quit()) running_ = false;
            }
            void update() {
                // 🔜 Phase 2 预留：registry.update(); 系统调度入口
            }
            void render() {
                render_ctx_.clear();
                // 🔜 Phase 2 预留：RenderSystem::run(registry, render_ctx_.get());
                render_ctx_.present();
            }
    };
}
