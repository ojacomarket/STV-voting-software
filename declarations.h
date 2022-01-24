#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define ELANIKE_ARV 10 // place here a number of citizens in your country (since everybody could attend ballot)
#define RIIGIKOGU_LIIKMED 101 // place here a max number of politics, that can obtain mandates in your country

int check_data_validity(const char *buffer, int size);

void
create_vote_tables(char buffer[ELANIKE_ARV][RIIGIKOGU_LIIKMED * 3], int votes_per_candidate[RIIGIKOGU_LIIKMED],
                   int score_per_vote[RIIGIKOGU_LIIKMED],
                   int votes, int candidates);

int parse_data_return_droop(char buffer[ELANIKE_ARV][RIIGIKOGU_LIIKMED * 3], int *total_candidates, int *mandates_given,
                            int *total_votes);

int find_max(const int *array, int size);

void copy_int_array(const int *array, int *copy, int size);

void parallel_sort(char buffer[RIIGIKOGU_LIIKMED * 3], int *array1, int *array2, int size);

void assign_mandates(int votes_per_candidate[RIIGIKOGU_LIIKMED], int mandates_given, int candidates, int droop);

void greeting();

int find_duplicates_in_string(const char *array, int size);

int find_absence(const char *search_from, const char *sample, int size, int votes_per_ballot);

void init_2d_char_array(char array_2D[ELANIKE_ARV][RIIGIKOGU_LIIKMED * 3], int size, int symbol);

void init_char_array(char *array, int size, int symbol);

void print_fancy(int total_candidates, int available_mandates, const int *votes_per_candidate,
                 const int *votes_per_candidate_final,
                 const int *score_per_vote, char *voting_results_candidates_hierarchy, int droop);