#pragma once

#include <iostream>
#include <array>

#include "GameConstants.h"
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

    void loadDetails(SDL_Renderer* renderer, SDL_Texture* sprite_sheet);
    void render(SDL_Renderer* renderer);

    int getPoints() const { return m_points; }

    bool canRenderScore() const { return m_render_score; }
    bool playerScored() const { return m_scored; }

    bool isWall(int y, int x, bool fence_on = true);
    bool isWall(position pos, bool fence_on);

    bool playerEaten() const { return m_player_eaten; }

    bool redVulnerable() const { return m_red_vulnerable; }
    bool pinkVulnerable() const { return m_pink_vulnerable; }
    bool blueVulnerable() const { return m_blue_vulnerable; }
    bool orangeVulnerable() const { return m_orange_vulnerable; }

    bool vulnerable(color ghost_color);

    bool redEaten() const { return m_red_eaten; }
    bool pinkEaten() const { return m_pink_eaten; }
    bool blueEaten() const { return m_blue_eaten; }
    bool orangeEaten() const { return m_orange_eaten; }

    bool eaten(color ghost_color);

    bool bigPointEaten() const { return m_big_point_eaten; }

    char tileAt(int y, int x) const { return m_map[y][x]; }

    void setTileAt(int y, int x, char c) { m_map[y][x] = c; }
    void togglePlayerEatenFlag() { m_player_eaten = !m_player_eaten; }

    char getGhostBuffer(int index) const{ return m_ghost_buffer[index]; }
    void setGhostBuffer(char c, int index);
    void setGhostVulnerable(color ghost_color);

    void toggleRedVulnerableFlag() { m_red_vulnerable = !m_red_vulnerable; }
    void togglePinkVulnerableFlag() { m_pink_vulnerable = !m_pink_vulnerable; }
    void toggleBlueVulnerableFlag() { m_blue_vulnerable = !m_blue_vulnerable; }
    void toggleOrangeVulnerableFlag() { m_orange_vulnerable = !m_orange_vulnerable; }

    void toggleRedEatenFlag() { m_red_eaten = !m_red_eaten; }
    void togglePinkEatenFlag() { m_pink_eaten = !m_pink_eaten; }
    void toggleBlueEatenFlag() { m_blue_eaten = !m_blue_eaten; }
    void toggleOrangeEatenFlag() { m_orange_eaten = !m_orange_eaten; }

    void toggleVulnerableFlag(color ghost_color);
    void toggleEatenFlag(color ghost_color);

    void toggleBigPointEatenFlag() { m_big_point_eaten = !m_big_point_eaten; }

    void toggleRenderScoreFlag() { m_render_score = !m_render_score; }
    void togglePlayerScoreFlag() { m_scored = !m_scored; }

    void resolveCollision(int y, int x, color ghost_color);
    void updatePlayerPos(int old_y, int old_x, int new_y, int new_x);
    void resetPlayer();

    void printMap();

private:
    SDL_Rect m_dest{ 0, 0, tile_len, tile_len };

    SDL_Rect m_big_point_src{ big_point_x, big_point_y, tile_len, tile_len };
    SDL_Rect m_point_src{ point_x, point_y, tile_len, tile_len };
    SDL_Rect m_berry_src{ berry_x, berry_y, tile_len, tile_len };

    SDL_Rect area_rect{ 0, 0, tile_len * map_w, tile_len * map_h };

    SDL_Texture* m_sprite_sheet{ nullptr };
    SDL_Texture* m_area_tex{ nullptr };

    int m_big_points{ 0 };
    int m_points{ 0 };

    std::array<char, 4> m_ghost_buffer = { ' ',' ',' ',' ' }; // For storing points that collide with ghosts

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
    bool m_scored{ false };

    // Change this to 1D and std::array?
    char m_map[map_h][map_w] = {
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
