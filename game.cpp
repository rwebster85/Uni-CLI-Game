// Requires C++ 17
#include <iostream>
#include <fstream>
#include <filesystem>
#include <random>
#include <boost/asio.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include "definitions.h"
#include "streamer.h"
#include "savedata.hpp"
#include "category.hpp"
#include "question.h"
#include "ui.h"
#include "categoriesbuilder.h"
#include "game.h"

using namespace raw;

Game::Game()
{
    LoadGame();

    if (IsNetworkGame())
    {
        NetworkServerMessage("Waiting for client to connect...");
        std::string connected;
        {
            Streamer* streamer = Streamer::GetStreamer();
            tcp::iostream* Stream = streamer->GetStream();
            // This code pauses the program until it receives a string.
            *Stream >> connected;
            Stream->clear();

            NetworkServerMessage("Client has now connected.");
        }
    }

    UI::TypeOut(StartupWarning(), 1);
    UI::WaitForEnter();

    // For good measure
    UI::ClearScreen();

    BuildCategories();
    RandomiseCategories();

    MainMenu();
}

void Game::LoadGame()
{
    if (!std::filesystem::exists("saves/savedata.rw"))
    {
        // No save data file exist. Create a new save instance.
        // Only gets saved to a file after a round is complete.
        SaveData new_save;
        Save = new_save;
        return;
    }

    try
    {
        std::ifstream in("saves/savedata.rw");
        boost::archive::text_iarchive arch_in(in);
        arch_in >> Save;
    }
    catch (const std::exception&)
    {
        throw std::exception("The save file 'saves/savedata.rw' may be corrupt.");
    }
}

void Game::SaveGame()
{
    UI::TypeOut("Saving...", 1);

    if (!std::filesystem::exists("saves"))
    {
        // No save folder exists. Create it.
        UI::TypeOut("Creating save directory...");
        std::filesystem::create_directory("saves");
    }

    {
        std::ofstream ofs("saves/savedata.rw");
        boost::archive::text_oarchive arch(ofs);
        arch << Save;
    }
    UI::TypeOut("Save complete.", 0, 2);
}

void Game::RoundComplete()
{
    Category CurrentCategory = GetCurrentCategory();
    std::string category_name = CurrentCategory.GetName();
    Save.UpdateCategory(category_name, round, score);

    SaveGame();

    round = 0;
    score = 0;
}

void Game::BuildCategories()
{
    if (!std::filesystem::exists("categories"))
    {
        throw std::exception("No categories folder found in program folder.");
    }

    CategoriesBuilder Builder("categories");
    Builder.Build();

    categories = Builder.GetCategories();
    categories_count = categories.size();

    if (categories_count <= 0)
    {
        throw std::exception("No categories were created.");
    }
}

void Game::RandomiseCategories()
{
    std::random_device random;
    std::mt19937 generator(random());
    std::shuffle(categories.begin(), categories.end(), generator);
}

Category Game::GetCurrentCategory() { return categories.at(current_category); }

void Game::NextCategory()
{
    current_category++;

    if (current_category > (categories_count - 1))
    {
        // We've ran out of categories. Loop back around to the first one.
        current_category = 0;
    }

    // Always randomise the category answers when starting a new category
    Category CurrentCategory = GetCurrentCategory();
    CurrentCategory.SetQuestions();
    categories.at(current_category) = CurrentCategory;
}

void Game::QuickRound(bool easy)
{
    round++;

    Category CurrentCategory = GetCurrentCategory();

    std::string category_name = CurrentCategory.GetName();

    UI::RoundHeader(round, questions_per_game, category_name);

    std::vector<std::string> current_question = CurrentCategory.GetQuestion(round);
    Question question(current_question[0], current_question[1], current_question[2]);
    
    std::string guess = question.Ask(easy);

    if (IsNetworkGame())
    {
        if (guess == "!")
        {
            NetworkServerMessage("Player did not guess anything.");
        }
        else
        {
            NetworkServerMessage("Player Guessed: '" + guess + "'.");
        }
    }

    if (question.AnswerCorrect(guess))
    {
        if (IsNetworkGame())
        {
            NetworkServerMessage("Answer was correct.");
        }

        score += 1;
        UI::TypeOut("That's the right answer!", 1);
    }
    else
    {
        UI::TypeOut("Sorry, the correct answer is " + current_question[2] + ".", 1);
    }

    UI::WaitForEnter();
}

void Game::StoryRound()
{
    round++;

    Category CurrentCategory = GetCurrentCategory();

    std::string category_name = CurrentCategory.GetName();

    UI::RoundHeader(round, questions_per_game, category_name);

    std::vector<std::string> current_question = CurrentCategory.GetQuestion(round);
    Question question(current_question[0], current_question[1], current_question[2]);

    std::string guess = question.Ask();

    if (IsNetworkGame())
    {
        if (guess == "!")
        {
            NetworkServerMessage("Player did not guess anything.");
        }
        else
        {
            NetworkServerMessage("Player Guessed: '" + guess + "'.");
        }
    }

    if (question.AnswerCorrect(guess))
    {
        if (IsNetworkGame())
        {
            NetworkServerMessage("Answer was correct.");
        }

        score += 1;
        UI::TypeOut("That's the right answer!", 1);
    }
    else
    {
        UI::TypeOut("Sorry, the correct answer is " + current_question[2] + ".", 1);
    }

    UI::WaitForEnter();
}

void Game::StoryTitleScreen()
{
    UI::PrintStoryTitleScreen(days_since_calamity);
}

std::string Game::StartupWarning()
{
    return "For best results, this program should be run in fullscreen,\n on a device capable of displaying at least 120 characters per row.";
}

std::string Game::StartupMessage()
{
    return "Initialise the Osman Protocol? (Y/N): ";
}
std::string Game::SkipIntroMessage()
{
    return "Skip intro sequence? (Y/N): ";
}

std::string Game::PlayEndMessage()
{
    std::string play_complete = "Game over. You scored ";
    play_complete += std::to_string(score);
    play_complete += " out of ";
    play_complete += std::to_string(questions_per_game);
    play_complete += ".";
    return play_complete;
}

std::string Game::PlayAgainMessage()
{
    return "Would you like to try again? (Y/N): ";
}

void Game::MainMenu()
{
    bool in_menu = true;

    while (in_menu)
    {
        if (IsNetworkGame())
        {
            NetworkServerMessage("Printing menu screen.");
        }

        UI::ClearScreen();
        UI::PrintUI("main_splash");
        UI::PrintMenuSelect();

        char select = UI::GetUserSelection();

        if (select == '1')
        {
            // Quick Play
            QuickPlay();
        }
        else if (select == '2')
        {
            // Story Mode
            StoryStart();
        }
        else if (select == '3')
        {
            // Player Stats
            UI::ClearScreen();
            UI::StatsScreen(Save);
            UI::WaitForEnter();
            continue;
        }
        else if (select == '4')
        {
            // How to play
            UI::ClearScreen();
            UI::PrintUI("how_to_play");
            UI::WaitForEnter();
            continue;
        }
        else if (select == '5')
        {
            // Exit
            in_menu = false;
            UI::ClearScreen();
            UI::PrintUI("end_splash");
            UI::WaitForEnter();

            if (IsNetworkGame())
            {
                UI::NetworkPrint('\n');
                UI::NetworkPrint("!exit!\n");
            }

            return;
        }

        if (IsNetworkGame())
        {
            if (NetworkDisconnected())
            {
                in_menu = false;
                NetworkServerMessage("The client disconnected. Press enter to close.");
                std::cin.ignore();
                return;
            }
        }

    }

}

void Game::StoryStart()
{
    if (IsNetworkGame())
    {
        NetworkServerMessage("Player selected story mode.");
    }

    playing = true;

    UI::ClearScreen();

    SetGameDays();

    if (SkipIntro() == false)
    {
        if (IsNetworkGame())
        {
            NetworkServerMessage("Playing the story mode intro.");
        }
        UI::ClearScreen();
        UI::StoryIntro();
    }
    else
    {
        UI::ClearScreen();
    }

    if (ConfirmStartup() == true)
    {
        if (IsNetworkGame())
        {
            NetworkServerMessage("Player is now playing story mode.");
        }

        UI::ClearScreen();
        //StoryTitleScreen();

        while (playing)
        {
            if (IsNetworkGame())
            {
                NetworkServerMessage("\nSelecting a new category.");
            }

            NextCategory();

            while (questions_per_game > round)
            {
                if (IsNetworkGame())
                {
                    if (NetworkDisconnected())
                    {
                        playing = false;
                        NetworkServerMessage("The client disconnected. Press enter to close.");
                        std::cin.ignore();
                        return;
                    }
                    NetworkServerMessage("Starting a new story round.");
                }
                UI::ClearScreen();
                StoryRound();
            }

            UI::TypeOut(std::to_string(score), 1, 1);

            int score_percent = static_cast<int>((score * 100.0) / questions_per_game);
            UI::ClearScreen();

            if (IsNetworkGame())
            {
                NetworkServerMessage(
                    "Story round has now ended. The player scored "
                    + std::to_string(score)
                    + "/"
                    + std::to_string(questions_per_game)
                );
            }

            UI::StoryEndScreen(questions_per_game, score);
            UI::WaitForEnter(1);
            UI::ClearScreen();

            if (score == 0)
            {
                UI::PrintUI("rounds/round_over_0");
                UI::WaitForEnter(1);
                UI::ClearScreen();
            }
            else if (score_percent == 100)
            {
                UI::PrintUI("rounds/round_over_100");
                UI::WaitForEnter(1);
                UI::ClearScreen();
            }
            else
            {
                UI::PrintUI("rounds/round_over_message");
                UI::WaitForEnter(1);
                UI::ClearScreen();
            }

            RoundComplete();

            playing = StoryPlayAgain();
        }
    }
}

bool Game::ConfirmStartup()
{
    UI::TypeOut(StartupMessage(), 1);
    char start = UI::GetUserSelection();
    return (toupper(start) == 'Y');
}

bool Game::SkipIntro()
{
    UI::TypeOut(SkipIntroMessage(), 1);
    char skip = UI::GetUserSelection();
    return (toupper(skip) == 'Y');
}

void Game::QuickPlay()
{
    if (IsNetworkGame())
    {
        NetworkServerMessage("Player selected quick play mode.");
    }

    bool easy = false;

    playing = true;

    while (playing)
    {
        if (IsNetworkGame())
        {
            NetworkServerMessage("\nSelecting a new category.");
        }

        easy = false;

        NextCategory();

        UI::ClearScreen();

        if (CanPlayOnEasy())
        {
            // Offer the easy mode to the user
            easy = PlayOnEasy();
        }

        while (questions_per_game > round)
        {
            if (IsNetworkGame())
            {
                if (NetworkDisconnected())
                {
                    playing = false;
                    NetworkServerMessage("The client disconnected. Press enter to close.");
                    std::cin.ignore();
                    return;
                }
                NetworkServerMessage("Starting a new quick round.");
            }

            UI::ClearScreen();
            QuickRound(easy);
        }

        if (IsNetworkGame())
        {
            NetworkServerMessage(
                "Quick play round has now ended. The player scored "
                + std::to_string(score)
                + "/"
                + std::to_string(questions_per_game)
            );
        }

        UI::ClearScreen();
        UI::TypeOut(PlayEndMessage(), 1, 2);

        RoundComplete();

        playing = QuickPlayAgain();
    }
}

bool Game::CanPlayOnEasy()
{
    Category CurrentCategory = GetCurrentCategory();
    std::string category_name = CurrentCategory.GetName();
    int rounds_played = Save.GetRoundsPlayed(category_name);
    int score_percent = Save.GetPercent(category_name);
    return (rounds_played > 0 && score_percent < 50);
}

bool Game::PlayOnEasy()
{
    Category CurrentCategory = GetCurrentCategory();
    std::string category_name = CurrentCategory.GetName();

    std::string next_round = "The next category is " + category_name + ".";
    std::string play_easy = "You've struggled with this category before. Would like to play on easy? Y/N: ";
    UI::TypeOut(next_round, 1);
    UI::TypeOut(play_easy, 1);
    char easy = UI::GetUserSelection();
    return (toupper(easy) == 'Y');
}

bool Game::QuickPlayAgain()
{
    UI::TypeOut(PlayAgainMessage(), 1);
    char again = UI::GetUserSelection();
    return (toupper(again) == 'Y');
}

bool Game::StoryPlayAgain()
{
    UI::PrintUI("rounds/round_over_again");
    UI::TypeOut(PlayAgainMessage(), 1);
    char again = UI::GetUserSelection();
    return (toupper(again) == 'Y');
}

int Game::CountCategories() { return categories.size(); }

void Game::PrintCategories()
{
    for (auto c : categories)
        UI::Out(c.GetName() + "\n");
}

void Game::SetGameDays()
{
    // Now time unix timestamp
    std::time_t unix_now = std::time(nullptr);

    // The "Calamity" happened on 23rd October 2037.
    // This game takes place exactly 150 years later.
    days_since_calamity = 54786;
}

int Game::GetDaysSinceCalamity()
{
    return days_since_calamity;
}

bool Game::IsNetworkGame()
{
#if defined(RAW_NETWORK) && (RAW_NETWORK == true)
    return true;
#else
    return false;
#endif
}

void Game::NetworkServerMessage(const std::string& message)
{
    if (IsNetworkGame() == false)
    {
        return;
    }

    std::cout << message << std::endl;
}

bool Game::NetworkDisconnected()
{
    Streamer* streamer = Streamer::GetStreamer();
    tcp::iostream* Stream = streamer->GetStream();

    if (Stream->fail() == true)
    {
        return true;
    }

    return false;
}
