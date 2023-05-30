#pragma once
#include <string>
#include <vector>
#include <map>

//Knight
static constexpr int KNIGHT_SPEED = 5;
static constexpr int KNIGHT_COST = 400;
static constexpr int KNIGHT_RANGE = 1;
static constexpr int KNIGHT_BUILD_TIME = 5;
static constexpr char KNIGHT_CLASS_LETTER = 'K';

//Swordsman
static constexpr int SWORDSMAN_SPEED = 2;
static constexpr int SWORDSMAN_COST = 250;
static constexpr int SWORDSMAN_RANGE = 1;
static constexpr int SWORDSMAN_BUILD_TIME = 3;
static constexpr char SWORDSMAN_CLASS_LETTER = 'S';


//Archer
static constexpr int ARCHER_SPEED = 2;
static constexpr int ARCHER_COST = 250;
static constexpr int ARCHER_RANGE = 5;
static constexpr int ARCHER_BUILD_TIME = 3;
static constexpr char ARCHER_CLASS_LETTER = 'A';


//Pikeman
static constexpr int PIKEMAN_SPEED = 2;
static constexpr int PIKEMAN_COST = 200;
static constexpr int PIKEMAN_RANGE = 2;
static constexpr int PIKEMAN_BUILD_TIME = 3;
static constexpr char PIKEMAN_CLASS_LETTER = 'P';


//Ram
static constexpr int RAM_SPEED = 2;
static constexpr int RAM_COST = 500;
static constexpr int RAM_RANGE = 1;
static constexpr int RAM_BUILD_TIME = 4;
static constexpr char RAM_CLASS_LETTER = 'R';


//Catapult
static constexpr int CATAPULT_SPEED = 2;
static constexpr int CATAPULT_COST = 800;
static constexpr int CATAPULT_RANGE = 7;
static constexpr int CATAPULT_BUILD_TIME = 6;
static constexpr char CATAPULT_CLASS_LETTER = 'C';


//Worker
static constexpr int WORKER_SPEED = 2;
static constexpr int WORKER_COST = 100;
static constexpr int WORKER_RANGE = 1;
static constexpr int WORKER_BUILD_TIME = 2;
static constexpr char WORKER_CLASS_LETTER = 'W';


//Base
static constexpr int BASE_DURABILITY = 200;
static constexpr char BASE_CLASS_LETTER = 'B';


//Game rules
static constexpr char FRIENDLY_UNIT_OWNER = 'P';
static constexpr char FORBINDDEN_POSITION = '9';
static constexpr char MINE_POSITION = '6';
static constexpr char NEUTRAL_POSITION = '0';
static constexpr char NOT_PRODUCING = '0';
static constexpr int WRONG_POS_ON_MAPCLIP = INT_MAX;
static constexpr int DEFAULT_GOLD_AMOUNT = 2000;
static constexpr int DEFAULT_TIME_LIMIT = 5;

//Exception messages
static const std::string NO_STATUS_FILE = "Status file not found at path: ";
static const std::string NO_MAP_FILE = "Map file not found at path: ";
static const std::string NO_ORDERS_FILE = "Orders file not found at path: ";


// Damages between units
const std::map<char, int> unitsToIndexInAttackTable{
	{KNIGHT_CLASS_LETTER, 0},
	{SWORDSMAN_CLASS_LETTER, 1},
	{ARCHER_CLASS_LETTER, 2},
	{PIKEMAN_CLASS_LETTER, 3},
	{CATAPULT_CLASS_LETTER, 4},
	{RAM_CLASS_LETTER, 5},
	{WORKER_CLASS_LETTER, 6},
	{BASE_CLASS_LETTER, 7}
};

const std::vector<std::vector<int>> attackTable{
	{35, 35, 35, 35, 35, 50, 35, 35}, // Knight
	{30, 30, 30, 20, 20, 30, 30, 30}, // Swordsman
	{15, 15, 15, 15, 10, 10, 15, 15}, // Archer
	{35, 15, 15, 15, 15, 10, 15, 10}, // Pikeman
	{40, 40, 40, 40, 40, 40, 40, 50}, // Catapult
	{10, 10, 10, 10, 10, 10, 10, 50}, // Ram
	{5, 5, 5, 5, 5, 5, 5, 1},		  // Worker
	{0, 0, 0, 0, 0, 0, 0, 0}		  //Base (cannot tryToAttack)
};
