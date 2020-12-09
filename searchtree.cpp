#include "searchtree.h"

#include <random>
#include <algorithm>

SearchTreeNode::SearchTreeNode(const UniverseState state) :  current_planet(state.planet_list.at(state.planet_index)), state(state)
{
    children_initialized = false;
    child_backward = nullptr;
    child_orbit_or_retrieve = nullptr;
    child_forward = nullptr;

    planet_index = state.planet_index;
    std::cout << "Planet index on construction: " << state.planet_index << std::endl;

    // Set travel time backward
    travel_time_backward = current_planet.distance_from_previous;

    // Set travel time forward if we are not at the end
    if (planet_index != static_cast<int>(state.planet_list.size()-1))
        travel_time_forward = state.planet_list.at(planet_index+1).distance_from_previous;

    // Set orbit time
    orbit_time = current_planet.diameter/10000; // 500,000 km planet would take 50 hours, 1000 km planet would take 0.1 hours

    // Set data retrieval state variable
    if (state.data_retrieval_states.contains(planet_index))
        retrieval_state = state.data_retrieval_states.at(planet_index);
    else
        retrieval_state = DataRetrievalState::None;

    std::cout << "Retrieval state on construction: " << static_cast<int>(retrieval_state) << std::endl;

    // Set retrieval time if we are in a state which we know it
    if (retrieval_state == DataRetrievalState::Orbited || retrieval_state == DataRetrievalState::Retrieved)
    {
        retrieval_time = current_planet.retrieve_time;
    }
}

void SearchTreeNode::init_children()
{
    // Build tree
    // Travel backward
    if (travel_time_backward.has_value())
    {
        UniverseState back_state = state;
        back_state.planet_index--;
        back_state.time_taken += travel_time_backward.value();
        SearchTreeNode *ptr = new SearchTreeNode(back_state);
        child_backward = ptr;
    }

    // Orbit or data retrieval
    if (retrieval_state == DataRetrievalState::None)
    {
        UniverseState orbit_state = state;
        orbit_state.time_taken += orbit_time;
        orbit_state.data_retrieval_states.insert_or_assign(planet_index, DataRetrievalState::Orbited);
        SearchTreeNode *ptr = new SearchTreeNode(orbit_state);
        child_orbit_or_retrieve = ptr;
    }
    else if (retrieval_state == DataRetrievalState::Orbited)
    {
        UniverseState retrieve_state = state;
        retrieve_state.time_taken += retrieval_time.value();
        retrieve_state.data_retrieval_states.insert_or_assign(planet_index, DataRetrievalState::Retrieved);
        SearchTreeNode *ptr = new SearchTreeNode(retrieve_state);
        child_orbit_or_retrieve = ptr;
    }

    // Travel forward
    if (travel_time_forward.has_value())
    {
        UniverseState forward_state = state;
        forward_state.planet_index++;
        forward_state.time_taken += travel_time_forward.value();
        SearchTreeNode *ptr = new SearchTreeNode(forward_state);
        child_forward = ptr;
    }

    children_initialized = true;
}

SearchTreeNode &SearchTreeNode::choose_optimal_node()
{
    if (!get_children_initialized())
        init_children();

    Direction optimal_dir = choose_optimal_dir();
    switch (optimal_dir)
    {
    case Direction::Backward: {
        std::cout << "Choosing backward";
        if (child_backward)
            return *child_backward;
        break;
    }
    case Direction::None: {
        std::cout << "Choosing none (orbit/retrieve)" << std::endl;
        if (child_orbit_or_retrieve)
            return *child_orbit_or_retrieve;
        break;
    }
    case Direction::Forward: {
        std::cout << "Choosing forward";
        if (child_forward)
            return *child_forward;
        break;
    }
    }

    throw std::runtime_error("reached the end of the function without doing anything!!!!");
}

Direction SearchTreeNode::choose_optimal_dir()
{
    std::optional<float> total_cost_backward = travel_time_backward;
    std::optional<float> total_cost_current = 0;
    std::optional<float> total_cost_forward = travel_time_forward;

    if (total_cost_backward.has_value())
    {
        total_cost_backward.value() += calculate_effective_time(child_backward, Direction::Backward).value();
    }

    if (auto result = calculate_effective_time(this, Direction::None); result.has_value())
    {
        total_cost_current.value() += result.value();
    }
    else
    {
        total_cost_current.reset();
    }

    if (total_cost_forward.has_value())
    {
        total_cost_forward.value() += calculate_effective_time(child_forward, Direction::Forward).value();
    }

    std::cout << "total time costs:" << std::endl;
    std::cout << total_cost_backward.value_or(-1) << std::endl;
    std::cout << total_cost_current.value_or(-1) << std::endl;
    std::cout << total_cost_forward.value_or(-1) << std::endl;

    std::optional<float> min = std::min({total_cost_backward, total_cost_current, total_cost_forward});
    std::cout << "Min: " << min.value_or(-1) << std::endl;

    if (total_cost_backward == min)
    {
        return Direction::Backward;
    }
    else if (total_cost_forward == min)
    {
        return Direction::Forward;
    }

    return Direction::None;
}

std::optional<float> SearchTreeNode::calculate_effective_time(SearchTreeNode *node, Direction dir)
{
    const float RETRIEVAL_TIME_GUESS = 50.0f;
    float total_cost = 0;
    switch (node->retrieval_state)
    {
    case DataRetrievalState::None: {
        total_cost += node->orbit_time;
        total_cost += RETRIEVAL_TIME_GUESS;
        break;
    }
    case DataRetrievalState::Orbited: {
        total_cost += node->retrieval_time.value();
        break;
    }
    case DataRetrievalState::Retrieved: {
        if (dir == Direction::Backward)
        {
            auto val = calculate_effective_time(node->child_backward, dir);
            total_cost += val.value_or(0);
        }
        else if (dir == Direction::Forward)
        {
            auto val = calculate_effective_time(node->child_forward, dir);
            total_cost += val.value_or(0);
        }
        else
        {
            return std::optional<float>();
        }
        break;
    }
    }

    return total_cost;
}

SearchTree::SearchTree()
{
    generate_universe();
}

void SearchTree::run()
{
    bool success = find_suitable_planet();
}

bool SearchTree::find_suitable_planet()
{
    UniverseState initial_state(planet_list, initial_planet);
    SearchTreeNode root_node(initial_state);
    std::optional<int> suitable_planet_index = one_level_deeper(root_node);

    if (suitable_planet_index.has_value())
    {
        std::cout << "Found suitable planet: " << suitable_planet_index.value();
        return true;
    }
    else
    {
        std::cout << "Failed to find suitable planet";
        return false;
    }
}

std::optional<int> SearchTree::one_level_deeper(SearchTreeNode &node)
{
    if (!node.get_children_initialized())
        node.init_children();

    if (is_goal_state(node))
        return node.get_planet_index();


    std::cout << "One level deeper " << std::endl;
    std::cout << "---" << std::endl;
    SearchTreeNode &optimal_node = node.choose_optimal_node();

    return one_level_deeper(optimal_node);
}

bool SearchTree::is_goal_state(const SearchTreeNode &node)
{
    if (node.get_retrieval_state() == DataRetrievalState::Retrieved)
    {
        const Planet &current_planet = node.get_current_planet();
        return true;
    }

    return false;
}

void SearchTree::generate_universe()
{
    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<int> planet_count_dist(PLANET_MIN_COUNT, PLANET_MAX_COUNT);

    const int planet_count = planet_count_dist(generator);
    std::cout << "GENERATING UNIVERSE CONTAINING " << planet_count << " PLANETS" << std::endl;

    // generate each planet with random attributes
    std::uniform_real_distribution<float> dia_dist(PLANET_MIN_TEMP, PLANET_MAX_TEMP);
    std::uniform_real_distribution<float> dist_dist(PLANET_MIN_TEMP, PLANET_MAX_TEMP); // just let me have this
    std::uniform_real_distribution<float> time_dist(PLANET_MIN_RETRIEVAL_TIME, PLANET_MAX_RETRIEVAL_TIME);
    std::uniform_real_distribution<float> temp_dist(PLANET_MIN_TEMP, PLANET_MAX_TEMP);
    std::uniform_real_distribution<float> percent_dist(0, 100);
    std::uniform_int_distribution<int> bool_dist(0, 1);


    for (int i = 0; i < planet_count; i++)
    {
        planet_list.push_back(Planet {
            dia_dist(generator),
            dist_dist(generator), // heh
            time_dist(generator),
            static_cast<bool>(bool_dist(generator)),
            static_cast<bool>(bool_dist(generator)),
            temp_dist(generator),
            percent_dist(generator)
        });
    }
    planet_list.at(0).distance_from_previous.reset();

    // set current planet
    std::uniform_int_distribution<int> random_planet(0, planet_list.size()-1);
    initial_planet = random_planet(generator);
    std::cout << "STARTING PLANET IS #" << initial_planet+1 << std::endl;
}
