/*
 * Tour.hpp
 * Stores a candidate tour through all cities
 */
#ifndef TOUR_HPP
#define TOUR_HPP

#include <vector>
#include <algorithm>
#include "TourManager.hpp"

class Tour
{
  public:
	std::vector<City> tour;

	int distance = 0;
	TourManager tourManager;

	Tour()
	{
		distance = 0;
	}

	Tour(TourManager IntourManager)
	{
		tourManager = IntourManager;
		for (int i = 0; i < tourManager.numberOfCities(); i++)
		{
			tour.push_back(tourManager.getCity(i));
		}
	}

	Tour(std::vector<City> &tour)
	{
		this->tour = tour;
	}

	std::vector<City> &getTour()
	{
		return tour;
	}

	void generateIndividual()
	{
		std::random_shuffle(tour.begin(), tour.end());
	}

	City getCity(int tourPosition)
	{
		return tour[tourPosition];
	}

	void setCity(int tourPosition, City city)
	{
		tour[tourPosition] = city;
		distance = 0;
	}

	int getDistance()
	{
		if (distance == 0)
		{
			int tourDistance = 0;
			for (int cityIndex = 0; cityIndex < tourSize(); cityIndex++)
			{
				City fromCity = getCity(cityIndex);
				City destinationCity;
				if (cityIndex + 1 < tourSize())
				{
					destinationCity = getCity(cityIndex + 1);
				}
				else
				{
					destinationCity = getCity(0);
				}

				tourDistance += fromCity.distanceTo(destinationCity);
			}
			distance = tourDistance;
		}
		return distance;
	}

	int tourSize()
	{
		return tour.size();
	}

	std::string toString()
	{
		std::string geneString = "|";
		for (int i = 0; i < tourSize(); i++)
		{
			geneString += getCity(i).toString() + "|";
		}
		return geneString;
	}
};

#endif
