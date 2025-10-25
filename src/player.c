#include "player.h"

int getPlayersInitCoordinate(int playerIndex, int axis) {
    static int PLAYERS_INIT_COORDINATES[][2] = {{40, 160}, {440, 160}};

    return PLAYERS_INIT_COORDINATES[playerIndex][axis];
}

int getPlayersInitDirection(int playerIndex, int axis) {
    static int PLAYERS_INIT_DIRECTION[][2] = {{1, 0}, {-1, 0}};

    return PLAYERS_INIT_DIRECTION[playerIndex][axis];
}

uint16_t getColorSet(int index) {
    static uint16_t PLAYER_COLOR_SET[] = {DISPLAY_COLOR(31, 0, 0), DISPLAY_COLOR(31, 27, 0), DISPLAY_COLOR(31, 50, 0),
							   DISPLAY_COLOR(0, 63, 0), DISPLAY_COLOR(20, 30, 8), DISPLAY_COLOR(0, 63, 31),
							   DISPLAY_COLOR(0, 0, 31), DISPLAY_COLOR(15, 30, 8), DISPLAY_COLOR(31, 0, 31)};

    return PLAYER_COLOR_SET[index];
}

bool isPlayerHitedAnotherObject(int playerCoordinates[], int playerDirection[], uint16_t* fb) {
	int x_coordinate = playerCoordinates[0];
	int y_coordinate = playerCoordinates[1];
	int x_direction = playerDirection[0];
	int y_direction = playerDirection[1];
	int x_offset = 0;
	int y_offset = 0;

	if(x_direction == 1 && y_direction == 0) {
		x_offset = PLAYER_SIZE;
	} else if(x_direction == 0 && y_direction == 1) {
		y_offset = -1;
	} else if(x_direction == -1 && y_direction == 0) {
		x_offset = -1;
	} else if(x_direction == 0 && y_direction == -1) {
		y_offset = PLAYER_SIZE;
	}

    if(fb[DISPLAY_WIDTH*(y_coordinate+y_offset) + x_coordinate+x_offset] != 0) {
        return false;
    }

    return true;
}

void resetPlayers(player_t* players[]) {
	for(size_t i = 0; i < NUM_OF_PLAYERS; i++) {
		player_t* player = players[i];

		player->coordinates[0] = getPlayersInitCoordinate(i, 0);
		player->coordinates[1] = getPlayersInitCoordinate(i, 1);
		player->direction[0] = getPlayersInitDirection(i, 0);
		player->direction[1] = getPlayersInitDirection(i, 1);
    player->isLooser = false;

		player->speed->acceleration = 1;
		player->speed->amountOfAccelerations[i] = 8;
		player->speed->durationCounter[i] = 0;
		player->speed->regenerationCounter[i] = 0;
	}
}

int findWinner(player_t* players[], int numOfPlayers){
	int ret = 0;
	for(int i = 0; i < numOfPlayers; i++){
		if (!(players[i]->isLooser)) return i;
	}

	return ret;
}

void chooseRandomDirection(player_t* player) {
	int randomSign = getRandomSign();
	int x = player->direction[0];
	int y = player->direction[1];

	player->direction[0] = y*randomSign;
	player->direction[1] = x*randomSign;
}

void initPlayer(player_t* player, int coordinates[2], int direction[2],
                uint32_t controlKnob, uint8_t knob, bool isLooser, bool isAI,
                player_speed_t* speed) {

  player->coordinates[0] = coordinates[0];
  player->coordinates[1] = coordinates[0];

  player->direction[0] = direction[0];
  player->direction[1] = direction[1];

  player->controlKnob = controlKnob;
  player->knob = knob;

  player->isLooser = isLooser;
  player->isAI = isAI;
  player->speed = speed;
}
