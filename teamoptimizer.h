#include <string>
#include <vector>

#include "project.h"

// I have realized later that this could probably just be a struct...
struct Candidate {
  public:
    std::string name;
    int years_programming;
    int years_designing;
    int years_leading;
};

class TeamOptimizer : Project
{
  public:
    explicit TeamOptimizer();
    void run() override;
    
    void add_candidate();
    void list_candidates();
    void view_candidates();
  private:
    std::vector<Candidate> candidate_list;
};