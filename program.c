#include "declarations.h"

int main() {

    /************************ Here is stored ballot results and voting scores ************************/
    int score_per_vote[RIIGIKOGU_LIIKMED] = {0};
    int votes_per_candidate[RIIGIKOGU_LIIKMED] = {0};
    int votes_per_candidate_before_droop[RIIGIKOGU_LIIKMED] = {0};

    /************************ Here is stored 3 main parameters of voting system ************************/
    int total_candidates = 0;
    int available_mandates = 0;
    int total_ballots = 0;

    /************************ Here is stored buffer for user input and results of voting ************************/
    char buffer[ELANIKE_ARV][RIIGIKOGU_LIIKMED * 3];
    char voting_results_candidates_hierarchy[RIIGIKOGU_LIIKMED * 3];

    /************************ Init char arrays with ASCII symbol 63 = "?" ************************/
    init_char_array(voting_results_candidates_hierarchy, RIIGIKOGU_LIIKMED * 3, 63);
    init_2d_char_array(buffer, RIIGIKOGU_LIIKMED * 3, 63);


    /************************ Greet user at the beginning ************************/
    greeting();

    /************************ Calculate Droop quota value and parse data from input ************************/
    int droop_quota = parse_data_return_droop(buffer, &total_candidates, &available_mandates, &total_ballots);

    /************************ Copy candidates list to unhierarchied array of voting results ************************/
    strcpy(voting_results_candidates_hierarchy, buffer[0]);

    /************************ Based on ballots and candidate amount, fills two tables:
     * "score_per_vote" and "votes_per_candidate" ************************/
    create_vote_tables(buffer, votes_per_candidate, score_per_vote, total_ballots, total_candidates);

    /*  printf("Total %d Mandates %d Droop %d Votes in total %d\n", total_candidates, available_mandates, droop_quota,
             total_ballots);
  */

    for (int i = 0; i < total_candidates; i++) {
         printf("Value of votes is %d\n", votes_per_candidate[i]);
     }
     for (int i = 0; i < total_candidates; i++) {
         printf("Power of votes is %d\n", score_per_vote[i]);
     }
    printf("\n\n");

    /*  printf("Max of amount is at location of %d\n", find_max(votes_per_candidate, total_candidates));
      printf("Max of points is at location of %d\n", find_max(score_per_vote, total_candidates));*/

    /************************ Sort two tables (result of "create_vote_tables") in ascending order ************************/
    parallel_sort(voting_results_candidates_hierarchy, votes_per_candidate, score_per_vote, total_candidates);

    for (int i = 0; i < total_candidates; ++i) {
        printf("\nCandidates after fileting::: %c\n", voting_results_candidates_hierarchy[i * 3]);
    }
    printf("\n\n\t\t\tSORTED ARRAYS\n\n");
    for (int i = 0; i < total_candidates; i++) {
        printf("Value of votes is %d\n", votes_per_candidate[i]);
    }
    for (int i = 0; i < total_candidates; i++) {
        printf("Power of votes is %d\n", score_per_vote[i]);
    }


    copy_int_array(votes_per_candidate, votes_per_candidate_before_droop, total_candidates);

    /************************ Parse two tables (result of "parallel_sort"), based on Droop quota
     * assign available mandates ************************/
    assign_mandates(votes_per_candidate, available_mandates, total_candidates, droop_quota);

    /************************ Fancy terminal output of STV votes ************************/


    printf("\n\n\n");
    printf("\n\n\t\t\tVOTING RESULTS\n\n");
    print_fancy(total_candidates, available_mandates, votes_per_candidate_before_droop, voting_results_candidates_hierarchy, droop_quota);
   /* for (int i = 0; i < available_mandates; i++) {
        printf("Value of votes is %d\n", votes_per_candidate[i]);
        if (i == 0) {
            printf("Mandate received %c\n", voting_results_candidates_hierarchy[i]);
        } else {
            printf("Mandate received %c\n", voting_results_candidates_hierarchy[i * 3]);
        }
    }
    for (int i = 0; i < total_candidates; i++) {
        printf("Power of votes is %d\n", score_per_vote[i]);
    }*/
    return 1;
}



