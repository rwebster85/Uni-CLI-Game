#include <algorithm>
#include <random>
#include <iostream>
#include "ui.h"
#include "category.hpp"

using namespace raw;

void Category::SetName(std::string name)
{
    // Convert filename underscores to spaces before setting the category name
    std::transform(name.begin(), name.end(), name.begin(), [](char letter) {
        return letter == '_' ? ' ' : letter;
    });
    _name = name;
}

void Category::Randomise()
{
    std::random_device random;
    std::mt19937 generator(random());
    std::shuffle(_answers.begin(), _answers.end(), generator);
}

void Category::ResetQuestions() {
    _data.clear();
    _answers.clear();
    _answers = _original_answers;
}

bool Category::IsVowel(const char& letter)
{
    if (
        letter == 'A'
        || letter == 'E'
        || letter == 'I'
        || letter == 'O'
        || letter == 'U'
        ) {
        return true;
    }
    return false;
}

std::string Category::substitute(std::string& input, bool easy)
{
    std::string question = "";

    std::string the_input = input;

    // Questions are always substituted from capital letters
    std::transform(the_input.begin(), the_input.end(), the_input.begin(), ::toupper);

    for (size_t i = 0; i < the_input.length(); i++)
    {
        if (isspace(the_input[i]))
        {
            // Spaces between words are represented by 3 spaces
            question += "   ";
        }
        else
        {
            if (easy == true && IsVowel(the_input[i]))
            {
                // Vowels are left intact on easy mode.
                question += the_input[i];
            }
            else
            {
                // Convert the letter to a number of the alphabet, where A = 1
                question += std::to_string(static_cast<int>(the_input[i]) - 64);
            }

            // Unless this letter is the end of the line, add an additional space after it
            if (i + 1 != the_input.length()) question += " ";
        }
    }
    return question;
}

Category::Category(std::string name, std::vector<std::string> answers)
{
    SetName(name);
    _original_answers = answers;
}

std::string Category::GetName() { return _name; }

std::vector<std::string> Category::GetAnswers()
{
    std::vector<std::string> answers;
    for (const auto& a : _data)
        answers.push_back(std::get<2>(a));

    return answers;
}

std::vector<std::string> Category::GetQuestions()
{
    std::vector<std::string> answers;
    for (const auto& a : _data)
        answers.push_back(std::get<0>(a));

    return answers;
}

void Category::SetQuestions()
{
    // First reset the data for good measure
    ResetQuestions();

    // Randomise the answer strings
    Randomise();

    // Then we build a vector of answer/question groups
    for (const auto& answer : _answers)
    {
        std::string question = answer;
        std::string subbed = substitute(question);
        std::string subbed_e = substitute(question, true);
        _data.push_back(std::tuple<std::string, std::string, std::string>(subbed, subbed_e, answer));
        size++;
    }
}

std::vector<std::string> Category::GetQuestion(int position)
{
    int map_pos = (position - 1);

    std::tuple<std::string, std::string, std::string> q = _data.at(map_pos);

    std::vector<std::string> question = { std::get<0>(q), std::get<1>(q), std::get<2>(q) };
    return question;
}

void Category::PrintAnswers()
{
    for (const auto& a : _data)
        UI::Out("\n Print Answer: " + std::get<0>(a) + " : " + std::get<2>(a) + "\n");
}

void Category::PrintAll()
{
    std::cout << "\n\n Printing All\n\n";
    std::cout << GetName() << "\n";
    for (const auto& a : _data) {
        std::string out = ("Q: " + std::get<0>(a) + "\nQ_E: " + std::get<1>(a) + "\nA: " + std::get<2>(a) + "\n");
        std::cout << out << "\n";
    }
}
