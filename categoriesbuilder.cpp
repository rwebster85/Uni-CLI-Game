#include <fstream>
#include <filesystem>
#include <iostream>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include "categoriesbuilder.h"

using namespace raw;

void CategoriesBuilder::SerialiseCategories(std::vector<Category> cats)
{
    {
        std::ofstream out(folder + "/categories.rw");
        boost::archive::text_oarchive arch(out);
        arch << cats;
    }
}

std::vector<Category> CategoriesBuilder::DeserialiseCategories()
{
    std::vector<Category> cats;

    try
    {
        std::ifstream in(folder + "/categories.rw");
        boost::archive::text_iarchive arch_in(in);
        arch_in >> cats;
    }
    catch (const std::exception& ex)
    {
        throw std::exception(ex.what());
    }

    return cats;
}

CategoriesBuilder::CategoriesBuilder(std::string input_folder) :folder(input_folder) {}

void CategoriesBuilder::Build()
{
    std::vector<Category> temp_cats;

    for (const auto& file : std::filesystem::directory_iterator(folder))
    {
        // Only accept .txt files
        if (file.path().extension() != ".txt")
            continue;

        std::ifstream category_file(file.path());
        if (category_file.is_open()) {

            std::string category_name = file.path().stem().string();
            std::vector<std::string> answers;
            std::string answer;

            while (std::getline(category_file, answer))
                answers.push_back(answer);

            category_file.close();

            Category c(category_name, answers);
            temp_cats.push_back(c);
        }
    }

    if (temp_cats.size() == 0)
    {
        throw std::exception("No category text files found in categories folder.");
    }

    SerialiseCategories(temp_cats);

    // Additional layer of abstraction to ensure serialisation worked.
    categories = DeserialiseCategories();
}

std::vector<Category> CategoriesBuilder::GetCategories()
{
    return categories;
}
