#pragma once
#include <string>
#include <boost/serialization/vector.hpp>

namespace raw {
	/** Stores performance data relating to a category */
	class CategoryRecord
	{
	private:
		/** The name of this category. */
		std::string name;

		/** Number of games played for this category. */
		int games_played = 0;

		/** Number of rounds played for this category. */
		int rounds_played = 0;

		/** Total user score for this category. */
		int score_total = 0;

		/** Allow Boost to access private members. */
		friend class boost::serialization::access;

		template<class Archive>
		void serialize(Archive& ar, const unsigned int version);

	public:
		/** Default constructor used by Boost. */
		CategoryRecord() {}

		/** Creates a new category with a given name. */
		CategoryRecord(std::string name);

		/** Sets the name of this category. */
		void SetName(std::string name);

		/** Increases the number of games played by 1. */
		void IncreaseGames();

		/**
		 * Increase the number of rounds played.
		 * 
		 * @param rounds Number of rounds to increase by
		 */
		void IncreaseRounds(int rounds);

		/**
		 * Increase the total score for this category.
		 * 
		 * @param score The score to increase by
		 */
		void IncreaseScore(int score);

		/**
		 * Get the number of games played for this category.
		 * 
		 * @return int Number of games played
		 */
		int GetGames();

		/**
		 * Get the number of rounds played for this category.
		 * 
		 * @return int Number of rounds
		 */
		int GetRounds();

		/**
		 * Get the total score earned for this category.
		 * 
		 * @return int Total score
		 */
		int GetScore();

		/**
		 *  Returns the percentage score for this category.
		 *
		 * @return int Rounded percentage
		 */
		int GetPercent();
	};

	//Boost Template Implementation Code-------------------------------------------------
	template<class Archive>
	void CategoryRecord::serialize(Archive& ar, const unsigned int version)
	{
		ar& name;
		ar& games_played;
		ar& rounds_played;
		ar& score_total;
	}
}
