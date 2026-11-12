#include <iostream>
//   initialize population
//       evaluate population
// while (termination criteria not reached)
//   {
// select solutions for next population
// perform crossover and mutation
// evaluate population
//   }
#include <list>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <map>
#include <set>
using namespace std;

int MAP_BOUNDARY = 1000;
int CITIES_IN_TOUR = 4;

class City
{

private:
  double x;
  double y;
  string name;

public:
  City(double x, double y, string name)
  {
    if (x < 0 || x > MAP_BOUNDARY || y < 0 || y > MAP_BOUNDARY)
    {
      cout << " Coordinates are limited to the range [0, 1000] inclusive (MAP_BOUNDARY)." << endl;

      return;
    }
    this->x = x;
    this->y = y;
    this->name = name;
  }

  double get_distance_between_two_cities(City *anotherCity)
  {

    double dx = x - anotherCity->x;
    double dy = y - anotherCity->y;
    return sqrt(dx * dx + dy * dy);
  }

  string get_name()
  {
    return name;
  }
};

class Tour
{

  vector<City *> cities;
  double fitness;
  bool isElite;

public:
  Tour(City *city, bool isElite = false)
  {
    cities.push_back(city);
    this->isElite = isElite;
  }

  Tour(const Tour &other)
  {
    cities = other.cities;
    fitness = other.fitness;
    isElite = other.isElite;
  }



  void add(City *city)
  {
    if (cities.size() > CITIES_IN_TOUR)
    {
      cout << "A tour can only have " << CITIES_IN_TOUR << " cities." << endl;
      return;
    }
    cities.push_back(city);
    if (cities.size() == CITIES_IN_TOUR)
    {
      determine_fitness(this);
    }
  }

  void set_elite(bool isElite)
  {
    this->isElite = isElite;
  }

  vector<City *> *get_cities()
  {
    return &cities;
  }

  double getFitness()
  {
    return fitness;
  }

  double
  determine_fitness(Tour *aTour)
  {

    double result = 0;
    vector<City *> *citiesPtr = aTour->get_cities();
    for (int i = 0; i < citiesPtr->size() - 1; i++)
    {
      double distance = (*citiesPtr)[i]->get_distance_between_two_cities((*citiesPtr)[i + 1]);
      result += distance;
    }
    fitness = result;
    return result;
  }

  string toString()
  {
    // ABCD
    string result = "";
    for (int i = 0; i < cities.size(); i++)
    {
      result += cities[i]->get_name();
    }
    return result;
  }

  bool operator<(const Tour &other) const
  {
    return fitness < other.fitness;
  }
};

class Population
{
  set<Tour *> tours;

public:
  Population(Tour *tour)
  {
    tours.insert(tour);
  }

  void add(Tour *tour)
  {
    tours.insert(tour);
  }

  set<Tour *> *get_tours()
  {
    return &tours;
  }
  int getSize()
  {
    return tours.size();
  }

  double best_distance() // fittest!
  {
    double bestDistance = 0;
    Tour *previousTour = nullptr;
    for (auto &entry : tours)
    {
      Tour *aTour = entry;
      double distance = aTour->determine_fitness(aTour);
      if (distance < bestDistance || bestDistance == 0)
      {
        bestDistance = distance;
        if (previousTour)
        {
          previousTour->set_elite(false);
        }
        aTour->set_elite(true);
        previousTour = aTour;
      }
    }
    return bestDistance;
  }
};

int POPULATION_SIZE = 32;

void random_shuffle(vector<City *>::iterator begin, vector<City *>::iterator end)
{
  // Fisher-Yates shuffle algorithm
  for (auto it = end - 1; it > begin; --it)
  {
    auto j = begin + rand() % (it - begin + 1);
    std::swap(*it, *j);
  }
}

void initializePopulaiton(Population &aPopulation)
{
  // makesure there are POPULATION_SIZE many tours in aPopulation
  if (aPopulation.getSize() < POPULATION_SIZE)
  {
    for (int i = aPopulation.getSize(); i < POPULATION_SIZE; i++)
    {
      // create a new tour with the same cities but in a different order
      Tour *newTour = new Tour(*(*aPopulation.get_tours()->begin()));
      random_shuffle(newTour->get_cities()->begin(), newTour->get_cities()->end());
      aPopulation.add(newTour);
    }
  }
}

void evaluatePopulaiton(Population &aPopulation)
{
  cout << "Best distance: " << aPopulation.best_distance() << endl;
}

int main()
{
  //   initialize population
  //       evaluate population

  cout << "All good!";
  // Cities_to_visit
  City A(0, 0, "A");
  City B(4, 0, "B");
  City C(3.84, 0.65, "C");
  City D(-.75, -1.63, "D");

  // tour
  Tour aTour(&A);
  aTour.add(&B);
  aTour.add(&C);
  aTour.add(&D);

  Population aPopulation(&aTour);
  initializePopulaiton(aPopulation);
  evaluatePopulaiton(aPopulation);

  // while (termination criteria not reached)
  //   {
  // select solutions for next population
  // perform crossover and mutation
  // evaluate population
  //   }

  const double IMPROVEMENT_FACTOR = 1.2; // 20% improvement
  const int ITERATIONS = 1000;

  int iterations = 1;
  double fitness = 0;
  int PARENT_POOL_SIZE = 2;

  fitness =  aPopulation.best_distance();
  double base_distance = fitness;
  double best_distance = fitness;
  while (
      ((base_distance / best_distance) < IMPROVEMENT_FACTOR) &&
      (iterations < ITERATIONS))
  {
    // select solutions for next population
    Pair < Population , Population> parentsPair = selectParents(aPopulation, PARENT_POOL_SIZE); 

    // perform crossover and mutation
    crossover_and_mutation()

        // evaluate population
    iterations++;
  }

  return 0;
}