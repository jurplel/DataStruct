#include "searchtree.h"

#include <random>
#include <algorithm>
#include <limits>
#include <chrono>

SearchTreeNode::SearchTreeNode(const UniverseState state) :  current_planet(state.planet_list.at(state.planet_index)), state(state)
{
    children_initialized = false;
    child_backward = nullptr;
    child_orbit_or_retrieve = nullptr;
    child_forward = nullptr;

    planet_index = state.planet_index;

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
        child_backward.reset(ptr);
    }

    // Orbit or data retrieval
    if (retrieval_state == DataRetrievalState::None)
    {
        UniverseState orbit_state = state;
        orbit_state.time_taken += orbit_time;
        orbit_state.data_retrieval_states.insert_or_assign(planet_index, DataRetrievalState::Orbited);
        SearchTreeNode *ptr = new SearchTreeNode(orbit_state);
        child_orbit_or_retrieve.reset(ptr);
    }
    else if (retrieval_state == DataRetrievalState::Orbited)
    {
        UniverseState retrieve_state = state;
        retrieve_state.time_taken += retrieval_time.value();
        retrieve_state.data_retrieval_states.insert_or_assign(planet_index, DataRetrievalState::Retrieved);
        SearchTreeNode *ptr = new SearchTreeNode(retrieve_state);
        child_orbit_or_retrieve.reset(ptr);
    }

    // Travel forward
    if (travel_time_forward.has_value())
    {
        UniverseState forward_state = state;
        forward_state.planet_index++;
        forward_state.time_taken += travel_time_forward.value();
        SearchTreeNode *ptr = new SearchTreeNode(forward_state);
        child_forward.reset(ptr);
    }

    children_initialized = true;
}

std::unique_ptr<SearchTreeNode> &SearchTreeNode::choose_optimal_node()
{
    if (!get_children_initialized())
        init_children();

    Direction optimal_dir = choose_optimal_dir();
    switch (optimal_dir)
    {
    case Direction::Backward: {
        std::cout << "Choosing backward" << std::endl;
        return child_backward;
        break;
    }
    case Direction::None: {
        std::cout << "Choosing none (orbit/retrieve)" << std::endl;
        return child_orbit_or_retrieve;
        break;
    }
    case Direction::Forward: {
        std::cout << "Choosing forward" << std::endl;
        return child_forward;
        break;
    }
    }

    throw std::runtime_error("reached the end of choose_optimal_node without returning anything!!!!");
}

Direction SearchTreeNode::choose_optimal_dir()
{
    std::optional<float> total_cost_backward = travel_time_backward;
    std::optional<float> total_cost_current = 0;
    std::optional<float> total_cost_forward = travel_time_forward;

    if (total_cost_backward.has_value() && child_backward)
    {
        if (std::optional<float> result = calculate_effective_time(*child_backward, Direction::Backward); result.has_value())
            total_cost_backward.value() += result.value();
    }

    if (auto result = calculate_effective_time(*this, Direction::None); result.has_value())
    {
        total_cost_current.value() += result.value();
    }
    else
    {
        total_cost_current.reset();
    }

    if (total_cost_forward.has_value() && child_forward)
    {
        if (std::optional<float> result = calculate_effective_time(*child_forward, Direction::Forward); result.has_value())
            total_cost_forward.value() += result.value();
    }

    std::cout << "Effective cost of:" << std::endl;
    std::cout << "Going backward: " << total_cost_backward.value_or(-1) << std::endl;
    std::cout << "Orbit/Retrieve: " << total_cost_current.value_or(-1) << std::endl;
    std::cout << "Going forward: " << total_cost_forward.value_or(-1) << std::endl;


    // Use value_or with the maximum float value so any undefined values dont get picked.
    std::optional<float> min = std::min({total_cost_backward.value_or(std::numeric_limits<float>::max()),
                                         total_cost_current.value_or(std::numeric_limits<float>::max()),
                                         total_cost_forward.value_or(std::numeric_limits<float>::max())});
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

std::optional<float> SearchTreeNode::calculate_effective_time(SearchTreeNode &node, Direction dir)
{
    const float RETRIEVAL_TIME_GUESS = 50.0f;
    float total_cost = 0;
    switch (node.retrieval_state)
    {
    case DataRetrievalState::None: {
        total_cost += node.orbit_time;
        total_cost += RETRIEVAL_TIME_GUESS;
        break;
    }
    case DataRetrievalState::Orbited: {
        total_cost += node.retrieval_time.value();
        break;
    }
    case DataRetrievalState::Retrieved: {
        if (dir == Direction::Backward)
        {
            if (!node.children_initialized)
                node.init_children();

            if (!node.get_child_backward())
            {
                std::cerr << "backward node ptr is not defined (not fatal)" << std::endl;
                return std::numeric_limits<float>::max();
            }

            auto val = calculate_effective_time(*node.get_child_backward(), dir);
            total_cost += val.value_or(std::numeric_limits<float>::max());
            total_cost += node.get_child_backward()->get_travel_time_backward().value_or(std::numeric_limits<float>::max());
        }
        else if (dir == Direction::Forward)
        {
            if (!node.children_initialized)
                node.init_children();

            if (!node.get_child_forward())
            {
                std::cerr << "forward node ptr is not defined (not fatal)" << std::endl;
                return std::numeric_limits<float>::max();
            }
            auto val = calculate_effective_time(*node.get_child_forward(), dir);
            total_cost += val.value_or(std::numeric_limits<float>::max());
            total_cost += node.get_child_forward()->get_travel_time_forward().value_or(std::numeric_limits<float>::max());
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
    unsigned long long bef = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    find_suitable_planet();
    unsigned long long aft = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    std::cout << "Finished " << aft-bef << " ms" << std::endl;
}

bool SearchTree::find_suitable_planet()
{
    UniverseState initial_state(planet_list, initial_planet);
    SearchTreeNode root_node(initial_state);
    std::optional<int> suitable_planet_index = one_level_deeper(root_node);

    if (suitable_planet_index.has_value())
    {
        std::cout << "Found suitable planet: #" << suitable_planet_index.value()+1 << std::endl;
        return true;
    }
    else
    {
        std::cout << "Failed to find suitable planet" << std::endl;
        return false;
    }
}

std::optional<int> SearchTree::one_level_deeper(SearchTreeNode &node)
{
    if (!node.get_children_initialized())
        node.init_children();

    if (is_goal_state(node))
        return node.get_planet_index();


    std::cout << "---" << std::endl;
    std::cout << "Currently on planet  #" << node.get_planet_index()+1 << std::endl;
    std::cout << "---" << std::endl;
    std::unique_ptr<SearchTreeNode> &optimal_node = node.choose_optimal_node();

    if (!optimal_node)
        return std::optional<int>();

    return one_level_deeper(*optimal_node);
}

bool SearchTree::is_goal_state(const SearchTreeNode &node)
{
    if (node.get_retrieval_state() == DataRetrievalState::Retrieved)
    {
        const Planet &current_planet = node.get_current_planet();
        std::cout << "Checking planet #" << node.get_planet_index()+1 << std::endl
                  << current_planet.avg_surface_temp << "°K " << std::endl
                  << "terrain: " << current_planet.has_terrain << " "
                  << "water: " << current_planet.has_water << std::endl
                  << "o2: " << current_planet.oxygen_percentage << "%" << std::endl;

        if (!(current_planet.avg_surface_temp > 238 && current_planet.avg_surface_temp < 328 ))
            return false;

        if (!current_planet.has_terrain)
            return false;

        if (!current_planet.has_water)
            return false;

        if (!(current_planet.oxygen_percentage > 20))
            return false;

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
    std::uniform_real_distribution<float> dia_dist(PLANET_MIN_DIAMETER, PLANET_MAX_DIAMETER);
    std::uniform_real_distribution<float> dist_dist(PLANET_MIN_DISTANCE, PLANET_MAX_DISTANCE); // just let me have this
    std::uniform_real_distribution<float> time_dist(PLANET_MIN_RETRIEVAL_TIME, PLANET_MAX_RETRIEVAL_TIME);
    std::uniform_real_distribution<float> temp_dist(PLANET_MIN_TEMP, PLANET_MAX_TEMP);
    std::uniform_real_distribution<float> percent_dist(0, 50);
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
