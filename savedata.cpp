#include <vector>
#include <fstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include "savedata.hpp"
#include "categoryrecord.hpp"

using namespace raw;

std::map<std::string, CategoryRecord> SaveData::GetCategoryRecords()
{
    return category_records;
}

void SaveData::UpdateCategory(std::string name, int rounds, int score)
{
    // Check if the category record exists by name
    if (category_records.find(name) == category_records.end())
    {
        // Does not exist, create a new one.
        NewCategory(name, rounds, score);
    }
    else
    {
        // The category exists, update.
        category_records.at(name).IncreaseGames();
        category_records.at(name).IncreaseRounds(rounds);
        category_records.at(name).IncreaseScore(score);
    }
}

void SaveData::SetName(std::string name)
{
    save_name = name;
}

std::string SaveData::GetName()
{
    return save_name;
}

void SaveData::NewCategory(std::string name, int rounds, int score)
{
    CategoryRecord record(name);
    record.IncreaseGames();
    record.IncreaseRounds(rounds);
    record.IncreaseScore(score);

    category_records.insert({ name, record });
}

int SaveData::GetGamesPlayed(std::string category)
{
    int games_played = 0;

    if (!category.empty())
    {
        auto it = category_records.find(category);
        if (it != category_records.end())
        {
            games_played += it->second.GetGames();
        }
    }
    else
    {
        for (auto i : category_records)
            games_played += i.second.GetGames();
    }

    return games_played;
}

int SaveData::GetRoundsPlayed(std::string category)
{
    int rounds_played = 0;

    if (!category.empty())
    {
        auto it = category_records.find(category);
        if (it != category_records.end())
        {
            rounds_played += it->second.GetRounds();
        }
    }
    else
    {
        for (auto i : category_records)
            rounds_played += i.second.GetRounds();
    }

    return rounds_played;
}

int SaveData::GetScore(std::string category)
{
    int score_total = 0;

    if (!category.empty())
    {
        auto it = category_records.find(category);
        if (it != category_records.end())
        {
            score_total += it->second.GetScore();
        }
    }
    else
    {
        for (auto i : category_records)
            score_total += i.second.GetScore();
    }

    return score_total;
}

int SaveData::GetPercent(std::string category)
{
    int percent = 0;

    if (!category.empty())
    {
        auto it = category_records.find(category);
        if (it != category_records.end())
        {
            percent += it->second.GetPercent();
        }
    }
    else
    {
        int rounds = GetRoundsPlayed();

        if (rounds <= 0)
        {
            return percent;
        }

        int score = GetScore();
        float percf = round(((float)score / (float)rounds) * 100.0);
        percent = static_cast<int>(percf);
    }

    return percent;
}
