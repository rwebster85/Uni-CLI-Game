#pragma once
#include "category.hpp"

namespace raw {
    /** Handles reading category files and generates a serialised file to work from. */
    class CategoriesBuilder
    {
    private:
        /** The name of the folder inside the program directory where categories are stored. */
        std::string folder;

        /**
         * An array of Category objects created to be passed back to Game.
         * 
         * @see Game::BuildCategories()
         */
        std::vector<Category> categories;

        /**
         * Takes an array of categories and uses Boost to serialise them.
         * 
         * @param cats An array of category objects
         */
        void SerialiseCategories(std::vector<Category> cats);

        /**
         * Attempts to convert serialised categories into an array for storage in the class.
         * 
         * @return std::vector<Category> An array of category objects deserialised by Boost
         */
        std::vector<Category> DeserialiseCategories();

    public:
        /**
         * The constructor accepts the relative folder where the category text files are located.
         * 
         * @param input_folder The folder name, inside the program folder.
         */
        CategoriesBuilder(std::string input_folder);

        /**
         * Builds the category data, each Category object will contain a name and a set of answers.
         * Also actions serialisation and deserialisation of category data.
         * 
         * @see SerialiseCategories()
         * @see DeserialiseCategories()
         */
        void Build();

        /** Retrieves the created categories. */
        std::vector<Category> GetCategories();
    };
}
