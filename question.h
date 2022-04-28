#pragma once

namespace raw {
    class Question
    {
    private:
        /** The fully substituted question being asked. */
        std::string q;

        /** The vowels visible substituted question being asked. */
        std::string q_e;

        /** The answer to the question being asked. */
        std::string a;

        /** The user's guess. */
        std::string guess;

        /**
         * Get the user's guess via input.
         * 
         * @return std::string The guess.
         */
        std::string GetGuess();

    public:
        /**
         * Stores information about the current question.
         * 
         * @param q The question
         * @param q_e The "easy" question with vowels in place
         * @param a The answer
         */
        Question(std::string q, std::string q_e, std::string a);

        /**
         * Outputs the question and asks the user for their guess.
         * 
         * @return std::string Returns the user's guess from input
         */
        std::string Ask(bool is_easy = false);

        /**
         * Where the guess is correct.
         * Normalises both the guess and answer for comparison.
         * 
         * @param guess The user's guess
         * @return bool 
         */
        bool AnswerCorrect(std::string guess);
    };
}
