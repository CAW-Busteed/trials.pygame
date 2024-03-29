#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {
        // Query for each rank from voter of each nominee
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    // for preference[voter]
    for (int i= 0; i < candidate_count; i++)
    {

        string check_name = candidates[i].name;

        //if name matches a candidate name
        if (strcmp(check_name, name) == 0)
        {
            //add vote to pref
            //printf("%s matches.\n", check_name);
            preferences[voter][i] = rank;
            return true;
         }
    }
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    int running = candidate_count;

    //go through each candidate to confirm their eligibility
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].eliminated == true)
        {
            running -= 1;
        }
    }

    string ranking[running];
    //go through each voter's preference
    for (int i= 0; i < voter_count; i++)
    {
        //go through their candidate ranking
        for (int j = 0; j < candidate_count; j++)
        {
            //match to each candidate
            for (int k = 0; k <candidate_count; k++)
            {
                if (candidates[k].eliminated == false)
                {
                    ranking[preferences[i][j]] = candidates[k].name;
                }
            }
            for (int k = 0; k <candidate_count; k++)
            {
                for (int l= 0; l <candidate_count; l++)
                {
                    if (ranking[k] == candidates[l].name)
                    {
                        candidates[l].votes += 1;
                    }
                }
            }
        }
    }
    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    for (int i= 0; i < candidate_count; i++)
    {
        //return whomever has more than half the vote
        if (candidates[i].votes > (voter_count/2))
        {
            printf("%s\n", candidates[i].name);
            return true;
        }
    }
    //otherwise, keep going
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    int smallest_val;
    // TODO return the lowest NUMBER of votes someone has
    for (int i= 0; i < candidate_count; i++)
    {
        for (int j= i+1; j < candidate_count; j++)
        {
            if (candidates[i].votes <= candidates[j].votes && candidates[i].eliminated == false)
            {
                smallest_val = candidates[i].votes;
            }
            else if (candidates[j].votes <= candidates[i].votes && candidates[j].eliminated == false)
            {
                smallest_val = candidates[j].votes;
            }
        }
    }
    return smallest_val;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    // if all VIABLEcandidates have the same number of votes, they share a minimum!
        for (int i= 0; i < candidate_count; i++)
        {
            if (candidates[i].votes != min && candidates[i].eliminated == false)
            {
                return false;
            }
        }
        return true;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    // those with minimum shift out of the running.
    for (int i= 0; i < candidate_count; i++)
    {
        if (candidates[i].votes == min && candidates[i].eliminated == false)
        {
            candidates[i].eliminated = true;
        }
    }
    return;
}