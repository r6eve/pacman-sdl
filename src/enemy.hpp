#ifndef ENEMY_H
#define ENEMY_H

namespace enemy {

void init();

void draw();

// TODO: take parameter as enemy_charater
void move_normal_enemy(unsigned int enemy_type);

// TODO: take parameter as enemy_charater
void move_lose_enemy(unsigned int enemy_type);

/**
 * Return true if the player whose state is normal hits enemy, and false
 * otherwise.
 */
bool check_hit_enemy();

}  // namespace enemy

#endif
