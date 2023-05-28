#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../Player.h"
#include <string>
#include <sstream>
#include <fstream>

static const std::string mapPath = "mapa.txt";
static const std::string statusPath = "status.txt";
static const std::string ordersPath = "rozkazy.txt";

class TempFile
{
public:

	TempFile() = default;

	TempFile(const std::string& fileName, const std::string& content = "")
		:
		fileName(fileName)
	{
		std::ofstream file(fileName);
		file << content;
		file.close();
	}
	~TempFile()
	{
		if (!fileName.empty())
		{
			std::remove(fileName.c_str());
		}
	}

	std::string getContent() const
	{
		std::ifstream file(fileName);
		std::stringstream iss;
		iss << file.rdbuf();
		file.close();
		return iss.str();
	}

	std::string getFileName()
	{
		return fileName;
	}

private:
	std::string fileName;
};

TEST(PlayerConstructorTests, IncorrectMapFile)
{
	TempFile statusFile(statusPath);
	TempFile ordersFile(ordersPath);

	EXPECT_THROW({
		try
		{
			Player player(mapPath, statusFile.getFileName(), ordersFile.getFileName(), DEFAULT_TIME_LIMIT);
		}
		catch (const std::runtime_error& e)
		{
			EXPECT_STREQ((NO_MAP_FILE + mapPath).c_str(), e.what());
			throw;
		}
		}, std::runtime_error);
}

TEST(PlayerConstructorTests, IncorrectStatusFile)
{
	TempFile mapFile(mapPath);
	TempFile ordersFile(ordersPath);

	EXPECT_THROW({
		try
		{
			Player player(mapFile.getFileName(), statusPath, ordersFile.getFileName(), DEFAULT_TIME_LIMIT);
		}
		catch (const std::runtime_error& e)
		{
			EXPECT_STREQ((NO_STATUS_FILE + statusPath).c_str(), e.what());
			throw;
		}
		}, std::runtime_error);
}

TEST(PlayerConstructorTests, IncorrectOrdersFile)
{
	TempFile mapFile(mapPath);
	TempFile statusFile(statusPath);

	EXPECT_THROW({
		try
		{
			Player player(mapFile.getFileName(), statusFile.getFileName(), ordersPath, DEFAULT_TIME_LIMIT);
		}
		catch (const std::runtime_error& e)
		{
			EXPECT_STREQ((NO_ORDERS_FILE + ordersPath).c_str(), e.what());
			throw;
		}
		}, std::runtime_error);
}

TEST(PlayerConstructorTests, IncorrectTimeLimit)
{
	TempFile mapFile(mapPath);
	TempFile statusFile(statusPath);
	TempFile ordersFile(ordersPath);
	constexpr int timeLimit = (-1) * DEFAULT_TIME_LIMIT;

	EXPECT_THROW({
		try
		{
			Player player(mapFile.getFileName(), statusFile.getFileName(), ordersFile.getFileName(), timeLimit);
		}
		catch (const std::invalid_argument& e)
		{
			EXPECT_STREQ(("The player won't be able to make a move with time = " + std::to_string(timeLimit)).c_str(), e.what());
			throw;
		}
		}, std::invalid_argument);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class AllFilesTogether : public ::testing::Test
{
public:
	TempFile* mapF;
	TempFile* statusF;
	TempFile* ordersF;

	AllFilesTogether() :
		mapF(nullptr),
		statusF(nullptr),
		ordersF(nullptr)
	{
	}
	~AllFilesTogether() override
	{
		delete mapF;
		delete statusF;
		delete ordersF;
	}

	void SetUp(const std::string& mapContent, const std::string& statusContent, const std::string& ordersContent)
	{
		delete mapF;
		delete statusF;
		delete ordersF;

		mapF = new TempFile(mapPath, mapContent);
		statusF = new TempFile(statusPath, statusContent);
		ordersF = new TempFile(ordersPath, ordersContent);

		Player player(mapF->getFileName(), statusF->getFileName(), ordersF->getFileName(), DEFAULT_TIME_LIMIT);

		player.takeATurn();
	}
};

TEST_F(AllFilesTogether, CreateACatapult)
{
	const std::string map =
		"10000\n"
		"09020";

	const std::string status =
		"800\n"
		"P B 10 0 0 98 0\n"
		"E B 11 3 1 150 A";

	const std::string randomOrdersFromBefore = "11 B A";

	SetUp(map, status, randomOrdersFromBefore);

	EXPECT_STREQ(ordersF->getContent().c_str(), "10 B C\n");
}

TEST_F(AllFilesTogether, DoNothing)
{
	const std::string map =
		"10000\n"
		"09020";

	const std::string status =
		"50\n"
		"P B 10 0 0 98 0\n"
		"E B 11 3 1 150 A";

	SetUp(map, status, "");

	EXPECT_STREQ(ordersF->getContent().c_str(), "");
}

TEST_F(AllFilesTogether, MoveUnitOverObstacles)
{
	const std::string map =
		"1009999002";

	const std::string status =
		"0\n"
		"P B 10 0 0 98 0\n"
		"E B 11 9 0 150 A\n"
		"P K 5 2 0 70";

	SetUp(map, status, "");

	EXPECT_STREQ(ordersF->getContent().c_str(), "5 M 7 0\n");
}

TEST_F(AllFilesTogether, CreateMinerAndMoveOtherMinersToMines)
{
	const std::string map =
		"1000000002\n"
		"0000000000\n"
		"0000666000\n"
		"0000666600\n";

	const std::string status =
		"1000\n"
		"P B 0 0 0 98 0\n"
		"E B 1 9 0 150 A\n"
		"P W 2 3 1 20\n"
		"P W 3 2 2 20\n"
		"P W 4 2 2 20\n";

	const std::string expectedOrders =
		"0 B W\n"
		"2 M 4 2\n"
		"3 M 4 2\n"
		"4 M 4 2\n";

	SetUp(map, status, "");

	EXPECT_STREQ(ordersF->getContent().c_str(), expectedOrders.c_str());
}

TEST_F(AllFilesTogether, ComeCloserAndAttack)
{
	const std::string map =
		"2000000001\n";

	const std::string status =
		"0\n"
		"P B 0 9 0 98 A\n"
		"E B 1 0 0 150 A\n"
		"P C 2 8 0 50\n"
		"P K 3 2 0 50";

	const std::string expectedOrders =
		"2 M 7 0\n"
		"3 M 1 0\n"
		"2 A 1\n";

	SetUp(map, status, "");

	EXPECT_STREQ(ordersF->getContent().c_str(), expectedOrders.c_str());
}

TEST_F(AllFilesTogether, StandStillAndAttack)
{
	const std::string map =
		"2000000001\n";

	const std::string status =
		"0\n"
		"P B 0 9 0 98 A\n"
		"E B 1 0 0 150 A\n"
		"P C 2 5 0 50\n"
		"P K 3 2 0 50";

	// even if units don't move they generate a "move"
	// since the amount of moves per tour is not limited
	// this is no problem
	const std::string expectedOrders =
		"2 M 5 0\n"
		"3 M 1 0\n"
		"2 A 1\n";

	SetUp(map, status, "");

	EXPECT_STREQ(ordersF->getContent().c_str(), expectedOrders.c_str());
}

// this will happen rarely
TEST_F(AllFilesTogether, AvoidPlaceWithEnemyUnit)
{
	const std::string map =
		"2099099001\n";

	const std::string status =
		"0\n"
		"P B 0 9 0 98 A\n"
		"E B 1 0 0 150 A\n"
		"E C 2 4 0 50\n"
		"P K 3 7 0 50";

	const std::string expectedOrders = "3 M 7 0\n";

	SetUp(map, status, "");

	EXPECT_STREQ(ordersF->getContent().c_str(), expectedOrders.c_str());
}

// this will happen rarely
TEST_F(AllFilesTogether, AttackEnemyUnit)
{
	const std::string map =
		"2009999001\n";

	const std::string status =
		"0\n"
		"P B 0 9 0 98 A\n"
		"E B 1 0 0 150 A\n"
		"E C 2 2 0 50\n"
		"P A 3 7 0 50";

	const std::string expectedOrders =
		"3 M 7 0\n"
		"3 A 2\n";

	SetUp(map, status, "");

	EXPECT_STREQ(ordersF->getContent().c_str(), expectedOrders.c_str());
}

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}