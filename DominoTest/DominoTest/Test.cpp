#include <gtest/gtest.h>
#include <iostream>
#include "DominoDynamic.h"

struct DominoTesting : testing::Test
{

	DominoDynamic::DominoSet* set;

	DominoTesting()
	{
		set = new DominoDynamic::DominoSet;
	}

	~DominoTesting()
	{
		delete set;
	}
};

TEST_F(DominoTesting, InitializeAndAssignmentTest)
{
	using namespace DominoDynamic;
	EXPECT_EQ(1, set->getCurSize());
	Domino domino = (*set)[0];
	EXPECT_EQ(domino.bot, set->getDominos()[0].bot);
	EXPECT_EQ(domino.top, set->getDominos()[0].top);
	DominoSet set1;
	domino = set1.getDominos()[0];
	delete set;
	set = new DominoSet(DominoSet(1, 3));
	EXPECT_EQ(1, set->getDominos()[0].bot);
	EXPECT_EQ(3, set->getDominos()[0].top);
	
	EXPECT_ANY_THROW(DominoSet(-1));
	EXPECT_ANY_THROW(DominoSet(1, 1));
	EXPECT_ANY_THROW(DominoSet(-1, 9));
	delete set;
	set = new DominoSet(Domino(1, 1));
	EXPECT_EQ(1, (*set)[0].bot);
	EXPECT_EQ(1, (*set)[0].top);
	DominoSet set2 = set1;
	EXPECT_EQ(set1[0].bot, (set2)[0].bot);
	EXPECT_EQ(set1[0].top, (set2)[0].top);
}

TEST_F(DominoTesting, FunctionsTest)
{
	using namespace DominoDynamic;
	Domino domino(0, 5);
	*set + domino;
	EXPECT_EQ((*set)[set->find(domino)].bot, 0);
	EXPECT_EQ((*set)[set->find(domino)].top, 5);
	EXPECT_EQ((*set).contains(domino), true);
	*set - domino;
	EXPECT_EQ((*set).find(domino), -1);
	EXPECT_EQ((*set).getCurSize(), 1);
	EXPECT_EQ((*set).contains(domino), false);

	DominoSet* set1 = new DominoSet(Domino(3, 5));
	*set + domino;
	DominoSet* set2 = *set1 + *set;
	EXPECT_EQ((*set2).contains(domino), true);
	EXPECT_EQ((*set2).contains(Domino(5, 3)), true);
	EXPECT_EQ((*set2).contains(Domino(0, 0)), true);

	delete set2;
	set->addRandom();
	EXPECT_EQ(set->getCurSize(), 3);
	(*set) - set->getDominos()[2];
	//set: [0, 0], [1, 2], [0, 5]
	(*set) + Domino(2, 1);
	EXPECT_EQ((*set)[0].bot, 0);
	EXPECT_EQ((*set)[1].top, 5);
	EXPECT_EQ((*set)[2].top, 2);
	set->sort();
	EXPECT_EQ((*set)[0].bot, 0);
	EXPECT_EQ((*set)[1].top, 2);
	EXPECT_EQ((*set)[2].top, 5);
	set2 = set->getSubset(0);
	EXPECT_EQ((*set2)[0].top, 0);
	EXPECT_EQ((*set2)[1].top, 5);
	EXPECT_EQ(set2->getCurSize(), 2);
	delete set2;
	set2 = set->openByNumber(1);
	EXPECT_EQ((*set2)[0].top, 2);
	EXPECT_EQ((*set2)[0].bot, 1);
	(*set2) = *set;
	EXPECT_EQ((*set2)[0].bot, 0);
	EXPECT_EQ((*set2)[1].top, 2);
	EXPECT_EQ((*set2)[2].top, 5);
	delete set2;
	++(*set);
	EXPECT_EQ(set->getCurSize(), 4);
	domino = (*set)[3];
	EXPECT_EQ(set->contains(domino), true);
	--(*set);
	EXPECT_EQ(set->contains(domino), false);
	EXPECT_EQ(set->getCurSize(), 3);
}

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}