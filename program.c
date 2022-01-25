#include "stvlib.h"

int main() {

    /************************ Here is stored ballot numerical results ************************/
    int score_per_vote[POLITICS] = {0}; // how many points each politic obtained (score per vote)
    int votes_per_candidate[POLITICS] = {0}; // how many votes each politic obtained
    int votes_per_candidate_before_droop[POLITICS] = {
            0}; // this array is obligatory, when you wish to see not only final results, but also raw ones

    /************************ Here is stored 3 main parameters of voting system ************************/
    int total_candidates = 0; // variable to store votes per candidate
    int available_mandates = 0; // variable to store score per vote for each candidate
    int total_ballots = 0; // how many citizens have been voted

    /************************ Here is stored buffer for user input and results of voting ************************/
    char buffer[CITIZENS][POLITICS *
                          3]; // buffer, that store [0]: candidates list, [1]: amount of mandates, [2 + n]: single citizen's ballot
    char voting_results_candidates_hierarchy[
            POLITICS * 3]; // array, that stores final result of voting (where winner is at top and so on)

    /************************ Greet user at the beginning ************************/
    greeting();

    /************************ Parse user input from console, return Droop quota ************************/
    int droop_quota = parse_data_from_console(buffer, &total_candidates, &available_mandates, &total_ballots);

    /************************ Based on amount of ballots and candidates, fills two tables:
     * "score_per_vote" and "votes_per_candidate", then sort them in descending order ************************/
    create_vote_tables_sort_desc(buffer, voting_results_candidates_hierarchy, votes_per_candidate, score_per_vote,
                                 total_ballots, total_candidates);

    /************************ Save initial values of votes, but use copied array to future process
     * this is needed, when you wish to see not only final result of votes, but also raw one ************************/
    copy_int_array(votes_per_candidate, votes_per_candidate_before_droop, total_candidates);

    /************************ Parse two tables (result of "create_vote_tables_sort_desc"), then based on Droop quota
     * assign available mandates ************************/
    assign_mandates(votes_per_candidate, available_mandates, total_candidates, droop_quota);

    /************************ Print to terminal results of voting in a fancy way ************************/
    print_fancy(total_candidates, available_mandates, votes_per_candidate_before_droop, votes_per_candidate,
                score_per_vote,
                voting_results_candidates_hierarchy, droop_quota);

    return 1;
}
