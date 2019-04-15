/*
 * TourManager.cpp
 * Holds the cities of a tour
 */
#ifndef TOURMANAGER_HPP
#define TOURMANAGER_HPP

#include <vector>
#include "City.hpp"

class TourManager
{
  public:
	std::vector<City> destinationCities;

	void addCity(City city)
	{
		destinationCities.push_back(city);
	}

	City getCity(int index)
	{
		return destinationCities[index];
	}

	int numberOfCities()
	{
		return destinationCities.size();
	}

	std::string toString()
	{
		std::string geneString = "|";
		for (int i = 0; i < numberOfCities(); i++)
		{
			geneString += getCity(i).toString() + "|";
		}
		return geneString;
	}
};

#endif