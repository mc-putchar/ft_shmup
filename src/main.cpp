
#include <curses.h>
#include <fcntl.h>
#include <ncurses.h>
#include <unistd.h>  // for usleep

#include <algorithm>
#include <chrono>
#include <csignal>
#include <cstdio>
#include <cstdlib>  // for system()
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

#include "Enemy.hpp"
#include "Entity.hpp"
#include "Game.hpp"
#include "GameConfig.hpp"
#include "Player.hpp"
#include "Weapon.hpp"
#include "ft_shmup.hpp"

int64_t g_score = 0;

static void game_loop(Game& world, Player& p);
static int check_for_collision(Game& world, Entity const& e);
static void display_hud(Game& world, Player& p);
static void draw_background(Game const& world);

void display_story(GameConfig& config, int rows, int cols) {
    (void)cols;
    std::string line;
    int i = 0;
    if ((int)config.intro.size() == 0) {
        mvprintw(0, 0, "...loading story...");
    }
    if ((int)config.intro.size() > rows) {
        attron(A_BLINK);
        mvprintw(i + 1, 16, "%s", "Press Any Keys To Continue");
        attroff(A_BLINK);
        refresh();
    } else {
        for (const std::string& line : config.intro) {
            mvprintw(i + 1, 15, "%s", line.c_str());
            i++;
            refresh();
            usleep(50000);
        }
    }
    getch();
}

typedef struct {
    double fps;
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> endTime;
    std::chrono::duration<double> elapsedTime;
    std::chrono::duration<double> remainingTime;
} GameParams;

int main(int ac, char** av) {
    if (ac > 1)
        std::cout << "Usage: " << av[0] << std::endl;

    usleep(100000);  // give time to alacritty to set up the terminal

    GameConfig& config = GameConfig::getConf();
    GameParams params;
    params.fps = 60.0;
    params.startTime = std::chrono::high_resolution_clock::now();

    Game world;
    world.status = STORY;
    init_screen();
    refresh();
    if (world.status == STORY) {
        display_story(config, LINES, COLS);
        world.status = MENU;
    }
    init_windows(world);
    wrefresh(world.main);
    nodelay(stdscr, TRUE);  // non-blocking getch

    while (true) {
        Texture skin(10, 3, "~>L-\\___  ~XE[]==O}>~>F-/```  ");
        Player p(Point(1, 15), 3, 0, skin);
        Texture bullet_tex(1, 1, "-");
        Texture laser_icon(3, 3, ",_,|!|```");
        Weapon* laser = new Weapon(laser_icon, 1000, 1, bullet_tex, 3);
        p.set_weapon(laser);
        g_score = 0;

        game_loop(world, p);
        if (draw_menu(world))
            break;
    }
    return 0;
}

static void game_loop(Game& world, Player& p) {
    nodelay(world.main, TRUE);  // Make wgetch non-blocking
    nodelay(world.hud, TRUE);   // Make wgetch non-blocking
    nodelay(stdscr, TRUE);      // Make wgetch non-blocking
    int ch;
    int i = 0;
    Enemy::create_enemies(world.enemies, 210);
    world.status = PLAY;
    world.bullets.clear();
    world.bullets.reserve(64);
    while (true) {
        if (g_score == 1500) {
            mvwprintw(world.main, 1, 1, "YOU WIN");
            wrefresh(world.main);
            usleep(1000000);
            cleanup_and_exit(world);
        }
        if (i % 123) {
            draw_background(world);
            refresh();
            wrefresh(world.main);
        }
        init_pair(3, COLOR_RED, COLOR_BLACK);  // Define gray color pair
        for (std::vector<Enemy>::iterator it = world.enemies.begin();
             it != world.enemies.end(); ++it) {
            Enemy* enemy = dynamic_cast<Enemy*>(&(*it));
            if (enemy) {
                enemy->update(world.bullets, i);
                wattron(world.main,
                        COLOR_PAIR(3));  // Assuming color pair 2 is red
                put_entity(world.main, *enemy);
                wattroff(world.main, COLOR_PAIR(3));
            }
        }
        init_pair(4, COLOR_WHITE, COLOR_MAGENTA);  // Define gray color pair
        for (std::vector<Projectile*>::iterator it = world.bullets.begin();
             it != world.bullets.end(); ++it) {
            (*it)->update();
            wattron(world.main, COLOR_PAIR(4));
            put_projectile(world.main, *it);
            wattroff(world.main, COLOR_PAIR(4));
        }
        for (auto bullet : world.bullets) {
            if (!bullet->active) {
                world.bullets.erase(std::remove(world.bullets.begin(),
                                                world.bullets.end(), bullet),
                                    world.bullets.end());
                delete bullet;
            }
        }
        ch = wgetch(world.main);
        if (!(i & 3)) {
            wclear(world.main);
            wclear(world.hud);
            // i = 0;
        }
        // if (ch != ERR) {
        switch (ch) {
            case 'w':
                // mvwprintw(world.main, 10, 40, "KEY_UP");
                p.move(world, Point(0, -1));
                break;
            case 's':
                // mvwprintw(world.main, 10, 40, "KEY_DOWN");
                p.move(world, Point(0, 1));
                break;
            case 'a':
                // mvwprintw(world.main, 10, 40, "KEY_LEFT");
                p.move(world, Point(-1, 0));
                break;
            case 'd':
                // mvwprintw(world.main, 10, 40, "KEY_RIGHT");
                p.move(world, Point(1, 0));
                break;
            case ' ':
                // mvwprintw(world.main, 10, 40, "KEY_SPACE");
                p.fire(world.bullets, i);
                break;
            case 10:  // Enter key
                break;
            case 27:  // Escape key
                cleanup_and_exit(world);
                break;
            default:
                p.move(world, Point(0, 0));
                wborder(world.main, 0, 0, 0, 0, 0, 0, 0, 0);
                display_hud(world, p);
                break;
        }
        // }
        refresh();
        wrefresh(world.main);
        wrefresh(world.hud);
        usleep(10000);  // Sleep for 20ms
        i++;
        if (check_for_collision(world, p) == 1) {
            p.set_health(p.get_health() - 1);
            if (p.get_health() == 0) {
                for (auto bullet : world.bullets)
                    delete bullet;
                world.bullets.clear();
                world.enemies.clear();
                mvwprintw(world.main, 1, 1, "GAME OVER");
                wrefresh(world.main);
                usleep(1000000);
                return;
            }
        }
    }
}

static int check_for_collision(Game& world, Entity const& e) {
    auto check_collision = [](Point const& p1, Point const& size1,
                              Point const& p2, Point const& size2) {
        return (p1.x < p2.x + size2.x && p1.x + size1.x > p2.x &&
                p1.y < p2.y + size2.y && p1.y + size1.y > p2.y);
    };

    for (Enemy const& enemy : world.enemies) {
        if (check_collision(e.get_position(), e.get_size(),
                            enemy.get_position(), enemy.get_size())) {
            world.enemies.erase(
                std::remove(world.enemies.begin(), world.enemies.end(), enemy),
                world.enemies.end());
            return 1;
        }
    }

    for (Projectile* bullet : world.bullets) {
        if (check_collision(e.get_position(), e.get_size(),
                            bullet->get_position(), bullet->get_size())) {
            delete bullet;
            world.bullets.erase(
                std::remove(world.bullets.begin(), world.bullets.end(), bullet),
                world.bullets.end());
            return 1;
        }
    }

    for (Enemy& enemy : world.enemies) {
        for (Projectile* bullet : world.bullets) {
            if (check_collision(enemy.get_position(), enemy.get_size(),
                                bullet->get_position(), bullet->get_size())) {
                world.enemies.erase(std::remove(world.enemies.begin(),
                                                world.enemies.end(), enemy),
                                    world.enemies.end());
                delete bullet;
                world.bullets.erase(std::remove(world.bullets.begin(),
                                                world.bullets.end(), bullet),
                                    world.bullets.end());
                g_score += 10;
            }
        }
    }

    return (0);
}

static void display_hud(Game& world, Player& p) {
    mvwprintw(world.hud, 1, 1, "Health: %d", p.get_health());
    mvwprintw(world.hud, 3, 1, "Score: %ld", g_score);
    mvwprintw(world.hud, 5, 1, "FPS: 60");
    wborder(world.hud, 0, 0, 0, 0, 0, 0, 0, 0);
    wrefresh(world.hud);
}

static char get_background_element(void) {
    int chance = rand() % 10000;
    if (chance == 1) {
        return '@';
    } else if (chance < 10) {
        return chance % 2 ? '*' : '+';
    } else if (chance < 20) {
        return chance % 2 ? ',' : '`';
    } else {
        return ' ';
    }
}

static void draw_background(Game const& world) {
    int16_t mw, mh;
    getmaxyx(world.main, mh, mw);
    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);  // Define gray color pair
    for (int i = 0; i < mh; ++i) {
        for (int j = 0; j < mw; ++j) {
            wattron(world.main, COLOR_PAIR(1));
            mvwaddch(world.main, i, j, get_background_element());
            wattroff(world.main, COLOR_PAIR(1));
        }
    }
}

int cleanup_and_exit(Game& world) {
    delwin(world.main);
    delwin(world.hud);
    endwin();
    for (auto bullet : world.bullets)
        delete bullet;
    world.bullets.clear();
    world.enemies.clear();
    exit(0);
}
