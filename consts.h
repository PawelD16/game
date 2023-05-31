#pragma once
#include <string>
#include <vector>
#include <map>

// Knight
static constexpr int KNIGHT_DURABILITY = 70;
static constexpr int KNIGHT_SPEED = 5;
static constexpr int KNIGHT_COST = 400;
static constexpr int KNIGHT_RANGE = 1;
static constexpr int KNIGHT_BUILD_TIME = 5;
static constexpr char KNIGHT_CLASS_LETTER = 'K';

// Swordsman
static constexpr int SWORDSMAN_DURABILITY = 60;
static constexpr int SWORDSMAN_SPEED = 2;
static constexpr int SWORDSMAN_COST = 250;
static constexpr int SWORDSMAN_RANGE = 1;
static constexpr int SWORDSMAN_BUILD_TIME = 3;
static constexpr char SWORDSMAN_CLASS_LETTER = 'S';

// Archer
static constexpr int ARCHER_DURABILITY = 40;
static constexpr int ARCHER_SPEED = 2;
static constexpr int ARCHER_COST = 250;
static constexpr int ARCHER_RANGE = 5;
static constexpr int ARCHER_BUILD_TIME = 3;
static constexpr char ARCHER_CLASS_LETTER = 'A';

// Pikeman
static constexpr int PIKEMAN_DURABILITY = 50;
static constexpr int PIKEMAN_SPEED = 2;
static constexpr int PIKEMAN_COST = 200;
static constexpr int PIKEMAN_RANGE = 2;
static constexpr int PIKEMAN_BUILD_TIME = 3;
static constexpr char PIKEMAN_CLASS_LETTER = 'P';

// Ram
static constexpr int RAM_DURABILITY = 90;
static constexpr int RAM_SPEED = 2;
static constexpr int RAM_COST = 500;
static constexpr int RAM_RANGE = 1;
static constexpr int RAM_BUILD_TIME = 4;
static constexpr char RAM_CLASS_LETTER = 'R';

// Catapult
static constexpr int CATAPULT_DURABILITY = 50;
static constexpr int CATAPULT_SPEED = 2;
static constexpr int CATAPULT_COST = 800;
static constexpr int CATAPULT_RANGE = 7;
static constexpr int CATAPULT_BUILD_TIME = 6;
static constexpr char CATAPULT_CLASS_LETTER = 'C';

// Worker
static constexpr int WORKER_DURABILITY = 20;
static constexpr int WORKER_SPEED = 2;
static constexpr int WORKER_COST = 100;
static constexpr int WORKER_RANGE = 1;
static constexpr int WORKER_BUILD_TIME = 2;
static constexpr char WORKER_CLASS_LETTER = 'W';

// Base
static constexpr int BASE_DURABILITY = 200;
static constexpr char BASE_CLASS_LETTER = 'B';

// Game rules
static constexpr char OWNED_BY_TOUR_TAKER = 'P';
static constexpr char OWNED_BY_TOUR_WATCHER = 'E';
static constexpr char BASE_OF_TOUR_TAKER = '1';
static constexpr char BASE_OF_TOUR_WATCHER = '2';

static constexpr char ATTACK_ACTION = 'A';
static constexpr char MOVE_ACTION = 'M';
static constexpr char BUILD_ACTION = 'B';

static constexpr char FORBINDDEN_POSITION = '9';
static constexpr char MINE_POSITION = '6';
static constexpr char NEUTRAL_POSITION = '0';
static constexpr char NOT_PRODUCING = '0';
static constexpr int DEFAULT_GOLD_AMOUNT = 2000;
static constexpr int DEFAULT_TIME_LIMIT = 5;
static constexpr int TOURS_LIMIT = 2000;

// Exception messages
static const std::string NO_STATUS_FILE = "Status file not found at path: ";
static const std::string NO_MAP_FILE = "Map file not found at path: ";
static const std::string NO_ORDERS_FILE = "Orders file not found at path: ";
static const std::string NO_FILE = "File not found at path: ";
static const std::string INCORRECT_PLAYER = "One or more players incorrect!";

// Match results
static const std::string PLAYER1_WON = "Congratulations for winning to player1!";
static const std::string PLAYER2_WON = "Player2 has dominated!";
static const std::string DRAW = "Draw!";
static const std::string PLAYER1_DISQUALIFIED = "Player1 disqualified!";
static const std::string PLAYER2_DISQUALIFIED = "Player2 disqualified";


// Damages between units
const std::map<char, int> UNITS_TO_INDEX_FOR_ATTACK_TABLE{
	{KNIGHT_CLASS_LETTER, 0},
	{SWORDSMAN_CLASS_LETTER, 1},
	{ARCHER_CLASS_LETTER, 2},
	{PIKEMAN_CLASS_LETTER, 3},
	{CATAPULT_CLASS_LETTER, 4},
	{RAM_CLASS_LETTER, 5},
	{WORKER_CLASS_LETTER, 6},
	{BASE_CLASS_LETTER, 7}};

const std::vector<std::vector<int>> ATTACK_TABLE{
	{35, 35, 35, 35, 35, 50, 35, 35}, // Knight
	{30, 30, 30, 20, 20, 30, 30, 30}, // Swordsman
	{15, 15, 15, 15, 10, 10, 15, 15}, // Archer
	{35, 15, 15, 15, 15, 10, 15, 10}, // Pikeman
	{40, 40, 40, 40, 40, 40, 40, 50}, // Catapult
	{10, 10, 10, 10, 10, 10, 10, 50}, // Ram
	{5, 5, 5, 5, 5, 5, 5, 1},		  // Worker
	{0, 0, 0, 0, 0, 0, 0, 0}		  // Base (cannot tryToAttack)
};

// this collection allows for easy verification of existing unit types
const std::vector<char> POSSIBLE_UNIT_TYPES{
	KNIGHT_CLASS_LETTER, 
	SWORDSMAN_CLASS_LETTER, 
	ARCHER_CLASS_LETTER, 
	PIKEMAN_CLASS_LETTER, 
	RAM_CLASS_LETTER, 
	CATAPULT_CLASS_LETTER, 
	WORKER_CLASS_LETTER, 
	BASE_CLASS_LETTER
	};

const std::map<char, int> UNIT_COST_TABLE{
	{KNIGHT_CLASS_LETTER, KNIGHT_COST}, 
	{SWORDSMAN_CLASS_LETTER, SWORDSMAN_COST}, 
	{ARCHER_CLASS_LETTER, ARCHER_COST}, 
	{PIKEMAN_CLASS_LETTER, PIKEMAN_COST}, 
	{RAM_CLASS_LETTER, RAM_COST}, 
	{CATAPULT_CLASS_LETTER, CATAPULT_COST}, 
	{WORKER_CLASS_LETTER, WORKER_COST}
	};

const std::map<char, int> UNIT_BUILD_TIME_TABLE{
{KNIGHT_CLASS_LETTER, KNIGHT_BUILD_TIME}, 
{SWORDSMAN_CLASS_LETTER, SWORDSMAN_BUILD_TIME}, 
{ARCHER_CLASS_LETTER, ARCHER_BUILD_TIME}, 
{PIKEMAN_CLASS_LETTER, PIKEMAN_BUILD_TIME}, 
{RAM_CLASS_LETTER, RAM_BUILD_TIME}, 
{CATAPULT_CLASS_LETTER, CATAPULT_BUILD_TIME}, 
{WORKER_CLASS_LETTER, WORKER_BUILD_TIME}
};

const std::map<char, int> UNIT_DURABILITY_MAP{
{KNIGHT_CLASS_LETTER, KNIGHT_DURABILITY}, 
{SWORDSMAN_CLASS_LETTER, SWORDSMAN_DURABILITY}, 
{ARCHER_CLASS_LETTER, ARCHER_DURABILITY}, 
{PIKEMAN_CLASS_LETTER, PIKEMAN_DURABILITY}, 
{RAM_CLASS_LETTER, RAM_DURABILITY}, 
{CATAPULT_CLASS_LETTER, CATAPULT_DURABILITY}, 
{WORKER_CLASS_LETTER, WORKER_DURABILITY},
{BASE_CLASS_LETTER, BASE_DURABILITY}
};

// this collection allows for easy verification of existing players and their base numbers
// It could be easily expanded if there were to be more players
const std::map<char, char> PLAYER_TYPE_TO_BASE_NUM{
	{OWNED_BY_TOUR_TAKER, BASE_OF_TOUR_TAKER},
	{OWNED_BY_TOUR_WATCHER, BASE_OF_TOUR_WATCHER}};

const std::map<char, int> UNIT_RANGE{
{KNIGHT_CLASS_LETTER, KNIGHT_RANGE}, 
{SWORDSMAN_CLASS_LETTER, SWORDSMAN_RANGE}, 
{ARCHER_CLASS_LETTER, ARCHER_RANGE}, 
{PIKEMAN_CLASS_LETTER, PIKEMAN_RANGE}, 
{RAM_CLASS_LETTER, RAM_RANGE}, 
{CATAPULT_CLASS_LETTER, CATAPULT_RANGE}, 
{WORKER_CLASS_LETTER, WORKER_RANGE}
};

const std::map<char, int> UNIT_SPEED{
{KNIGHT_CLASS_LETTER, KNIGHT_SPEED}, 
{SWORDSMAN_CLASS_LETTER, SWORDSMAN_SPEED}, 
{ARCHER_CLASS_LETTER, ARCHER_SPEED}, 
{PIKEMAN_CLASS_LETTER, PIKEMAN_SPEED}, 
{RAM_CLASS_LETTER, RAM_SPEED}, 
{CATAPULT_CLASS_LETTER, CATAPULT_SPEED}, 
{WORKER_CLASS_LETTER, WORKER_SPEED},
{BASE_CLASS_LETTER, 0}
};

// maps actions to units that are allowed to do them
const std::map<char, std::vector<char>> ALLOWED_ACTIONS_BY_UNIT_TYPE{
	{ATTACK_ACTION, {POSSIBLE_UNIT_TYPES[0],
					POSSIBLE_UNIT_TYPES[1],
					POSSIBLE_UNIT_TYPES[2],
					POSSIBLE_UNIT_TYPES[3],
					POSSIBLE_UNIT_TYPES[4],
					POSSIBLE_UNIT_TYPES[5],
					POSSIBLE_UNIT_TYPES[6]}},
	{MOVE_ACTION, {POSSIBLE_UNIT_TYPES[0],
					POSSIBLE_UNIT_TYPES[1],
					POSSIBLE_UNIT_TYPES[2],
					POSSIBLE_UNIT_TYPES[3],
					POSSIBLE_UNIT_TYPES[4],
					POSSIBLE_UNIT_TYPES[5],
					POSSIBLE_UNIT_TYPES[6]}},
	{BUILD_ACTION, {POSSIBLE_UNIT_TYPES[7]}}};
