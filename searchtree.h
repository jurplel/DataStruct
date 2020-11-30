#ifndef SEARCHTREE_H
#define SEARCHTREE_H

#include "project.h"

#include <string>
#include <map>

struct PlanetData
{
    bool has_water;
    bool has_terrain;
    float avg_surface_temp;
    float oxygen_percentage;
};

struct Planet
{
    PlanetData data;
};

struct UniverseState
{
    std::vector<Planet> planet_list;
};

class SearchTree : Project
{
public:
    explicit SearchTree();
    void run() override;

protected:
    void generate_universe();

private:
    const int PLANET_MIN_COUNT = 500;
    const int PLANET_MAX_COUNT = 3500;
    // temps in K
    const float PLANET_MAX_TEMP = 1500.0f;
    const float PLANET_MIN_TEMP = 5.0f;

    UniverseState state;

};

#endif // SEARCHTREE_H