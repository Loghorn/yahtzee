#include "ScoreCalculator.h"
#ifdef CHECK_LEAKS
#include "../utils/debug-new/DebugNew.h"
#endif

#include <numeric>

using namespace std;

ScoreCalculator::ScoreCalculator(unsigned short _maxDiceValue)
	: maxDiceValue(_maxDiceValue)
{
}

ScoreCalculator::~ScoreCalculator()
{
}

vector<unsigned short> CalculateRanks(const vector<Die>& dice, unsigned short maxDiceValue)
{
	vector<unsigned short> ranks(maxDiceValue);
	for (const auto& die : dice)
	{
		ranks[die.value - 1] += 1;
	}
	return ranks;
}

void CalculateHistogram(const vector<unsigned short>& ranks, Histogram& histogram)
{
	for (unsigned short i = 0; i < ranks.size(); ++i)
	{
		if (ranks[i] > 0)
			histogram[ranks[i]].push_front(i + 1);
	}
}

inline bool histogram_has(const Histogram& histogram, unsigned short val)
{
	return histogram.find(val) != end(histogram);
}

inline unsigned short Score(unsigned short score, bool isFirstShot, unsigned short extraIfFirstRoll = 5)
{
	return isFirstShot ? score + extraIfFirstRoll : score;
}

void ScoreCalculator::CheckScore(const std::vector<Die>& dice, bool isFirstShort, ScoreTable& currentTable) const
{
	auto ranks = CalculateRanks(dice, maxDiceValue);
	Histogram histogram;
	CalculateHistogram(ranks, histogram);

	for (unsigned short i = 1; i <= maxDiceValue; ++i)
	{
		auto val = ranks[i - 1];
		if (val)
		{
			currentTable.AssignScoreIfNotAssigned(DieValueToScore(i), val * i);
		}
	}

	if (histogram_has(histogram, 2) && histogram_has(histogram, 3))
	{
		currentTable.AssignScoreIfNotAssigned(Scores::full, Score(30, isFirstShort));
	}

	if (histogram_has(histogram, 4))
	{
		currentTable.AssignScoreIfNotAssigned(Scores::poker, Score(40, isFirstShort));
	}

	if ((histogram.size() == 1) && histogram_has(histogram, 1))
	{
		auto dice = histogram.at(1);
		dice.sort();
		adjacent_difference(begin(dice), end(dice), begin(dice));
		if (all_of(++begin(dice), end(dice), bind(equal_to<unsigned short>(), placeholders::_1, 1)))
			currentTable.AssignScoreIfNotAssigned(Scores::straight, Score(20, isFirstShort));
	}

	if (histogram_has(histogram, 5))
	{
		currentTable.AssignScoreIfNotAssigned(Scores::yahtzee, Score(50, isFirstShort));
	}
}
