#ifndef ENEMY_H
#define ENEMY_H

namespace enemy {

void init();

void draw();

void move_normal_enemy(int enemy_type);

void move_lose_enemy(int enemy_type);

/**
 * Return true if the player whose state is normal hits enemy, and false
 * otherwise.
 */
bool check_hit_enemy();

}  // namespace enemy

#endif
