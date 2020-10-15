/****************************************
*       Written by Derek Perkins        *
*                 ---                   *
*       Tracks initiative order for     *
*       Dungeons & Dragons Encounters   *
****************************************/

#include <vector>
#include "Player.h"

using namespace std;

void build(Console c);                                  // User entries that build the player lists before concatenating them into a master list.
void cont(Console c, string time);                      // After combat ends, this will handle whether or not the user wants to continue
void gameLoop(vector<Player> fighters);                 // This will handle the loop until the user says that combat has ended.
vector<Player> reorder(std::vector<Player> p);          // Sort combatant list. Prioritizes players over monsters.
vector<Player> turn(vector<Player> p);                  // Rotate the vector to put the current actor at index 0.
Player rename(Player p, Console c);
vector<Player> add(vector<Player> p, Console c);        // Adds a combatant to the vector
int getIndex(vector<Player> p, int choice);             // Gets the vector index for a selected player
string timePassed(int rounds);                          // Returns a display message for elapsed time


int main()
{
    system("COLOR A");                                  // Change color of text. REALLY not necessary, but Gary requested it.

    Console c;                                          // Initialize Console. Purely to pass it to build()

    c.print("Initiative Tracker\n\n");

    build(c);
}

void build(Console c)
{
    std::vector<Player> fighters;                       // List that stores players and enemies. Will include legendary actions.
    Player entry;                                       // Placeholder object for user input.

    int players = 0;
    int enemies = 0;

    do { players = c.getInt("How many players?   "); }                      // Get number of players and enemies. Streamlines populating the vector.
    while (players < 0);                                                    // -1 is an error. 

    system("CLS");                                                              

    do { enemies = c.getInt("How many enemies?  "); } while (enemies < 0);  // Enemies (monsters/NPCs)

    system("CLS");

    // Might insert a Legendary Action thing prompt here. Unsure

    c.print("Player Entry\n---------------------------------------------");

    for (int i = 0; i < players; i++)
    {
        int x = i + 1;
        std::string label = "\nPlayer " + std::to_string(x) + ":    ";                      // Get name

        entry.setName(c.getString(label));
        do { entry.setInit(c.getInt("\nInitiative:    ")); } while (entry.getInit() < 0);   // Get initiative

        fighters.push_back(entry);
    }

    system("CLS");
    c.print("Enemy Entry\n-----------------------------------------------");

    for (int i = 0; i < enemies; i++)
    {
        int x = i + 1;
        std::string label = "\nEnemy " + std::to_string(x) + ":   ";                        // Get name

        entry.setName(c.getString(label));
        do { entry.setInit(c.getInt("\nInitiative:    ")); } while (entry.getInit() < 0);   // Get initiative

        fighters.push_back(entry);
    }

    fighters = reorder(fighters);                   // Sorts the vector and assigns an order number

    gameLoop(fighters);
}

void gameLoop(vector<Player> fighters)
{
    std::string comm = "";
    bool exit = false;
    Console c;
    int pTurn = 0;                          // Increment after a player goes. After all players go, then increment turn.
    int round = 1;                          // Increment after all players go.
    int combatants = fighters.size();       // Store the number of players, for tracking turns.
    string time;

    while (!exit)
    {
        time = timePassed(round);

        system("CLS");

        c.print("\t\t\tInitiative");
        c.print("\t\tRound: " + std::to_string(round) + "\t" + time);
        c.print("\n------------------------------------------------------------------------\n\n");

        for (int i = 0; i < fighters.size(); i++)
        {
            c.print("\t");
            fighters[i].pprint();
            c.print("\n\n");
        }

        c.print("------------------------------------------------------------------------\n\n");
        c.print("1.) Add (Add a combatant)\n");
        c.print("2.) Edit (Rename a player or NPC)\n");
        c.print("3.) Remove (NOT IMPLEMENTED)\n");
        c.print("4.) Exit\n");

        comm = c.getString("\nEnter command ('exit' to leave): ");

        if (comm == "exit" || comm == "4")
        {
            exit = true;
        }
        else if (comm == "remove" || comm == "3")
        {
            int x = c.getInt("Select a player to remove:   ");          // User selects for removal
            x--;
            x = getIndex(fighters, x);                                  // Get the index for selected combatant
            fighters.erase(fighters.begin() + x);                       // Remove combatant
            fighters = reorder(fighters);                               // Reorders the vector to reassign order numbers
            for (int i = 0; i < pTurn; i++)                             // Place combatants in the order in which we left off.
                fighters = turn(fighters);
        }
        else if (comm == "edit" || comm == "2")
        {
            int x = c.getInt("Select a player to edit:   ");
            x--;
            int index = getIndex(fighters, x);                      // Add input validation
            fighters[index] = rename(fighters[index], c);
        }
        else if (comm == "add" || comm == "1")
        {
            fighters = add(fighters, c);
            fighters = reorder(fighters);
        }
        else
        {
            fighters = turn(fighters);
            pTurn++;

            if (pTurn == combatants)
            {
                round++;
                pTurn = 0;
            }
        }
    }
    cont(c, time);
}

void cont(Console c, string time)
{
    system("CLS");

    std::string comm = "Random shit";

    c.print("Encounter ended! Time elapsed: " + time + "\n");

    while (comm != "y")
    {
        comm = c.getString("\nNew encounter (y/n)?     ");

        if (comm == "y" || comm == "Y")
        {
            build(c);
        }
        else if (comm == "n" || comm == "N")
        {
            break;
        }
        else
        {
            c.print("Unknown command! Enter 'y' or 'n'.");
        }
    }

    comm = c.getString("\nThanks for playing! Press ENTER to terminate the program.");
}

//------------------------------- Misc Functions ---------------------------------------------------------------------------------------

vector<Player> reorder(std::vector<Player> p)   // Sort combatants and assign an order number. Return the sorted list.
{
    int size = p.size();
    int index = 0;
    std::vector<Player> newP;

    for (int i = 35; i >= 0; i--)
    {
        for (int x = 0; x < size; x++)
        {
            if (p[x].getInit() == i)
            {
                newP.push_back(p[x]);       // Add combatant to index 'x'
            }
        }
    }

    for (int i = 0; i < size; i++)
    {
        p[i] = newP[i];
        p[i].setOrder(i);
    }

    return p;
}

vector<Player> turn(std::vector<Player> p)              // Cycle list and display current actor at the top. Return the list.
{
    Player temp = p[0];
    int a = p.size();
    a--;

    for (int i = 0; i < a; i++)
    {
        int x = i + 1;
        p[i] = p[x];
    }

    p[a] = temp;

    return p;
}

Player rename(Player p, Console c)                      // Changes the name of a Player object.
{
    std::string name = "";

    p.setName(c.getString("New Name:     "));

    return p;
}

vector<Player> add(vector<Player> p, Console c)
{
    Player entry;
    entry.setName(c.getString("New player name:\t"));
    entry.setInit(c.getInt("Initiative:\t"));

    p.push_back(entry);

    return p;
}

int getIndex(vector<Player> p, int choice)
{
    int i = -1;                                             // Default value is an error code

    for (int x = 0; x < p.size(); x++)                      // Iterate through the list
    {
        int order = p[x].getOrder();                        // Record the currently selected player's initiative order

        if (order == choice)                                // Check current order number with user-selected number. If it matches...
        {
            i = x;                                          // Set the return value
            break;                                          // Break loop for optimization. Don't need to continue if value is found.
        }
    }

    return i;
}

// Time Conversion

string timePassed(int rounds)
{
    int seconds = rounds * 6;
    int minutes = 0;
    int hours = 0;
    int temp = 0;

    if (seconds > 59)
    {
        minutes = seconds / 60;
        seconds = seconds % 60;
    }

    if (minutes > 59)
    {
        hours = minutes / 60;
        minutes = minutes % 60;
    }

    string elapse = "";
    string s = to_string(seconds) + " seconds";
    string m = to_string(minutes) + " minutes";
    string h = to_string(hours) + " hours";

    if (hours > 0)
        elapse += h;

    if (minutes > 0 && hours > 0)
        elapse += ", " + m;
    else if (minutes > 0)
        elapse += m;

    if (seconds > 0 && minutes > 0)
        elapse += ", " + s;
    else if (seconds > 0)
        elapse += s;
    
    return elapse;
}