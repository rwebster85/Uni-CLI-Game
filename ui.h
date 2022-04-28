#pragma once
#include "savedata.hpp"

namespace raw {
    /**
     * Handles outputting UI elements to the console as well as accepting user input.
     */
    class UI
    {
    private:
        /** The maximum width of the UI in characters during rounds. */
        static const int round_ui_width = 92;

        /** The maximum width of the UI in characters in the menu. */
        static const int menu_ui_width = 82;
    public:
        /**
         * Display the stats screen for the given save.
         * 
         * @param save The SaveData object containing the stats
         */
        static void StatsScreen(SaveData save);

        /**
         * Run through the story mode intro screens.
         */
        static void StoryIntro();

        /**
         * Print an image to the screen (ASCII format).
         * 
         * @param ui_name The filename (no extension) of the UI
         * @param new_line_before Output a newline character before the file
         */
        static void PrintUI(std::string ui_name, bool new_line_before = true);

        /**
         * Print an image to the screen (ASCII format).
         * 
         * @param image_name The filename (no extension) of the image
         */
        static void PrintImg(std::string image_name);

        /**
         * Output the main UI section for the round.
         * 
         * @param round The current round number
         * @param rounds The total number of rounds
         * @param category The name of the current category
         */
        static void RoundHeader(int round, int rounds, std::string category);

        /**
         * Output the UI section for the question.
         * 
         * @param question The question to ask
         */
        static void QuestionHeader(std::string question);

        /**
         * Print the story end screen summary.
         * 
         * @param rounds How many rounds were played
         * @param score The user score
         */
        static void StoryEndScreen(int rounds, int score);

        /**
         * Print the main story title screen.
         * 
         * @see Game::days_since_calamity
         * @param days_since_calamity Number of days since the bombs fell
         */
        static void PrintStoryTitleScreen(long int days_since_calamity);

        /**
         * Output the number of days since the bombs fell screen.
         * 
         * @see Game::days_since_calamity
         * @param days_since_calamity Number of days since the bombs fell
         */
        static void PrintDaysSince(long int days_since_calamity);

        /**
         * Print an empty spacer for the UI.
         * 
         * @param in_menu Whether this is in the menu or during play
         */
        static void EmptySpacer(bool in_menu = false);

        /**
         * Print a border spacer for the UI.
         * 
         * @param in_menu Whether this is in the menu or during play
         */
        static void BorderSpacer(bool in_menu = false);

        /**
         * Prints a string with menu borders, spacing is variable to the length of the string.
         * 
         * @param s The string to print
         */
        static void PrintMenuLine(std::string s);

        /** Prints a menu heading, wrapped in hyphens. */
        static void PrintMenuHeading(std::string heading);

        /** Prints a menu heading line of hyphens. */
        static void PrintMenuHeadingLine();

        /**
         * Clear the entire screen.
         */
        static void ClearScreen();

        /**
         * Dumps all input after pressing Enter.
         */
        static void CleanInput();

        /**
         * Pause the console window for microseconds.
         * 
         * @param ms Number of microseconds to wait
         */
        static void Wait(int ms = 2);

        /**
         * Pause the console window for seconds.
         * 
         * @param s Number of seconds to wait
         */
        static void WaitSeconds(int s = 2);

        /**
         * Ask the user to press enter and wait for input.
         * 
         * @param wait_to_show Number of seconds to wait before showing
         * @param skip_pause Whether to skip using system("pause") to provide a better user experience
         */
        static void WaitForEnter(int wait_to_show = 0, bool skip_pause = false);

        /**
         * Output a message to the screen instantly.
         * 
         * @param message The message to output
         * @param new_lines_before Number of newline characters to print before
         * @param new_lines_after Number of newline characters to print after
         * @param space_before Output a space before (aligns with the rest of the UI)
         */
        static void Out(std::string message, int new_lines_before = 0, int new_lines_after = 0, bool space_before = true);

        /**
         * Output a message to the screen, simulated as typing by intermittent pauses.
         *
         * @param message The message to output
         * @param new_lines_before Number of newline characters to print before
         * @param new_lines_after Number of newline characters to print after
         * @param space_before Output a space before (aligns with the rest of the UI)
         * @param ms Number of microseconds to wait after each character
         */
        static void TypeOut(const std::string& message, int new_lines_before = 0, int new_lines_after = 0, bool space_before = true, int ms = 2);

        /**
         * All string output that would usually std::cout is handled by Print.
         * This allows different implementations to have a standard output function.
         * 
         * @param message The message to print
         */
        static void Print(const std::string& message);

        /**
         * All char output that would usually std::cout is handled by Print.
         * This allows different implementations to have a standard output function.
         *
         * @param c The character to print
         */
        static void Print(const char& c);

        /** Requests the player make a menu selection. */
        static void PrintMenuSelect();

        /** Prints the question to the player. */
        static void PrintAskQuestion();

        /**
         * Prints a message across the network using Boost.
         * 
         * @param message The message to print
         */
        static void NetworkPrint(const std::string& message);

        /**
         * Prints a character across the network using Boost.
         * 
         * @param c The character to print
         */
        static void NetworkPrint(const char& c);

        /** Prints a newline character to the network using Boost. */
        static void NetworkPrintNewLine();

        /**
         * Checks whether this is a network play game by checking the
         * RAW_NETWORK preprocessor definition.
         *
         * @see definitions.h
         * @return bool
         */
        static bool IsNetworkGame();

        /**
         * Used to call `std::cout << std::flush` to allow standard output.
         * Different systems could have their own implementation.
         */
        static void Flush();

        /**
         * Returns the user selection/input. Uses CleanInput() after to dump additional inputs.
         * 
         * @return char A single character
         */
        static char GetUserSelection();

        /**
         * Return the user input when more than one character is needed.
         * 
         * @return std::string The user input
         */
        static std::string GetUserInput();
    };
}
