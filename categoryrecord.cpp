#include "categoryrecord.hpp"

using namespace raw;

CategoryRecord::CategoryRecord(std::string name) : name(name) {}

void CategoryRecord::SetName(std::string name) { name = name; }
void CategoryRecord::IncreaseGames() { games_played++; }
void CategoryRecord::IncreaseRounds(int rounds) { rounds_played += rounds; }
void CategoryRecord::IncreaseScore(int score) { score_total += score; }

int CategoryRecord::GetGames() { return games_played; }
int CategoryRecord::GetRounds() { return rounds_played; }
int CategoryRecord::GetScore() { return score_total; }

int CategoryRecord::GetPercent()
{
    int percent = 0;

    int rounds = GetRounds();

    if (rounds <= 0)
    {
        return percent;
    }

    int score = GetScore();
    float percf = round(((float)score / (float)rounds) * 100.0);
    percent = static_cast<int>(percf);

	return percent;
}
