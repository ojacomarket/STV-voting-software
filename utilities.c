#include "declarations.h"

/**
 * Parse data from a user and calculate Droop quota
 * @param buffer char 2D array, that will store user input (each input as single string)
 * @param total_candidates pointer to integer, that will store amount of voting candidates
 * @param mandates_given pointer to integer, that will store amount of mandates
 * @param total_votes pointer to integer, that will store amount of votes, that people made for candidates
 * @return [int]: Droop quota
 */
int parse_data_return_droop(char buffer[ELANIKE_ARV][RIIGIKOGU_LIIKMED * 3], int *total_candidates, int *mandates_given,
                            int *total_votes) {

    /************************ Loop until valid characters are inserted (UPPERCASE LETTER, ...) ************************/
    while (*total_candidates < 1) { // ask for candidates input, until at least is submitted
        printf("\n[###....... 30%] input your candidates: ");
        fgets(buffer[0], (RIIGIKOGU_LIIKMED * 3), stdin);
        *total_candidates = check_data_validity(buffer[0], (RIIGIKOGU_LIIKMED * 3));
    }
    printf("\n[OK]\n");

    /************************ Loop until valid characters are inserted (numbers from 1 to 101) ************************/
    while (1) { // ask for mandates amount, until number from 1 to 101 is assigned
        printf("\n[######.... 60%] input number of mandates to be given: ");
        fgets(buffer[1], 10, stdin);
        *mandates_given = atoi(buffer[1]);
        if (*mandates_given > 0 && *mandates_given < 102) {
            break;
        } else
            printf("\n[ERROR] invalid input ::: only numbers in the range of 1 to 101 are allowed...\n");
    }
    printf("\n[OK]\n");

    int votes_buffer_index = 2; // inside a buffer each string of VOTES will reside at index 2, 3, 4 and so forth

    /************************ Loop until valid characters are inserted (UPPERCASE LETTER, ...) ************************/
    while (1) { // ask for voting ballots, until Enter on an empty line is pressed
        printf("\n[#########. 90%] input your votes (to exit press 'Enter' on a new line): ");
        fgets(buffer[votes_buffer_index], (RIIGIKOGU_LIIKMED * 3), stdin);
        int vote = check_data_validity(buffer[votes_buffer_index], (RIIGIKOGU_LIIKMED * 3));
        if (!find_absence(buffer[0], buffer[votes_buffer_index], *total_candidates, vote) && vote > -1) {
            printf("\n[ERROR] unregistered candidate...\n");
            continue;
        }
        if (!vote) // invalid vote ? Continue to ask
            continue;
        else if (vote == -1) // enter is pressed and data is correct ? Exit loop of asking
            break;
        else // votes are still coming ? Continue to count them
            *total_votes += 1;
        printf("\n[OK]\n");
        votes_buffer_index++;
    }

    return (*total_votes / ((*total_candidates) + 1)) + 1; // droop quota equation
}

/**
* Check string to be applicable for STV voting system
* @param array string
* @param size size of string
* @return [-1]: if no input, [0]: if data is invalid, [1]: if data is valid
*/
int check_data_validity(const char *array, int size) {

    int total_candidates = 0; // counter for politicans to be voted for
    int valid_element = 0; // counter to track valid string element
    int coma = 0; // counter for comas
    int insertions = 0;
    for (int i = 0; i < size; i++) {
        // if string has ASCII elements of 64 - 91, 32, 44, 10 , 0
        if (array[i] > 64 && array[i] < 91 || array[i] == 32 || array[i] == 44 || array[i] == 10 || array[i] == 0) {
            insertions++;
            if (valid_element > 1) { // you cannot input more than 1 uppercase letter at once
                printf("\n[ERROR] invalid input ::: use UPPERCASE LETTER, UPPERCASE LETTER, ...\n");
                return 0;
            } else if (array[i] > 64 && array[i] < 91) { // if found ASCII for A to Z letters
                valid_element++;
                total_candidates++;
            } else if (array[i] == 44) { // if coma is found
                if (valid_element < 1) { // single coma is not allowed to input
                    printf("\n[ERROR] invalid input, use UPPERCASE LETTER, UPPERCASE LETTER, ...\n");
                    return 0;
                } else
                    coma++;
            } else if (array[i] == 32) { // if space is met
                if (valid_element < 1) { // single space is not allowed
                    printf("\n[ERROR] invalid input ::: use UPPERCASE LETTER, UPPERCASE LETTER, ...\n");
                    return 0;
                } else if (coma < 1) { // coma must be inside an input to separate uppercase letters
                    printf("\n[ERROR] invalid input ::: use UPPERCASE LETTER, UPPERCASE LETTER, ...\n");
                    return 0;
                } else { // reset counter, then single element is checked and valid
                    valid_element = 0;
                    coma = 0;
                }
            } else if (array[i] == 10) { // if enter is met
                if (total_candidates < 1) {
                    printf("\n[ERROR] no data was input...\n");
                    return -1;
                }
                if (!find_duplicates_in_string(array, insertions)) {
                    printf("\n[ERROR] no duplicate candidates allowed...\n");
                    return 0;
                }
                break;
            }
        } else {
            printf("\n[ERROR] invalid input ::: use UPPERCASE LETTER, UPPERCASE LETTER, ...\n");
            return 0;
        }
    }
    return total_candidates;
}

/**
 * Create two tables: one that will store how many people have chosen each candidate and another - points for each choice
 * @param buffer 2D array that store all user input data
 * @param votes_per_candidate integer array, that will store amount of votes for each candidate
 * @param score_per_vote integer array, that will store amount of points for each candidate (each vote has different points)
 * @param votes how many votes been made by people in total
 * @param candidates how many ballot candidates we have
 */
void
create_vote_tables(char buffer[ELANIKE_ARV][RIIGIKOGU_LIIKMED * 3], int votes_per_candidate[RIIGIKOGU_LIIKMED],
                   int score_per_vote[RIIGIKOGU_LIIKMED],
                   int votes, int candidates) {

    for (int i = 0; i < votes; i++) { // we need to iterate over each vote and get data from there
        for (int j = 0; j < candidates; j++) { // we assign amount of votes made and points given per each candidate
            for (int k = 0; k <
                            candidates; k++) { // fix string of candidates and parse strings of votes by people to find similarities
                if (buffer[0][j * 3] == buffer[2 + i][k * 3]) { // if candidate is found in a single vote
                    votes_per_candidate[j]++; // add +1, which indicates, that that particular candidate has been chosen
                    score_per_vote[j] += (candidates -
                                          k); // points, that each candidate is given by each vote is assigned with respect to candidate name position in a vote
                }
            }
        }
    }
}

/**
 * Sort two integer and one candidate arrays respectively in descending order
 * @param candidate_results array of candidate names (1 symbol per name)
 * @param array1 integer array, that represent votes per candidate
 * @param array2 integer array, that represent score per single vote
 * @param size size of integer array (both "array1" and "array2" should be equal in size)
 */
void parallel_sort(char candidate_results[RIIGIKOGU_LIIKMED * 3], int *array1, int *array2, int size) {

    int max_from_1 = 0; // variable, stores index of maximum value from "array1"
    int max_from_2 = 0; // variable, stores index of maximum value from "array2"
    int tmp; // variable, stores previous value of the whatever "int" array
    char tmp_letter; // variable, stores previous char of "candidate_results" array

    /************************ Don't change initial arrays, make copies and operate on them ************************/
    int copy_array1[size];
    copy_int_array(array1, copy_array1, size);
    int copy_array2[size];
    copy_int_array(array2, copy_array2, size);


    /************************ Loop throughout all available candidates ************************/

    for (int i = 0; i < size; ++i) {

        max_from_1 = find_max(copy_array1, size); // index of maximum value found in "array1"
        max_from_2 = find_max(copy_array2, size); // index of maximum value found in "array2"

        /************************ If value from "array1" is ultimately maximum OR at least maximum in "array1" ************************/
        if (max_from_1 == max_from_2 || array1[max_from_1] > array1[max_from_2]) {

            copy_array1[max_from_1] = -1; // set -1 to index of maximum value to prevent it to be maximum next iteration
            copy_array2[max_from_1] = -1; // set -1 to index of maximum value to prevent it to be maximum next iteration

            /************************ Change int values inside the real array of voting amount ************************/
            tmp = array1[i];
            array1[i] = array1[max_from_1];
            array1[max_from_1] = tmp;

            /************************ Change char values inside the array of candidates ************************/
            tmp_letter = candidate_results[i * 3];
            candidate_results[i * 3] = candidate_results[max_from_1 * 3];
            candidate_results[max_from_1 * 3] = tmp_letter;

            /************************ If value is ultimately maximum ************************/
            if (max_from_1 == max_from_2) {

                /************************ Shift int value in real array of voting scores ************************/
                tmp = array2[i];
                array2[i] = array2[max_from_2];
                array2[max_from_2] = tmp;

                /************************ Shift int value in copied array of voting scores ************************/
                tmp = copy_array2[i];
                copy_array2[i] = copy_array2[max_from_2];
                copy_array2[max_from_2] = tmp;
            }
                /************************ If value is bigger inside only "array1" ************************/
            else {

                /************************ Shift int value in real array of voting scores ************************/
                tmp = array2[i];
                array2[i] = array2[max_from_1];
                array2[max_from_1] = tmp;

                /************************ Shift int value in copied array of voting scores ************************/
                tmp = copy_array2[i];
                copy_array2[i] = copy_array2[max_from_1];
                copy_array2[max_from_1] = tmp;
            }

            /************************ Change int values inside the copied array of voting amount ************************/
            tmp = copy_array1[i];
            copy_array1[i] = copy_array1[max_from_1];
            copy_array1[max_from_1] = tmp;

        }
            /************************ If value from "array1" is less than value from "array1" at index of (maximum from "array2") ************************/
        else {
            copy_array1[max_from_2] = -1;
            copy_array2[max_from_2] = -1;

            /************************ Change int values inside the array of voting amount ************************/
            tmp = array1[i];
            array1[i] = array1[max_from_2];
            array1[max_from_2] = tmp;

            /************************ Change char values inside the array of candidates ************************/
            tmp_letter = candidate_results[i * 3];
            candidate_results[i * 3] = candidate_results[max_from_2 * 3];
            candidate_results[max_from_2 * 3] = tmp_letter;

            /************************ Change int values inside the array of voting scores ************************/
            tmp = array2[i];
            array2[i] = array2[max_from_2];
            array2[max_from_2] = tmp;

            /************************ Change int values inside the copied array of voting amount ************************/
            tmp = copy_array1[i];
            copy_array1[i] = copy_array1[max_from_2];
            copy_array1[max_from_2] = tmp;

            /************************ Change int values inside the copied array of voting scores ************************/
            tmp = copy_array2[i];
            copy_array2[i] = copy_array2[max_from_2];
            copy_array2[max_from_2] = tmp;
        }
    }
}

/**
 * Find maximum value inside an array of integers
 * @param array array of integers
 * @param size size of the array
 * @return index of maximum value found inside the array
 */
int find_max(const int *array, int size) {
    int index_of_max = 0;
    for (int i = 1; i < size; ++i) {
        index_of_max = array[i] > array[index_of_max] ? i : index_of_max;
    }
    return index_of_max;
}

/**
 * Copy one integer array values into another integer array
 * @param array integer array of values to be copied from
 * @param copy integer array to copy values into
 * @param size size of both arrays should be the same
 */
void copy_int_array(const int *array, int *copy, int size) {
    for (int i = 0; i < size; ++i) {
        copy[i] = array[i];
    }
}

/**
 * Leave only those candidates, which passed Droop quota and has available mandate left
 * @param votes_per_candidate integer array of votes per single candidate
 * @param mandates_given how many available mandates we have
 * @param candidates total amount of available for voting candidates
 * @param droop Droop quota
 */
void assign_mandates(int votes_per_candidate[RIIGIKOGU_LIIKMED], int mandates_given, int candidates, int droop) {

    int extra_votes = 0; // droop and votes amount per candidate ratio (+/-)
    int redundant_candidates = candidates - mandates_given; // not all politics will be elected

    /************************ If candidates > than mandates, then loop on mandates, otherwise on candidates ************************/
    for (int i = 1; i <= (redundant_candidates >= 0 ? mandates_given : candidates); ++i) {
        extra_votes = droop - votes_per_candidate[i -
                                                  1]; // if negative = potential candidate for mandate!, if pos. = cannot proceed forward voting
        if (extra_votes >
            0) { // if candidate didn't overcome quota, then its votes will be charged to most dominant candidate
            int counter = 0; // helper, to be able to loop back and forth throughout array elements

            /************************ Loop until all redundant votes are assigned to most dominant candidate ************************/
            while (extra_votes > 0 && votes_per_candidate[i - 1] != droop) {
                votes_per_candidate[i - 1] += votes_per_candidate[mandates_given - counter];
                votes_per_candidate[(mandates_given - counter)] = 0;
                extra_votes--;
                counter++;
            }
        } else { // if candidate overcame quota, it potentially "can have" a mandate
            votes_per_candidate[i -
                                1] = droop; // all left votes will be charged to next "less dominant" candidate (next to most one)
            votes_per_candidate[i] += abs(extra_votes);
        }
    }
}

/**
 * Greet user with a message and program rules
 */
void greeting() {

    printf("\n\tWelcome to STV voting system!\n");
    printf("\nRules of use are simple:\n");
    printf("\ta) First input total amount of candidates ::: UPPERCASE LETTER, UPPERCASE LETTER, ...\n");
    printf("\tb) Then input how many mandates will be given ::: NUMBER IN A RANGE FROM 1 TO 101\n");
    printf("\tc) Finally you will be asked to input your voting ballots ::: UPPERCASE LETTER, UPPERCASE LETTER, ...\n");
    printf("\nAny other input, that hasn't been mentioned will be aborted and asked to input again\n\n");
    printf("Enjoy :)\n\n");
}

/**
 * Find duplicates inside a string (enhanced version of strchr() of <string.h> library)
 * @param array string
 * @param size size of string
 * @return [0]: if duplicates were found, [1]: if no duplicates were found
 */
int find_duplicates_in_string(const char *array, int size) {

    for (int j = 0; j < size; ++j) {
        for (int k = 0; k < size; ++k) {
            if (array[j] > 64 && array[j] < 91) {
                if (array[j] == array[k] && j != k) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

/**
 * Search a character from one string inside another
 * @param search_from string to find from
 * @param sample string that will serve its element to look inside a "search_from" string
 * @param size size of "search_from" string
 * @return [int]: how many characters were found
 */
int find_absence(const char *search_from, const char *sample, int size, int votes_per_ballot) {
    int found; // counter for similar symbols found inside two arrays
    int symbol;// any symbol that found inside "sample" array
    int valid = 0;
    for (int j = 0; j < votes_per_ballot; ++j) {
        found = 0;
        symbol = 0;
        for (int k = 0; k < size; ++k) {
            if (search_from[k * 3] > 64 && search_from[k * 3] < 91 && sample[j * 3] > 64 &&
                sample[j * 3] < 91) { // if it is ASCII symbols for A - Z
                symbol++;
                if (sample[j * 3] == search_from[k * 3]) { // if equal symbols
                    found++;
                    valid++;
                }
            } else
                break;
        }
        if (!found && symbol) // if found any other than available in "search_from" symbols
            return 0;
    }
    return valid;
}

/**
 * Init array of symbols
 * @param array char array
 * @param size size of char array
 * @param symbol symbol ASCII to use as an array initializer
 */
void init_char_array(char *array, int size, int symbol) {
    for (int i = 0; i < size; ++i) {
        array[i] = (char) symbol;
    }
}

/**
 *
 * @param array_2D char array 2D
 * @param size size of char array_2D
 * @param symbol symbol ASCII to use as an array_2D initializer
 */
void init_2d_char_array(char array_2D[ELANIKE_ARV][RIIGIKOGU_LIIKMED * 3], int size, int symbol) {
    for (int i = 0; i < ELANIKE_ARV; ++i) {
        for (int j = 0; j < size; ++j) {
            array_2D[i][j] = (char) symbol;
        }
    }
}

/**
 * Fancy output into terminal as summary of STV voting
 * @param total_candidates total amount of candidates available
 * @param available_mandates how many mandates are given
 * @param votes_per_candidate array of votes per candidate
 * @param voting_results_candidates_hierarchy hierarchy of voting (result)
 */
void print_fancy(int total_candidates, int available_mandates, const int *votes_per_candidate,
                 const int *votes_per_candidate_final,
                 const int *score_per_vote, char *voting_results_candidates_hierarchy, int droop) {

    printf("\n\n\n");
    printf("\n\n[########## 100%]\tRAW VOTING RESULTS\n\n\n");

    for (int i = 0; i < total_candidates; i++) {

        printf("Candidate %c: ", voting_results_candidates_hierarchy[i * 3]);
        for (int j = 0; j < votes_per_candidate[i]; ++j) {
            printf("X");
        }
        printf("\t\t\t(Total score: %d)", score_per_vote[i]);
        printf("\n\n");
    }
    printf("\n\n\t\t\tFINAL VOTING RESULTS\n\n");
    printf("\n\tS u m m a r y:\n\n");
    printf("\t(Droop quota): ");
    for (int i = 0; i < droop; ++i) {
        printf("X");
    }

    printf("\n\n\t(Available mandates): %d\n\n\n", available_mandates);

    for (int i = 0; i < total_candidates; i++) {

        printf("Candidate %c: ", voting_results_candidates_hierarchy[i * 3]);
        for (int j = 0; j < (available_mandates > i ? votes_per_candidate_final[i] : votes_per_candidate[i]); ++j) {
            printf("X");
        }
        if (available_mandates > i) {
            printf("   [MANDATE ASSIGNED]");
        }
        printf("\n\n");
    }
}