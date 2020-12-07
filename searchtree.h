#ifndef SEARCHTREE_H
#define SEARCHTREE_H

#include "project.h"

#include <optional>
#include <memory>
#include <string>
#include <map>

struct Planet
{    
    float diameter;
    std::optional<float> distance_from_previous;
    float retrieve_time;

    bool has_water;
    bool has_terrain;
    float avg_surface_temp;
    float oxygen_percentage;
};

enum class DataRetrievalState
{
    None,
    Orbited,
    Retrieved
};

enum class Direction
{
    None,
    Forward,
    Backward
};

class UniverseState
{
public:
    explicit UniverseState(const std::vector<Planet> &planet_list, int current_planet) : planet_list(planet_list), current_planet(current_planet) {};
    // In our one-dimensional universe, travel will only be possible (or optimal, at least) between adjacent planets.
    const std::vector<Planet> &planet_list;
    int current_planet;
    std::unordered_map<int, DataRetrievalState> data_retrieval_states;
    float time_taken;
};

enum class Action
{
    Travel,
    Orbit,
    Data
};

class SearchTreeNode
{
public:
    explicit SearchTreeNode(const UniverseState &state);

    Direction choose_optimal_node();
    static std::optional<float> calculate_effective_time(const std::unique_ptr<SearchTreeNode> &node, Direction dir);

private:
    int planet_index;
    const Planet &current_planet;
    const UniverseState &state;
    // time will be measured in hours for this scenario
    std::optional<float> travel_time_backward;
    std::optional<float> travel_time_forward;
    float orbit_time;
    std::optional<float> retrieval_time;

    std::unique_ptr<SearchTreeNode> child_backward;
    std::unique_ptr<SearchTreeNode> child_orbit_or_retrieve;
    std::unique_ptr<SearchTreeNode> child_forward;
    DataRetrievalState retrieval_state;
};

class SearchTree : Project
{
public:
    explicit SearchTree();
    void run() override;

    std::optional<int> find_suitable_planet();
    std::optional<int> one_level_deeper(SearchTreeNode &node);
    static bool is_goal_state(const SearchTreeNode &node);

protected:
    void generate_universe();

private:
    const int PLANET_MIN_COUNT = 500;
    const int PLANET_MAX_COUNT = 3500;
    // temps in K
    const float PLANET_MIN_TEMP = 5.0f;
    const float PLANET_MAX_TEMP = 1500.0f;
    // diameter in km
    const float PLANET_MIN_DIAMETER = 1000.0f;
    const float PLANET_MAX_DIAMETER = 500000.0f;
    // distance in million km
    const float PLANET_MIN_DISTANCE = 20.0f;
    const float PLANET_MAX_DISTANCE = 500.0f;
    // time in hours
    const float PLANET_MIN_RETRIEVAL_TIME = 0.1f;
    const float PLANET_MAX_RETRIEVAL_TIME = 100.0f;

    std::vector<Planet> planet_list;
    int initial_planet;
};

#endif // SEARCHTREE_H
