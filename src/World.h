#pragma once

#include <iostream>
#include <vector>

#include "TextureManager.h"

struct position
{
    float x, y;

    friend position operator+(position const& p1, position const& p2);
    friend position operator-(position const& p1, position const& p2);
    friend position operator*(position const& p, float k);
    friend position operator*(float k, position const& p);
};

struct world_position
{
    int x, y;
};

enum color
{
    RED,
    PINK,
    BLUE,
    ORANGE
};

class World
{
public:

    World();
    ~World();

    void loadDetails(SDL_Renderer* renderer);
    void render(SDL_Renderer* renderer);
    void renderScore(SDL_Renderer* renderer);

    void setScorePos(int x, int y) { m_score_pos.x = x; m_score_pos.y = y; }

    int getPoints() { return m_points; }

    bool isWall(int y, int x, bool fence_on);
    bool isWall(position pos, bool fence_on);

    bool playerEaten() const { return m_player_eaten; }

    bool redVulnerable() const { return m_red_vulnerable; }
    bool pinkVulnerable() const { return m_pink_vulnerable; }
    bool blueVulnerable() const { return m_blue_vulnerable; }
    bool orangeVulnerable() const { return m_orange_vulnerable; }

    bool redEaten() const { return m_red_eaten; }
    bool pinkEaten() const { return m_pink_eaten; }
    bool blueEaten() const { return m_blue_eaten; }
    bool orangeEaten() const { return m_orange_eaten; }

    bool bigPointEaten() const { return m_big_point_eaten; }

    char charAt(int y, int x) const { return m_map[y][x]; }

    void setCharAt(int y, int x, char c) { m_map[y][x] = c; }
    void togglePlayerEatenFlag() { m_player_eaten = !m_player_eaten; }

    char getGhostBuffer(int index) const{ return m_ghost_buffer[index]; }
    void setGhostBuffer(char c, int index);
    void setAllGhostsVulnerable();

    void toggleRedVulnerableFlag() { m_red_vulnerable = !m_red_vulnerable; }
    void togglePinkVulnerableFlag() { m_pink_vulnerable = !m_pink_vulnerable; }
    void toggleBlueVulnerableFlag() { m_blue_vulnerable = !m_blue_vulnerable; }
    void toggleOrangeVulnerableFlag() { m_orange_vulnerable = !m_orange_vulnerable; }

    void toggleRedEatenFlag() { m_red_eaten = !m_red_eaten; }
    void togglePinkEatenFlag() { m_pink_eaten = !m_pink_eaten; }
    void toggleBlueEatenFlag() { m_blue_eaten = !m_blue_eaten; }
    void toggleOrangeEatenFlag() { m_orange_eaten = !m_orange_eaten; }

    void toggleBigPointEatenFlag() { m_big_point_eaten = !m_big_point_eaten; }

    void toggleRenderScoreFlag() { m_render_score = !m_render_score; }

    void updatePlayerPos(int old_y, int old_x, int new_y, int new_x);
    void resetPlayer();
    void resetGhost();

    void printMap()
    {
        for (int i = 0; i < 28; i++)
        {
            for (int j = 0; j < 23; j++)
            {
                std::cout << m_map[i][j];
            }
            std::cout << '\n';
        }
    }

private:
    SDL_Rect m_src{ 0, 0, 32, 32 };
    SDL_Rect m_dest{ 0, 0, 32, 32 };
    SDL_Rect area_rect{ 0, 0, 736, 896 };

    SDL_Texture* m_area_tex{ nullptr };
    SDL_Texture* m_point_tex{ nullptr };
    SDL_Texture* m_big_point_tex{ nullptr };
    SDL_Texture* m_score_tex{ nullptr };
    SDL_Texture* m_berry_tex{ nullptr };

    int m_big_points{ 0 };
    int m_points{ 0 };

    world_position m_score_pos{ 0,0 };

    std::vector<char> m_ghost_buffer{ ' ',' ',' ',' ' }; // For storing points that collide with ghosts

    bool m_red_vulnerable{ false };
    bool m_pink_vulnerable{ false };
    bool m_blue_vulnerable{ false };
    bool m_orange_vulnerable{ false };

    bool m_red_eaten{ false };
    bool m_pink_eaten{ false };
    bool m_blue_eaten{ false };
    bool m_orange_eaten{ false };

    bool m_player_eaten{ false };
    bool m_big_point_eaten{ false };
    bool m_render_score{ false };

    char m_map[28][23] = {
        {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
        {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},
        {'#','*','*','*','*','*','*','*','*','*','*','#','*','*','*','*','*','*','*','*','*','*','#'},
        {'#','@','#','#','#','*','#','#','#','#','*','#','*','#','#','#','#','*','#','#','#','@','#'},
        {'#','*','#','#','#','*','#','#','#','#','*','#','*','#','#','#','#','*','#','#','#','*','#'},
        {'#','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','#'},
        {'#','*','#','#','#','*','#','*','#','#','#','#','#','#','#','*','#','*','#','#','#','*','#'},
        {'#','*','*','*','*','*','#','*','#','#','#','#','#','#','#','*','#','*','*','*','*','*','#'},
        {'#','#','#','#','#','*','#','*','#','#','#','#','#','#','#','*','#','*','#','#','#','#','#'},
        {'#','#','#','#','#','*','#','*','*','*','*','#','*','*','*','*','#','*','#','#','#','#','#'},
        {'#','#','#','#','#','*','#','#','#','#',' ','#',' ','#','#','#','#','*','#','#','#','#','#'},
        {'#','#','#','#','#','*','#',' ',' ',' ',' ',' ',' ',' ',' ',' ','#','*','#','#','#','#','#'},
        {'#','#','#','#','#','*','#',' ','#','#','#','-','#','#','#',' ','#','*','#','#','#','#','#'},
        {' ',' ',' ',' ',' ','*',' ',' ','#','B','P',' ','R','O','#',' ',' ','*',' ','$',' ',' ',' '},
        {'#','#','#','#','#','*','#',' ','#','#','#','#','#','#','#',' ','#','*','#','#','#','#','#'},
        {'#','#','#','#','#','*','#',' ',' ',' ',' ',' ',' ',' ',' ',' ','#','*','#','#','#','#','#'},
        {'#','#','#','#','#','*','#',' ','#','#','#','#','#','#','#',' ','#','*','#','#','#','#','#'},
        {'#','#','#','#','#','*','#',' ','#','#','#','#','#','#','#',' ','#','*','#','#','#','#','#'},
        {'#','*','*','*','*','*','*','*','*','*','*','#','*','*','*','*','*','*','*','*','*','*','#'},
        {'#','*','#','#','#','*','#','#','#','#','*','#','*','#','#','#','#','*','#','#','#','*','#'},
        {'#','@','*','*','#','*','*','*','*','*','*','C','*','*','*','*','*','*','#','*','*','@','#'},
        {'#','#','#','*','#','*','#','*','#','#','#','#','#','#','#','*','#','*','#','*','#','#','#'},
        {'#','*','*','*','*','*','#','*','*','*','*','#','*','*','*','*','#','*','*','*','*','*','#'},
        {'#','*','#','#','#','#','#','#','#','#','*','#','*','#','#','#','#','#','#','#','#','*','#'},
        {'#','*','#','#','#','#','#','#','#','#','*','#','*','#','#','#','#','#','#','#','#','*','#'},
        {'#','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','#'},
        {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},
        {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '}
    };

    void calculatePoints();
};
