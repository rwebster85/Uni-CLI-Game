#pragma once
#include <vector>
#include <string>
#include <map>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/utility.hpp>// Enable serialising std::pair
#include <boost/serialization/map.hpp>
#include "categoryrecord.hpp"

namespace raw {
    class SaveData
    {
    private:
        /** The working set of category records with easy key/value lookup. */
        std::map<std::string, CategoryRecord> category_records;

        /** Allow Boost to access private members. */
        friend class boost::serialization::access;

        template<class Archive>
        void serialize(Archive& ar, const unsigned int version);

        /** The user name for this save. Not implemented in this prototype. */
        std::string save_name;

        /**
         * Creates a new category record.
         * Used by public member UpdateCategory() if the category is in the map.
         *
         * @param name The name of the category
         * @param rounds How many rounds were played
         * @param score The score the player got
         */
        void NewCategory(std::string name, int rounds, int score);

    public:
        SaveData() {}

        /**
         * Get the key/value map of all category records in this save.
         * 
         * @return std::map<std::string,CategoryRecord> The category records
         */
        std::map<std::string, CategoryRecord> GetCategoryRecords();

        /**
         * Updates a given category record with more play data.
         * Uses private member NewCategory() if the category is not in the map.
         * 
         * @param name The name of the category
         * @param rounds How many rounds were played
         * @param score The score the player got
         */
        void UpdateCategory(std::string name, int rounds, int score);

        /**
         * Sets the name for this save user.
         * Currently not implemented in this prototype.
         * 
         * @param name The name to use for this save
         */
        void SetName(std::string name);

        /** Get the name of the save user. */
        std::string GetName();

        /**
         * Get the total number of games played. If none, get's total for all categories.
         * 
         * @param category The name of a category to check. Default none.
         * @return int Games played
         */
        int GetGamesPlayed(std::string category = "");

        /**
         * Get the total number of rounds played. If none, get's total for all categories.
         * 
         * @param category The name of a category to check. Default none.
         * @return int Rounds played
         */
        int GetRoundsPlayed(std::string category = "");

        /**
         * Get the total user score. If none, get's total for all categories.
         * 
         * @param category The name of a category to check. Default none.
         * @return int The user score
         */
        int GetScore(std::string category = "");

        /**
         * Returns the percentage score. If none, get's total for all categories.
         * 
         * @param category The name of a category to check. Default none.
         * @return int Rounded percentage
         */
        int GetPercent(std::string category = "");
    };

    //Boost Template Implementation Code-------------------------------------------------
    template<class Archive>
    void SaveData::serialize(Archive& ar, const unsigned int version)
    {
        ar& category_records;
        ar& save_name;
    }
}
