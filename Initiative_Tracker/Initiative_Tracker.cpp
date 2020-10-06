/****************************************
*       Written by Derek Perkins        *
*                 ---                   *
*       Tracks initiative order for     *
*       Dungeons & Dragons Encounters   *
****************************************/

#include <iostream>
#include <vector>
#include "Player.h"

using namespace std;

void build(Console c);                                  // User entries that build the player lists before concatenating them into a master list.
void cont(Console c);                                   // After combat ends, this will handle whether or not the user wants to continue
void gameLoop(vector<Player> fighters);                 // This will handle the loop until the user says that combat has ended.
vector<Player> reorder(std::vector<Player> p);          // Sort combatant list. Prioritizes players over monsters.
vector<Player> turn(vector<Player> p);                  // Rotate the vector to put the current actor at index 0.
Player rename(Player p, Console c);


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

    while (!exit)
    {
        system("CLS");

        c.print("\t\t\tInitiative");
        c.print("\t\tRound: " + std::to_string(round) + "\n------------------------------------------------------------------------\n\n");

        for (int i = 0; i < fighters.size(); i++)
        {
            c.print("\t");
            fighters[i].pprint();
            c.print("\n\n");
        }

        c.print("------------------------------------------------------------------------\n\n");
        c.print("1.) Edit (Rename a player or NPC)\n");
        c.print("2.) Exit\n");

        comm = c.getString("\nEnter command ('exit' to leave): ");

        if (comm == "exit" || comm == "2")
        {
            exit = true;
        }
        else if (comm == "edit" || comm == "1")
        {
            int x = c.getInt("Select a player to edit:   ");
            x--;
            fighters[x] = rename(fighters[x], c);
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
    cont(c);
}

void cont(Console c)
{
    system("CLS");

    std::string comm = "Random shit";

    while (comm != "y")
    {
        comm = c.getString("\nNew encounter (y/n)?     ");
        //comm = tolower(putchar(tolower(comm[0])));

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