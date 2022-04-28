#pragma once
#include <vector>
#include <tuple>
#include <string>
#include <boost/serialization/vector.hpp>

namespace raw {
    /** Stores all the information related to a game category. */
    class Category
    {
    private:
        /**
         * Current questions and answers (uppercase).
         * First string is the fully substituted question.
         * Second is the substituted string with vowels still in place.
         * Third is the answer string.
         */
        std::vector<std::tuple<std::string, std::string, std::string>> _data;

        /** The original list of answers from the text file, never modified. */
        std::vector<std::string> _original_answers;

        /** The working answer set to be randomised. */
        std::vector<std::string> _answers;

        /** The name of this category, taken from the file name of the answers text file. */
        std::string _name;

        /** The total number of items in the category. */
        int size = 0;

        /** Allow Boost to access private members. */
        friend class boost::serialization::access;

        /** Boost serialization method. Stores `Category::_name` and `Category::_original_answers` */
        template<class Archive>
        void serialize(Archive& ar, const unsigned int version);

        /**
         * Set the name of the category.
         * Taken from the filename of the category file.
         * 
         * @param name The name for the category
         */
        void SetName(std::string name);

        /**
         * Randomises all the answers in the category.
         * @see SetQuestions()
         */
        void Randomise();

        /** Resets all the answer data aside from `Category::_original_answers` */
        void ResetQuestions();

        /**
         * Whether the letter is a vowel or not.
         * 
         * @param letter 
         * @return bool 
         */
        bool IsVowel(const char& letter);

        /**
         * Perform the substitution of the answer to numbers of the alphabet for output.
         * Example: 'BILBO BAGGINS' would become "2 9 12 2 15   2 1 7 7 9 14 19".
         * Spaces between words are represented as 3 empty spaces.
         * If playing on easy mode, vowels are not replaced.
         * 
         * @param input The answer string
         * @param easy Keep vowels in place
         * @return std::string The substituted question string.
         */
        std::string substitute(std::string& input, bool easy = false);

    public:
        /** Empty default constructor is needed for Boost deserialisation. */
        Category() {};

        /**
         * The category data is set when instantiated..
         * 
         * @see Game::BuildCategories()
         * @param name The category name
         * @param answers An array of the category answers.
         */
        Category(std::string name, std::vector<std::string> answers);

        /**
         * Get the name of this category.
         * 
         * @return std::string Category name.
         */
        std::string GetName();

        /**
         * Get an array of all the answer strings.
         * 
         * @return std::vector<std::string> Answers
         */
        std::vector<std::string> GetAnswers();

        /**
         * Get an array of all the question strings.
         * 
         * @return std::vector<std::string> Array of questions
         */
        std::vector<std::string> GetQuestions();

        /**
         * Set the questions to be asked.
         * Performs `ResetQuestions()` and `Randomise()`, then `substitute()` on
         * each answer before building `Category::_data`.
         */
        void SetQuestions();

        /**
         * Get the current question as an array.
         * [0] = answer, [1] = question (substituted).
         * Used when instantiating a new Question.
         * 
         * @see Question
         * @param position The current question position being asked
         * @return std::vector<std::string> The question
         */
        std::vector<std::string> GetQuestion(int position);

        /** Print all answers in this category. Used for debugging. */
        void PrintAnswers();

        /** Print all questions and answers in this category. Used for debugging. */
        void PrintAll();
    };

    //Boost Template Implementation Code-------------------------------------------------
    template<class Archive>
    void Category::serialize(Archive& ar, const unsigned int version)
    {
        ar& _name;
        ar& _original_answers;
    }
}
