/*
 * Copyright (C) 2009 Christopho, Solarus - http://www.solarus-engine.org
 * 
 * Solarus is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Solarus is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include "hero/StatePlayerMovement.h"
#include "hero/HeroSprites.h"
#include "movements/PlayerMovement.h"

/**
 * @brief Constructor.
 * @param hero the hero
 */
Hero::StatePlayerMovement::StatePlayerMovement(Hero *hero):
  State(hero) {

}

/**
 * @brief Destructor.
 */
Hero::StatePlayerMovement::~StatePlayerMovement(void) {

}

/**
 * @brief Returns the movement of the hero controlled by the player.
 * @return the movement
 */
PlayerMovement * Hero::StatePlayerMovement::get_player_movement(void) {
  return (PlayerMovement*) hero->get_movement();
}

/**
 * @brief Starts this state.
 *
 * This function is called automatically when this state becomes the active state of the hero.
 *
 * @param previous_state the previous state of NULL if this is the first state (for information)
 */
void Hero::StatePlayerMovement::start(State *previous_state) {

  if (previous_state == NULL || !previous_state->can_control_movement()) {
    // create the movement unless it already exists
    hero->set_movement(new PlayerMovement(hero->get_walking_speed()));
  }

  get_player_movement()->set_moving_enabled(true, true);
}

/**
 * @brief Ends this state.
 *
 * This function is called automatically when this state is not the active state anymore.
 * You should here close everything the start() function has opened.
 * The destructor will be called at the next cycle.
 *
 * @param next_state the next state (for information)
 */
void Hero::StatePlayerMovement::stop(State *next_state) {
  
  if (!next_state->can_control_movement()) {
    // destroy the movement unless the next state intends to keep it
    hero->clear_movement();
  }
}

/**
 * @brief Changes the map.
 *
 * This function is called when the hero is about to go to another map.
 *
 * @param map the new map
 */
void Hero::StatePlayerMovement::set_map(Map *map) {

  State::set_map(map);
  set_animation_stopped();
}

/**
 * @brief Notifies this state that a directional key was just pressed.
 * @param direction4 direction of the key (0 to 3)
 */
void Hero::StatePlayerMovement::directional_key_pressed(int direction4) {

  // notify the movement
  get_player_movement()->directional_key_pressed(direction4);
}

/**
 * @brief Notifies this state that a directional key was just released.
 * @param direction4 direction of the key (0 to 3)
 */
void Hero::StatePlayerMovement::directional_key_released(int direction4) {

  // notify the movement
  get_player_movement()->directional_key_released(direction4);
}

/**
 * @brief Returns whether the player can control his movements in the current state.
 * @return true if the player can control his movements
 */
bool Hero::StatePlayerMovement::can_control_movement(void) {
  return true;
}

/**
 * @brief Returns whether the player can control his direction in the current state.
 * @return true if the player can control his direction
 */
bool Hero::StatePlayerMovement::can_control_direction(void) {
  return true;
}

/**
 * @brief Returns the direction of the hero's movement as defined by the controls applied by the player.
 *
 * If he is not moving, -1 is returned.
 * This direction may be different from the real movement direction because of obstacles.
 *
 * @return the hero's wanted direction between 0 and 359, or -1 if he is stopped
 */
int Hero::StatePlayerMovement::get_wanted_movement_direction(void) {
  return get_player_movement()->get_direction();
}

/**
 * @brief Notifies this state that the walking speed has changed.
 * 
 * If the hero can walk in this state, the state should modify its movement
 * to set the new speed.
 */
void Hero::StatePlayerMovement::notify_walking_speed_changed(void) {
  get_player_movement()->set_moving_speed(hero->get_walking_speed());
}

/**
 * @brief Notifies this state that the movement has changed.
 *
 * This function is called when the hero's movement direction changes (for instance
 * because the player pressed or released a directional key, or the hero just reached an obstacle).
 * The animations and collisions should be updated according to the new movement.
 */
void Hero::StatePlayerMovement::notify_movement_changed(void) {

  // the movement has changed: update the animation of the sprites

  bool movement_walking = get_player_movement()->is_started();
  bool sprites_walking = hero->get_sprites()->is_walking();

  if (movement_walking && !sprites_walking) {
    set_animation_walking();
  }
  else if (!movement_walking && sprites_walking) {
    set_animation_stopped();
  }
}

/**
 * @brief Stops the movement of the player and lets the player restart it when he can.
 *
 * This function is typically called when the player loses temporarily the control
 * (e.g. because of a script or a teletransporter) whereas the movement remains the same.
 * Then the movement may want to move a few pixels more as soon as it is resumed.
 * This function removes such residual effects of the player's movement.
 * If the current movement is not controlled by the player, this function has no effect.
 */
void Hero::StatePlayerMovement::reset_movement(void) {

  PlayerMovement *movement = get_player_movement();
  movement->stop();
  movement->set_moving_enabled(false, false);
  movement->set_moving_enabled(true, true);
}

/**
 * @brief Sets whether the movement allows to traverse obstacles.
 *
 * This function is used for debugging purposes only.
 *
 * @param stop_on_obstacles true to make the movement sensible to obstacles, false to ignore them
 */
void Hero::StatePlayerMovement::set_stop_on_obstacles(bool stop_on_obstacles) {

  get_player_movement()->set_stop_on_obstacles(stop_on_obstacles);
}

/**
 * @brief Returns whether the hero can be hurt in this state.
 * @return true if the hero can be hurt in this state
 */
bool Hero::StatePlayerMovement::can_be_hurt(void) {
  return true;
}
