#include <SDL2/SDL.h>
#include <iostream>
#include <functional>
#include <map>
#include <utility>

// ============ 1. 状态与事件定义 ============
enum class GameState {
    Menu,
    Playing,
    Pause,
    Quit,
    GameOver
};

enum class GameEvent {
    StartGame,
    ExitGame,
    PauseGame,
    ResumeGame,
    PlayerDied,
    Restart,
    None
};

// ============ 2. 动作回调（Entry / Exit） ============
using Action = std::function<void()>;

void onEnterMenu() {
    std::cout << "[ACTION] Entering Menu\n";
    // 可初始化菜单 UI、播放背景音乐等
}

void onExitMenu() {
    std::cout << "[ACTION] Exiting Menu\n";
}

void onEnterPlaying() {
    std::cout << "[ACTION] Starting Gameplay\n";
    // 重置玩家、生成地图等
}

void onExitPlaying() {
    std::cout << "[ACTION] Stopping Gameplay\n";
}

void onEnterGameOver() {
    std::cout << "[ACTION] Game Over Screen\n";
}

void onExitGameOver() {
    std::cout << "[ACTION] Leaving Game Over\n";
}

// ============ 3. 状态机核心：查表驱动 ============
class GlobalStateMachine {
    public:
        // 构造时初始化转移表和动作表
        GlobalStateMachine() {
            // --- 状态转移表: (current, event) -> next ---
            transitionTable = {
                {{GameState::Menu,     GameEvent::StartGame}, GameState::Playing},
                {{GameState::Menu,     GameEvent::ExitGame},    GameState::Quit},
                {{GameState::Pause,    GameEvent::ResumeGame},    GameState::Quit},
                {{GameState::Pause,    GameEvent::ResumeGame},    GameState::Quit},
                {{GameState::Playing,  GameEvent::PlayerDied}, GameState::GameOver},
                {{GameState::GameOver, GameEvent::Restart},    GameState::Menu}
            };

            // --- 进入动作表 ---
            entryActions = {
                {GameState::Menu,      onEnterMenu},
                {GameState::Playing,   onEnterPlaying},
                {GameState::GameOver,  onEnterGameOver}
            };

            // --- 退出动作表 ---
            exitActions = {
                {GameState::Menu,      onExitMenu},
                {GameState::Playing,   onExitPlaying},
                {GameState::GameOver,  onExitGameOver}
            };
        }

        // 外部触发事件（相当于 Verilog 的 input）
        void dispatch(GameEvent event) {
            if (event == GameEvent::None) return;

            auto it = transitionTable.find({currentState, event});
            GameState nextState = (it != transitionTable.end()) ? it->second : currentState;

            if (nextState != currentState) {
                // 执行退出动作（当前状态）
                if (auto exitIt = exitActions.find(currentState); exitIt != exitActions.end()) {
                    exitIt->second();
                }

                // 原子更新状态（Verilog: state <= next_state）
                currentState = nextState;

                // 执行进入动作（新状态）
                if (auto enterIt = entryActions.find(currentState); enterIt != enterActions.end()) {
                    enterIt->second();
                }
            }
        }

        GameState getCurrentState() const { return currentState; }

    private:
        GameState currentState = GameState::Menu;

        // 查表：无 if-else！
        std::map<std::pair<GameState, GameEvent>, GameState> transitionTable;
        std::map<GameState, Action> entryActions;
        std::map<GameState, Action> exitActions;
};

// ============ 4. SDL2 主循环集成 ============
int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow(
            "State Machine Demo",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            800, 600,
            SDL_WINDOW_SHOWN
            );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    bool running = true;
    SDL_Event e;

    GlobalStateMachine fsm;

    // 简单帧率控制
    const int FPS = 60;
    const int FRAME_DELAY = 1000 / FPS;
    Uint32 frameStart;

    while (running) {
        frameStart = SDL_GetTicks();

        // --- 处理输入（生成事件）---
        GameEvent currentEvent = GameEvent::None;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            } else if (e.type == SDL_KEYDOWN) {
                switch (fsm.getCurrentState()) {
                    case GameState::Menu:
                        if (e.key.keysym.sym == SDLK_RETURN) currentEvent = GameEvent::StartGame;
                        break;
                    case GameState::Playing:
                        if (e.key.keysym.sym == SDLK_x) currentEvent = GameEvent::PlayerDied;
                        break;
                    case GameState::GameOver:
                        if (e.key.keysym.sym == SDLK_r) currentEvent = GameEvent::Restart;
                        break;
                }
            }
        }

        // --- 状态机更新（Verilog 风格同步更新）---
        fsm.dispatch(currentEvent);

        // --- 渲染（根据当前状态）---
        SDL_SetRenderDrawColor(renderer, 30, 30, 50, 255);
        SDL_RenderClear(renderer);

        // 无 if-else 渲染？也可用查表，但此处为清晰使用 switch（仅用于渲染）
        switch (fsm.getCurrentState()) {
            case GameState::Menu:
                SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
                break;
            case GameState::Playing:
                SDL_SetRenderDrawColor(renderer, 0, 0, 200, 255);
                break;
            case GameState::GameOver:
                SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
                break;
        }
        SDL_RenderFillRect(renderer, nullptr); // 填充全屏

        SDL_RenderPresent(renderer);

        // 帧率控制
        int frameTime = SDL_GetTicks() - frameStart;
        if (FRAME_DELAY > frameTime) {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
