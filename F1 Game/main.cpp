#include <SDL3/SDL.h>
#include <iostream>
#include <vector>

// ------------------------------------------------------------
// Simple beginner SDL3 2D racing game
// OOP style + lots of comments
// ------------------------------------------------------------

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 700;

// ============================================================
// Car class
// This class stores the player car data.
// ============================================================
class Car {
private:
    float x;
    float y;
    float w;
    float h;

    float speed;
    float maxSpeed;
    float accel;
    float friction;
    float moveX;

    SDL_Color color;

public:
    Car(float startX, float startY, SDL_Color carColor) {
        x = startX;
        y = startY;
        w = 30;
        h = 50;

        speed = 0.0f;
        maxSpeed = 8.0f;
        accel = 0.25f;
        friction = 0.10f;
        moveX = 5.0f;

        color = carColor;
    }

    // Read keyboard state and change the car values.
    void handleInput(const bool* keys) {
        // W = go up
        if (keys[SDL_SCANCODE_W]) {
            speed -= accel;
        }

        // S = go down / brake
        if (keys[SDL_SCANCODE_S]) {
            speed += accel;
        }

        // A = move left
        if (keys[SDL_SCANCODE_A]) {
            x -= moveX;
        }

        // D = move right
        if (keys[SDL_SCANCODE_D]) {
            x += moveX;
        }

        // Keep speed inside limit
        if (speed < -maxSpeed) speed = -maxSpeed;
        if (speed > maxSpeed) speed = maxSpeed;
    }

    // Update car position every frame.
    void update() {
        y += speed;

        // Friction slowly stops the car
        if (speed > 0) {
            speed -= friction;
            if (speed < 0) speed = 0;
        }
        else if (speed < 0) {
            speed += friction;
            if (speed > 0) speed = 0;
        }

        // Keep player inside the road area
        if (x < 210) x = 210;
        if (x + w > 790) x = 790 - w;
        if (y < 0) y = 0;
        if (y + h > SCREEN_HEIGHT) y = SCREEN_HEIGHT - h;
    }

    // Draw the car
    void render(SDL_Renderer* renderer) {
        SDL_FRect body = { x, y, w, h };
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer, &body);

        // Cockpit
        SDL_FRect cockpit = { x + 8, y + 10, 14, 18 };
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &cockpit);
    }

    SDL_FRect getRect() const {
        SDL_FRect rect = { x, y, w, h };
        return rect;
    }

    void reset(float startX, float startY) {
        x = startX;
        y = startY;
        speed = 0;
    }
};

// ============================================================
// Obstacle class
// This is used for enemy cars.
// ============================================================
class Obstacle {
private:
    float x;
    float y;
    float w;
    float h;
    float speed;
    SDL_Color color;

public:
    Obstacle(float startX, float startY, float width, float height, float moveSpeed, SDL_Color obstacleColor) {
        x = startX;
        y = startY;
        w = width;
        h = height;
        speed = moveSpeed;
        color = obstacleColor;
    }

    void update() {
        y += speed;

        // If enemy leaves bottom of screen, send it back to top
        if (y > SCREEN_HEIGHT) {
            y = -h;
        }
    }

    void render(SDL_Renderer* renderer) {
        SDL_FRect rect = { x, y, w, h };
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer, &rect);
    }

    SDL_FRect getRect() const {
        SDL_FRect rect = { x, y, w, h };
        return rect;
    }
};

// ============================================================
// Helper function for collision
// This is easier for beginners than using an SDL collision helper.
// ============================================================
bool checkCollision(const SDL_FRect& a, const SDL_FRect& b) {
    if (a.x + a.w <= b.x) return false;
    if (a.x >= b.x + b.w) return false;
    if (a.y + a.h <= b.y) return false;
    if (a.y >= b.y + b.h) return false;
    return true;
}

// ============================================================
// Game class
// Controls the whole game.
// ============================================================
class Game {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;

    Car player;
    std::vector<Obstacle> obstacles;
    std::vector<float> roadLinesY;

public:
    Game()
        : player(SCREEN_WIDTH / 2.0f - 15.0f, SCREEN_HEIGHT - 120.0f, {255, 0, 0, 255}) {
        window = nullptr;
        renderer = nullptr;
        running = false;
    }

    bool init() {
        // Start SDL video
        if (!SDL_Init(SDL_INIT_VIDEO)) {
            std::cout << "SDL_Init failed: " << SDL_GetError() << std::endl;
            return false;
        }

        // Create window and renderer together
        if (!SDL_CreateWindowAndRenderer("Simple SDL3 Racing Game", SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer)) {
            std::cout << "SDL_CreateWindowAndRenderer failed: " << SDL_GetError() << std::endl;
            return false;
        }

        // Optional: turn on VSync so the game looks smoother
        SDL_SetRenderVSync(renderer, 1);

        // Add enemy cars
        obstacles.push_back(Obstacle(280, 60, 30, 50, 3.0f, {0, 0, 255, 255}));
        obstacles.push_back(Obstacle(470, 220, 30, 50, 4.0f, {255, 255, 0, 255}));
        obstacles.push_back(Obstacle(650, 120, 30, 50, 2.5f, {0, 255, 0, 255}));

        // Make dashed center road lines
        for (int i = 0; i < 10; i++) {
            roadLinesY.push_back(i * 80.0f);
        }

        running = true;
        return true;
    }

    void handleEvents() {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }
    }

    void update() {
        // Get keyboard state from SDL
        const bool* keys = SDL_GetKeyboardState(NULL);

        player.handleInput(keys);
        player.update();

        for (size_t i = 0; i < obstacles.size(); i++) {
            obstacles[i].update();
        }

        // Move road lines to create motion effect
        for (size_t i = 0; i < roadLinesY.size(); i++) {
            roadLinesY[i] += 6.0f;
            if (roadLinesY[i] > SCREEN_HEIGHT) {
                roadLinesY[i] = -40.0f;
            }
        }

        // Check collisions
        SDL_FRect playerRect = player.getRect();
        for (size_t i = 0; i < obstacles.size(); i++) {
            if (checkCollision(playerRect, obstacles[i].getRect())) {
                std::cout << "Crash! Player reset.\n";
                player.reset(SCREEN_WIDTH / 2.0f - 15.0f, SCREEN_HEIGHT - 120.0f);
                break;
            }
        }
    }

    void drawRoad() {
        // Grass background
        SDL_SetRenderDrawColor(renderer, 20, 140, 20, 255);
        SDL_RenderClear(renderer);

        // Main road
        SDL_FRect road = { 200, 0, 600, (float)SCREEN_HEIGHT };
        SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
        SDL_RenderFillRect(renderer, &road);

        // Side borders
        SDL_FRect leftBorder = { 200, 0, 8, (float)SCREEN_HEIGHT };
        SDL_FRect rightBorder = { 792, 0, 8, (float)SCREEN_HEIGHT };
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &leftBorder);
        SDL_RenderFillRect(renderer, &rightBorder);

        // Center dashed lines
        for (size_t i = 0; i < roadLinesY.size(); i++) {
            SDL_FRect line = { 495, roadLinesY[i], 10, 40 };
            SDL_RenderFillRect(renderer, &line);
        }
    }

    void render() {
        drawRoad();

        // Draw enemies
        for (size_t i = 0; i < obstacles.size(); i++) {
            obstacles[i].render(renderer);
        }

        // Draw player
        player.render(renderer);

        // Show everything on screen
        SDL_RenderPresent(renderer);
    }

    void run() {
        while (running) {
            handleEvents();
            update();
            render();
        }
    }

    void clean() {
        if (renderer != nullptr) {
            SDL_DestroyRenderer(renderer);
            renderer = nullptr;
        }

        if (window != nullptr) {
            SDL_DestroyWindow(window);
            window = nullptr;
        }

        SDL_Quit();
    }
};

// ============================================================
// main
// Program starts here.
// ============================================================
int main(int argc, char* argv[]) {
    Game game;

    if (!game.init()) {
        std::cout << "Game failed to start.\n";
        return 1;
    }

    game.run();
    game.clean();

    return 0;
}
