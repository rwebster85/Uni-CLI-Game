#include <random>
#include <string>
#include <iostream>
#include "ui.h"
#include "question.h"

using namespace raw;

std::string Question::GetGuess()
{
    std::string guess = UI::GetUserInput();
    return guess;
}

Question::Question(std::string q, std::string q_e, std::string a) :q(q), q_e(q_e), a(a) {}

std::string Question::Ask(bool is_easy) {

    std::string question;

    if (is_easy == true)
    {
        question = q_e;
    }
    else
    {
        question = q;
    }

    UI::QuestionHeader(question);
    
    UI::PrintAskQuestion();

    guess = GetGuess();

    return guess;
}

bool Question::AnswerCorrect(std::string guess)
{
    // Normalise both the question and answer, to do a case insensitive comparison
    std::string n_answer = a;
    std::transform(n_answer.begin(), n_answer.end(), n_answer.begin(), ::toupper);

    std::string n_guess = guess;
    std::transform(n_guess.begin(), n_guess.end(), n_guess.begin(), ::toupper);

    return (n_guess == n_answer);
}
