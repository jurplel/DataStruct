#include "searchtree.h"

#include <random>

SearchTree::SearchTree()
{
    generate_universe();
}

void SearchTree::run()
{

}

void SearchTree::generate_universe()
{
    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<int> planet_count_dist(PLANET_MIN_COUNT, PLANET_MAX_COUNT);

    const int planet_count = planet_count_dist(generator);
    std::cout << "GENERATING UNIVERSE CONTAINING " << planet_count << " PLANETS" << std::endl;

    std::uniform_real_distribution<float> temp_dist(PLANET_MIN_TEMP, PLANET_MAX_TEMP);
    std::uniform_real_distribution<float> percent_dist(0, 100);
    std::uniform_int_distribution<int> bool_dist(0, 1);

    for (int i = 0; i <= planet_count; i++)
    {
        state.planet_list.push_back(Planet {
            PlanetData {
                bool_dist(generator),
                bool_dist(generator),
                temp_dist(generator),
                percent_dist(generator)
            }
        });
    }
    
}