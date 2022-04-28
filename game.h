#pragma once
#include <vector>
#include "savedata.hpp"
#include "category.hpp"

namespace raw {
    /** Handles all logic of running and progressing the game. */
    class Game
    {
    private:
        /** How many questions are asked each round. */
        const int questions_per_game = 5;

        /**
         * Contains a record of player data.
         * Includes a CategoryRecord for each category the player has encountered.
         */
        SaveData Save;

        /**
         * Loads the SaveData file if present using Boost, otherwise creates new instance of SaveData.
         * @see SaveData
         */
        void LoadGame();

        /** Outputs the SaveData object using Boost to 'saves/savedata.rw' */
        void SaveGame();

        /**
         * Updates the stats for the category just played, saves the game,
         * then clears the rounds and score, ready for the next round if needed.
         * 
         * @see SaveData::UpdateCategory().
         * @see SaveGame()
         */
        void RoundComplete();

        /** The stored categories */
        std::vector<Category> categories;

        /** The current round being played. */
        int round = 0;

        /** The user score for the current round. */
        int score = 0;

        /** Total number of categories available. */
        int categories_count = 0;

        /** The current category index in use. */
        int current_category = -1;

        /** Number of days since the bombs fell. */
        long int days_since_calamity = 0;

        /** Used in a while loop, true as long as the player is playing rounds. */
        bool playing = true;

        /** Build the game categories from the "categories" directory. */
        void BuildCategories();

        /** Randomise all stored categories. */
        void RandomiseCategories();

        /**
         * Returns the current category for this round.
         * 
         * @return Category The category object
         */
        Category GetCurrentCategory();

        /**
         * Move to the next category.
         * If there are no more categories they loop back around.
         */
        void NextCategory();

        /**
         * Start a quick round without any story.
         * 
         * @param easy Whether this is an easy round
         */
        void QuickRound(bool easy = false);

        /** Start a story round. */
        void StoryRound();

        /**
         * Show the story round title screen.
         * 
         * @uses UI::PrintStoryTitleScreen()
         */
        void StoryTitleScreen();

        /**
         * Get the startup warning message.
         * 
         * @return std::string The warning
         */
        std::string StartupWarning();

        /**
         * Get the startup text to display for starting a story mode.
         * 
         * @see Game::ConfirmStartup()
         * @return std::string 
         */
        std::string StartupMessage();

        /**
         * Get the skip intro text to display.
         *
         * @return std::string
         */
        std::string SkipIntroMessage();

        /**
         * Get the round over text showing summary.
         *
         * @return std::string
         */
        std::string PlayEndMessage();

        /**
         * Get the text to display asking to play again.
         *
         * @return std::string
         */
        std::string PlayAgainMessage();

    public:
        /** Constructor begins the game automatically upon instantiation. */
        Game();

        /** Show the main menu with options. */
        void MainMenu();

        /** Handles the story mode play and ending. */
        void StoryStart();

        /**
         * Whether to start the game or not.
         * 
         * @return bool 
         */
        bool ConfirmStartup();

        /**
         * Whether to skip the story intro cutscene.
         * 
         * @return bool 
         */
        bool SkipIntro();

        /** Start a new quick play game. */
        void QuickPlay();

        /**
         * Whether to play another round of quick play.
         * 
         * @return bool 
         */
        bool QuickPlayAgain();

        /**
         * Whether to play another round of story mode.
         * 
         * @return bool 
         */
        bool StoryPlayAgain();

        /**
         * Whether the user wants to play on easy mode.
         * Checks CanPlayOnEasy() before asking the user.
         *
         * @return bool
         */
        bool PlayOnEasy();

        /**
         * Checks whether the player has a score percentage less than 50 for this category.
         * If the user has not played this category before, returns false;
         * 
         * @return bool Whether the user can play on easy
         */
        bool CanPlayOnEasy();

        /**
         * Count how many categories are stored.
         * 
         * @return int Number of categories.
         */
        int CountCategories();

        /** Print all category names. Used for debugging. */
        void PrintCategories();

        /** Set the number of days since the bombs fell. */
        void SetGameDays();

        /**
         * Get the number of days since the bombs fell.
         * 
         * @see Game::days_since_calamity
         * @return int The number of days.
         */
        int GetDaysSinceCalamity();

        /**
         * Checks whether this is a network play game by checking the
         * RAW_NETWORK preprocessor definition.
         * 
         * @see definitions.h
         * @return bool 
         */
        static bool IsNetworkGame();

        /**
         * Prints a message to the server screen if in network mode.
         * 
         * @param message The message to print out
         */
        void NetworkServerMessage(const std::string& message);

        /** Checks whether the network stream has disconnected. */
        bool NetworkDisconnected();
    };
}
