/*
 * Solve traveling salesman problem with Simulated Annealing Algorithm
 * http://www.theprojectspot.com/tutorial-post/simulated-annealing-algorithm-for-beginners/6
 * https://en.wikipedia.org/wiki/Simulated_annealing
 */

#include "Tour.hpp"
#include "TourManager.hpp"
#include <cmath>
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

double acceptanceProbability(int energy, int newEnergy, double temperature)
{
    if (newEnergy < energy)
    {
        return 1.0;
    }

    return exp((energy - newEnergy) / temperature);
}

int main(int argc, char *argv[])
{
    TourManager tourManager;

    // Create and add our cities
    City city = City(60, 200);
    tourManager.addCity(city);
    City city2 = City(180, 200);
    tourManager.addCity(city2);
    City city3 = City(80, 180);
    tourManager.addCity(city3);
    City city4 = City(140, 180);
    tourManager.addCity(city4);
    City city5 = City(20, 160);
    tourManager.addCity(city5);
    City city6 = City(100, 160);
    tourManager.addCity(city6);
    City city7 = City(200, 160);
    tourManager.addCity(city7);
    City city8 = City(140, 140);
    tourManager.addCity(city8);
    City city9 = City(40, 120);
    tourManager.addCity(city9);
    City city10 = City(100, 120);
    tourManager.addCity(city10);
    City city11 = City(180, 100);
    tourManager.addCity(city11);
    City city12 = City(60, 80);
    tourManager.addCity(city12);
    City city13 = City(120, 80);
    tourManager.addCity(city13);
    City city14 = City(180, 60);
    tourManager.addCity(city14);
    City city15 = City(20, 40);
    tourManager.addCity(city15);
    City city16 = City(100, 40);
    tourManager.addCity(city16);
    City city17 = City(200, 40);
    tourManager.addCity(city17);
    City city18 = City(20, 20);
    tourManager.addCity(city18);
    City city19 = City(60, 20);
    tourManager.addCity(city19);
    City city20 = City(160, 20);
    tourManager.addCity(city20);

    // std::cout << tourManager.toString() << std::endl;

    double temp = 10000;

    double coolingRate = 0.003;

    // initial random setting.
    Tour currentSolution = Tour(tourManager);
    currentSolution.generateIndividual();
    Tour best = Tour(currentSolution.getTour());

    // draw initial_map.
    cv::Mat initial_map = cv::Mat(cv::Size(256, 256), CV_8UC1, cv::Scalar(0));
    for (int i = 0; i < best.tourSize(); i++)
    {
        cv::circle(initial_map, cv::Point(best.getCity(i).getX(), best.getCity(i).getY()), 5, 255);
    }
    for (int i = 0; i < best.tourSize() - 1; i++)
    {
        cv::line(initial_map, cv::Point(best.getCity(i).getX(), best.getCity(i).getY()),
                 cv::Point(best.getCity(i + 1).getX(), best.getCity(i + 1).getY()), 255);
    }

    cv::imshow("Initial_map", initial_map);
    // cv::waitKey(0);

    std::cout << "Initial solution distance: " << best.getDistance() << std::endl;
    // std::cout << best.toString() << std::endl;

    cv::Mat map = cv::Mat(cv::Size(256, 256), CV_8UC1, cv::Scalar(0));

    /* Get init T0, N0 strictly positive transitions
     * T0 = -(mean(delta)/ ln(P0)), P0 = 0.95, N0 = 500
     */
    const int N0 = 500;
    const double P0 = 0.95;
    const double ln_p0 = log(P0);
    double sum_of_deltas = 0.0;
    for (int i = 0; i < N0;)
    {
        Tour newSolution = Tour(currentSolution.getTour());

        cv::RNG rng;
        rng = cv::RNG(cv::getTickCount());
        int tourPos1 = (int)(newSolution.tourSize() * rng.uniform(0.0, 1.0));
        rng = cv::RNG(cv::getTickCount());
        int tourPos2 = (int)(newSolution.tourSize() * rng.uniform(0.0, 1.0));

        City citySwap1 = newSolution.getCity(tourPos1);
        City citySwap2 = newSolution.getCity(tourPos2);

        newSolution.setCity(tourPos2, citySwap1);
        newSolution.setCity(tourPos1, citySwap2);

        int currentEnergy = currentSolution.getDistance();
        int neighbourEnergy = newSolution.getDistance();

        if (neighbourEnergy > currentEnergy) //strictyly postive transitions
        {
            sum_of_deltas += neighbourEnergy - currentEnergy;
            i++;
        }
    }

    double T0 = - ( sum_of_deltas / N0) / ln_p0;
    std::cout << "Initial temperature: " << T0 << std::endl;
    temp = T0;

    while (temp > 1)
    {
        for (int markovChainLength = 0; markovChainLength < 50; markovChainLength++)
        {
            Tour newSolution = Tour(currentSolution.getTour());

            cv::RNG rng;
            rng = cv::RNG(cv::getTickCount());
            int tourPos1 = (int)(newSolution.tourSize() * rng.uniform(0.0, 1.0));
            rng = cv::RNG(cv::getTickCount());
            int tourPos2 = (int)(newSolution.tourSize() * rng.uniform(0.0, 1.0));

            City citySwap1 = newSolution.getCity(tourPos1);
            City citySwap2 = newSolution.getCity(tourPos2);

            newSolution.setCity(tourPos2, citySwap1);
            newSolution.setCity(tourPos1, citySwap2);

            int currentEnergy = currentSolution.getDistance();
            int neighbourEnergy = newSolution.getDistance();

            rng = cv::RNG(cv::getTickCount());
            if (acceptanceProbability(currentEnergy, neighbourEnergy, temp) > rng.uniform(0.0, 1.0))
            {
                currentSolution = Tour(newSolution.getTour());
                markovChainLength++;
            }
        }

        if (currentSolution.getDistance() < best.getDistance())
        {
            best = Tour(currentSolution.getTour());
        }

        // std::cout << best.getDistance() << std::endl;
        temp *= 1 - coolingRate;

        map = 0;
        for (int i = 0; i < best.tourSize(); i++)
        {
            cv::circle(map, cv::Point(best.getCity(i).getX(), best.getCity(i).getY()), 5, 255);
        }
        for (int i = 0; i < best.tourSize() - 1; i++)
        {
            cv::line(map, cv::Point(best.getCity(i).getX(), best.getCity(i).getY()),
                     cv::Point(best.getCity(i + 1).getX(), best.getCity(i + 1).getY()), 255);
        }

        cv::imshow("Processing", map);
        cv::waitKey(1);
    }

    std::cout << "Final solution distance: " + std::to_string(best.getDistance()) << std::endl;
    std::cout << "Tour: " + best.toString() << std::endl;

    for (int i = 0; i < best.tourSize(); i++)
    {
        cv::circle(map, cv::Point(best.getCity(i).getX(), best.getCity(i).getY()), 5, 255);
    }
    for (int i = 0; i < best.tourSize() - 1; i++)
    {
        cv::line(map, cv::Point(best.getCity(i).getX(), best.getCity(i).getY()),
                 cv::Point(best.getCity(i + 1).getX(), best.getCity(i + 1).getY()), 255);
    }

    cv::imshow("Final map", map);
    cv::waitKey(0);

    return 0;
}
