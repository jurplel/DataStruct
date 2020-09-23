#ifndef TEAMOPTIMIZER_H
#define TEAMOPTIMIZER_H

#include <string>
#include <map>

#include "project.h"

struct Candidate {
  public:
    int years_programming;
    int years_designing;
    int years_leading;
    char appointed_role;
};

class TeamOptimizer : Project
{
  public:
    explicit TeamOptimizer();
    void run() override;
    
    void add_candidate();
    void set_candidate_info(std::string candidate_name, Candidate &candidate);
    void list_candidates();
    void select_candidates(const char &input_mode);
    Candidate& get_candidate();
    void view_candidates();
  private:
    // key is name
    std::map<std::string, Candidate> candidate_list;
};

#endif // TEAMOPTIMIZER_H