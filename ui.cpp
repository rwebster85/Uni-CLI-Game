#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <boost/asio.hpp>
#include "definitions.h"
#include "streamer.h"
#include "ui.h"

using namespace raw;

void UI::StatsScreen(SaveData save)
{
    PrintUI("player_stats");

    std::string games_s = "Games Played: " + std::to_string(save.GetGamesPlayed());
    std::string rounds_s = "Rounds Played: " + std::to_string(save.GetRoundsPlayed());
    std::string score_s = "Total Score: " + std::to_string(save.GetScore());
    std::string score_p = "Score Percent: " + std::to_string(save.GetPercent()) + "%";

    std::vector<std::string> stats = { games_s, rounds_s , score_s, score_p };

    // Print the total stats section
    for (const auto& s : stats)
    {
        PrintMenuLine(s);
    }

    EmptySpacer(true);

    // Now print each category stats
    std::map<std::string, CategoryRecord> records = save.GetCategoryRecords();

    for (const auto& r : records)
    {
        std::string name = r.first;
        std::transform(name.begin(), name.end(), name.begin(), ::toupper);
        PrintMenuHeading(name);
        EmptySpacer(true);

        games_s = "Games Played: " + std::to_string(save.GetGamesPlayed(r.first));
        rounds_s = "Rounds Played: " + std::to_string(save.GetRoundsPlayed(r.first));
        score_s = "Total Score: " + std::to_string(save.GetScore(r.first));
        score_p = "Score Percent: " + std::to_string(save.GetPercent(r.first)) + "%";

        std::vector<std::string> cat_stats = { games_s, rounds_s , score_s, score_p };

        for (const auto& s : cat_stats)
        {
            PrintMenuLine(s);
        }

        EmptySpacer(true);
    }

    BorderSpacer(true);
}

void UI::PrintMenuLine(std::string s)
{
    Out("#");

    double stat_length = s.length();

    int space_before, space_after;

    double space_total = static_cast<double>(menu_ui_width - 2) - stat_length;

    space_before = static_cast<int>(space_total / 2);
    space_after = static_cast<int>(space_total / 2);

    if ((space_before + space_after + stat_length) < static_cast<int>(menu_ui_width - 2))
    {
        space_before++;
    }

    while (space_before > 0)
    {
        Print(' ');
        space_before--;
    }

    Print(s);

    while (space_after > 0)
    {
        Print(' ');
        space_after--;
    }

    Out("#", 0, 1, false);
}

void UI::PrintMenuHeading(std::string heading)
{
    PrintMenuHeadingLine();
    PrintMenuLine(heading);
    PrintMenuHeadingLine();
}

void UI::PrintMenuHeadingLine()
{
    std::string line = "                    ";
    line += "--------------------------------------";
    line += "                    ";
    Out("#");
    Out(line);
    Out("#", 0, 1);
}

void UI::StoryIntro()
{
    // Back story setup
    ClearScreen();
    WaitSeconds(2);

    PrintUI("intro/scene_0");
    WaitForEnter(1);
    ClearScreen();

    PrintUI("intro/scene_1");
    WaitForEnter(1);
    ClearScreen();

    PrintUI("intro/scene_2");
    WaitSeconds(2);

    PrintImg("mushroom_cloud_in_scene");
    WaitSeconds(2);

    PrintUI("intro/scene_3", false);
    WaitForEnter(1);
    ClearScreen();

    PrintUI("intro/scene_4");
    WaitSeconds(2);

    PrintImg("osman_1_scene");

    PrintUI("intro/scene_5", false);
    WaitForEnter(1);
    ClearScreen();

    PrintUI("intro/scene_6");
    WaitForEnter(1);
    ClearScreen();

    PrintUI("intro/scene_7");
    WaitForEnter(1);
    ClearScreen();

    PrintUI("intro/scene_8");
    WaitForEnter(1);
    ClearScreen();

    PrintUI("intro/scene_9");
    WaitSeconds(2);

    PrintUI("intro/scene_10", false);

    PrintUI("intro/scene_11", false);
    WaitForEnter(1);
    ClearScreen();


    // Setting the scene
    PrintUI("intro/scene_12");
    WaitSeconds(2);

    PrintImg("terminal_0_scene");

    PrintUI("intro/scene_13", false);
    WaitForEnter(1);
    ClearScreen();

    PrintUI("intro/scene_14");
    WaitForEnter(1);
    ClearScreen();

    PrintUI("intro/scene_15");
    WaitForEnter(1);
    ClearScreen();

    PrintUI("intro/scene_16");
    WaitForEnter(1);
    ClearScreen();

    PrintUI("intro/scene_17");
    WaitForEnter(1);
    ClearScreen();

    PrintUI("intro/scene_18");
    WaitForEnter(1);
    ClearScreen();

    PrintUI("intro/scene_19");
    WaitForEnter(1);
    ClearScreen();

    PrintUI("intro/scene_20");
    WaitSeconds(3);

}

void UI::PrintUI(std::string ui_name, bool new_line_before)
{
    std::string ui_path = "ui/" + ui_name + ".txt";
    std::fstream file;

    file.open(ui_path, std::ios::in);
    if (file.is_open())
    {
        std::string line;
        if (new_line_before)
        {
            NetworkPrintNewLine();
        }
        while (getline(file, line))
        {
            Out(line, 0, 1);
            Wait(10);
        }
        file.close();
    }
}

void UI::PrintImg(std::string image_name)
{
    std::string image_path = "images/" + image_name + ".txt";
    std::fstream file;

    file.open(image_path, std::ios::in);
    if (file.is_open())
    {
        std::string line;
        while (getline(file, line))
        {
            line = line + "\n";
            Out(line);
            // Each line quickly prints out one after the other, rather than all at once. 
            Wait(10);
        }
        file.close();
    }
}

void UI::RoundHeader(int round, int rounds, std::string category)
{
    PrintUI("rounds/header");

    EmptySpacer();

    int round_space = 40;
    int round_space_loop = round_space;

    Out("#");
    while (round_space_loop > 0)
    {
        Print(' ');
        round_space_loop--;
    }
    std::string round_text = "Round: " + std::to_string(round) + "/" + std::to_string(rounds);
    Print(round_text);
    int round_length = round_text.length();
    int spaces_after = (round_ui_width - round_space - 1 - round_length) - 1;
    while (spaces_after > 0)
    {
        Print(' ');
        spaces_after--;
    }
    Out("#", 0, 1, false);
    EmptySpacer();

    int category_space = 30;
    int category_space_loop = category_space;

    Out("#");
    while (category_space_loop > 0)
    {
        Print(' ');
        category_space_loop--;
    }

    std::string category_text = "Category: " + category;
    Print(category_text);
    int category_length = category_text.length();
    spaces_after = (round_ui_width - category_space - 1 - category_length) - 1;
    while (spaces_after > 0)
    {
        Print(' ');
        spaces_after--;
    }
    Out("#", 0, 1, false);

    EmptySpacer();
}

void UI::QuestionHeader(std::string question)
{
    PrintUI("rounds/question_header", false);

    Out("#");

    double question_length = question.length();

    int space_before, space_after;

    double space_total = static_cast<double>(round_ui_width - 2) - question_length;

    space_before = static_cast<int>(space_total / 2);
    space_after = static_cast<int>(space_total / 2);

    if ((space_before + space_after + question_length) < static_cast<int>(round_ui_width - 2))
    {
        space_before++;
    }

    while (space_before > 0)
    {
        Print(' ');
        space_before--;
    }

    Print(question);

    while (space_after > 0)
    {
        Print(' ');
        space_after--;
    }

    Out("#", 0, 1, false);

    EmptySpacer();
    BorderSpacer();
}

void UI::StoryEndScreen(int rounds, int score)
{
    Print('\n');
    BorderSpacer();
    EmptySpacer();
    Out("#");

    std::string score_text = "Game over. You scored " + std::to_string(score) + " of out " + std::to_string(rounds) + ".";

    double score_length = score_text.length();

    int space_before, space_after;

    double space_total = static_cast<double>(round_ui_width - 2) - score_length;

    space_before = static_cast<int>(space_total / 2);
    space_after = static_cast<int>(space_total / 2);

    if ((space_before + space_after + score_length) < static_cast<int>(round_ui_width - 2))
    {
        space_before++;
    }

    while (space_before > 0)
    {
        Print(' ');
        space_before--;
    }

    Print(score_text);

    while (space_after > 0)
    {
        Print(' ');
        space_after--;
    }

    Out("#", 0, 1, false);
    EmptySpacer();
    BorderSpacer();
}

void UI::PrintStoryTitleScreen(long int days_since_calamity)
{
    PrintImg("osman_1");
    PrintUI("rounds/round_scene_1", false);
    WaitForEnter(1);
    ClearScreen();

    PrintImg("osman_1");
    PrintUI("rounds/round_scene_2", false);
    WaitForEnter(1);
    ClearScreen();

    PrintImg("osman_1");
    PrintDaysSince(days_since_calamity);
    WaitForEnter(1);
    ClearScreen();

    PrintImg("osman_2");
    PrintUI("rounds/round_scene_3", false);
    WaitForEnter(1);
    ClearScreen();

    PrintImg("osman_1");
    PrintUI("rounds/round_scene_4", false);
    WaitForEnter(1);
    ClearScreen();

    PrintImg("osman_2");
    PrintUI("rounds/round_scene_5", false);
    WaitForEnter(1);
    ClearScreen();
}

void UI::PrintDaysSince(long int days_since_calamity)
{
    int space = 21;
    int space_before = 21;

    EmptySpacer();
    Out("#");
    while (space_before > 0)
    {
        Print(' ');
        space_before--;
    }
    std::string calamity_text = "It has been " + std::to_string(days_since_calamity) + " days since the Great Calamity.";
    Print(calamity_text);
    int round_length = calamity_text.length();
    int space_after = (round_ui_width - space - 1 - round_length) - 1;
    while (space_after > 0)
    {
        Print(' ');
        space_after--;
    }
    Out("#", 0, 1, false);
    EmptySpacer();
    BorderSpacer();
}

void UI::EmptySpacer(bool in_menu)
{
    Out("#");
    int spaces = 90;

    if (in_menu == true)
    {
        spaces = 80;
    }

    while (spaces > 0)
    {
        Print(' ');
        spaces--;
    }
    Out("#", 0, 1, false);
}

void UI::BorderSpacer(bool in_menu)
{
    Out("#");
    int spaces = 90;

    if (in_menu == true)
    {
        spaces = 80;
    }

    while (spaces > 0)
    {
        Print('=');
        spaces--;
    }
    Out("#", 0, 1, false);
}

void UI::ClearScreen()
{
    if (IsNetworkGame())
    {
        // Send the clear screen command to the network client
        NetworkPrintNewLine();
        NetworkPrint("!clear!");
        return;
    }

    #if defined(_WIN32)
        system("cls");
    #else
        system("clear");
    #endif
}

void UI::CleanInput()
{
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void UI::Wait(int ms)
{
    std::this_thread::sleep_for(std::chrono::microseconds(ms));
}

void UI::WaitSeconds(int s)
{
    std::this_thread::sleep_for(std::chrono::seconds(s));
}

void UI::WaitForEnter(int wait_to_show, bool skip_pause)
{
    if (wait_to_show > 0)
    {
        WaitSeconds(wait_to_show);
    }

    if (!skip_pause)
    {
        // Although less portable, system("pause") provides a better user experience
        // for handling story scene output. Multiple presses of "enter" don't build up.

        if (IsNetworkGame())
        {
            // Send the command to the client
            NetworkPrint('\n');
            NetworkPrint("!pause!");
            return;
        }

        #if defined(_WIN32)
            Print("\n\n ");
            system("pause");
            return;
        #endif
    }

    TypeOut("Press ENTER to continue...", 2, 0);
    CleanInput();
}

void UI::Out(std::string message, int new_lines_before, int new_lines_after, bool space_before)
{
    while (new_lines_before > 0)
    {
        Print('\n');
        new_lines_before--;
    }

    if (space_before == true)
        Print(' ');

    Print(message);

    while (new_lines_after > 0)
    {
        Print('\n');
        new_lines_after--;
    }

    if (IsNetworkGame())
    {
        Flush();
        if (new_lines_after == 0)
        {
            new_lines_after = 1;
        }
    }
}

void UI::TypeOut(const std::string& message, int new_lines_before, int new_lines_after, bool space_before, int ms)
{
    if (IsNetworkGame())
    {
        Out(message, new_lines_before, new_lines_after, space_before);
        return;
    }

    while (new_lines_before > 0)
    {
        Print('\n');
        new_lines_before--;
    }

    if (space_before == true)
        Print(' ');

    for (size_t i = 0; i < message.size(); ++i)
    {
        // Print out a single character of the message string immediately, then wait the specified microseconds
        Print(message[i]);
        Flush();
        Wait(ms);
    }

    while (new_lines_after > 0)
    {
        Print("\n");
        new_lines_after--;
    }
}

void UI::Print(const std::string& message)
{
    if (IsNetworkGame())
    {
        std::string m = static_cast<std::string>(message);
        NetworkPrint(m);
    }
    else
    {
        std::cout << message;
    } 
}

void UI::Print(const char& c)
{
    if (IsNetworkGame())
    {
        if (c == '\n')
        {
            NetworkPrintNewLine();
        }
        else
        {
            NetworkPrint(c);
        }
    }
    else
    {
        if (c == '\n')
        {
            std::cout << std::endl;
        }
        else
        {
            std::cout << c;
        }
    }  
}

void UI::PrintMenuSelect()
{
    if (IsNetworkGame())
    {
        Streamer* streamer = Streamer::GetStreamer();
        tcp::iostream* Stream = streamer->GetStream();

        // Send the command to the client
        *Stream << '\n';
        *Stream << "!menuselect!";
    }
    else
    {
        TypeOut("Make your selection: ", 1);
    }
}

void UI::PrintAskQuestion()
{
    if (IsNetworkGame())
    {
        Streamer* streamer = Streamer::GetStreamer();
        tcp::iostream* Stream = streamer->GetStream();
        *Stream << '\n';
        *Stream << "!question!";
    }
    else
    {
        std::string output_a = "Answer: ";
        TypeOut(output_a, 1);
    }
}

void UI::Flush()
{
    if (IsNetworkGame())
    {
        Streamer* streamer = Streamer::GetStreamer();
        tcp::iostream* Stream = streamer->GetStream();
        *Stream << std::flush;
    }
    else
    {
        std::cout << std::flush;
    }
}

char UI::GetUserSelection()
{
    char input;
    std::string input_s;

    if (IsNetworkGame())
    {
        // Send the command to the client
        NetworkPrint('\n');
        NetworkPrint("!char!\n");

        Streamer* streamer = Streamer::GetStreamer();
        tcp::iostream* Stream = streamer->GetStream();
        *Stream >> input;
        Stream->ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    else
    {
        std::cin.sync();
        std::cin >> input;
        CleanInput();
    }

    return input;
}

std::string UI::GetUserInput()
{
    std::string input;

    if (IsNetworkGame())
    {
        // Send the command to the client
        NetworkPrint('\n');
        NetworkPrint("!input!\n");

        Streamer* streamer = Streamer::GetStreamer();
        tcp::iostream* Stream = streamer->GetStream();
        std::getline(*Stream, input);
        Stream->clear();

        //First intial response may be empty due to buffer.
        if (input.length() <= 0)
        {
            std::getline(*Stream, input);
            Stream->clear();
        }

    }
    else
    {
        std::cin.sync();
        std::getline(std::cin, input);
        std::cin.clear();
    }

    return input;
}

// NETWORK FUNCTIONS
void UI::NetworkPrint(const std::string& message)
{
    Streamer* streamer = Streamer::GetStreamer();
    tcp::iostream* Stream = streamer->GetStream();
    *Stream << message;
}

void UI::NetworkPrint(const char& c)
{
    Streamer* streamer = Streamer::GetStreamer();
    tcp::iostream* Stream = streamer->GetStream();
    *Stream << c;
}

void UI::NetworkPrintNewLine()
{
    if (IsNetworkGame())
    {
        Streamer* streamer = Streamer::GetStreamer();
        tcp::iostream* Stream = streamer->GetStream();
        *Stream << '\n';
    }
}

bool UI::IsNetworkGame()
{
#if defined(RAW_NETWORK) && (RAW_NETWORK == true)
    return true;
#else
    return false;
#endif
}
