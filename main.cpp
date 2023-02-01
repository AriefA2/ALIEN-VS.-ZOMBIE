// ********************************************************* 
// Course: TCP1101 PROGRAMMING FUNDAMENTALS 
// Year: Trimester 1, 2022/23 (T2215) 
// Lab: T14L 
// Names: Arief Aiman | Clement | Han Wai 
// IDs: 1211312245 | MEMBER_ID_2 | MEMBER_ID_3 
// Emails: 1211312245@student.mmu.edu.my | MEMBER_EMAIL_2 | 1211101843@student.mmu.edu.my 
// Phones: 017-4137890 | 012-4829822 | 011-28216621 
// ********************************************************* 

#include "pf/helper.h" // Currently non-functional
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime> 
#include <iomanip> 
using namespace std;

class Land // Land class, for map (Land)
{
private:
    vector< vector<char> > map_; // Vector declaration
    int dimX_, dimY_; // Map dimensions

public:
    Land(int dimX = 15, int dimY = 5); // Default dimensions, 15 x 5
    void init(int dimX, int dimY); // Dimension declaration
    void display() const; // Display map
};

Land::Land(int dimX, int dimY)
{
    init(dimX, dimY);
}

void Land::init(int dimX, int dimY) // Declare map dimensions
{
    dimX_ = dimX; // Row
    dimY_ = dimY; // Column

    char objects[] = {'^', 'v', '>', '<', 'p', 'h', 'r', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}; // Non-functional objects
    int noOfObjects = 15; // Number of objects in array, must include blank objects

    // 2D map generator
    map_.resize(dimY_); // Empty rows
    for (int i = 0; i < dimY_; ++i)
    {
        map_[i].resize(dimX_); // Resize each row
    }
    
    // Object positions
    for (int i = 0; i < dimY_; ++i) // Column position
    {
        for (int j = 0; j < dimX_; ++j) // Row position
        {
            int objNo = rand() % noOfObjects;
            map_[i][j] = objects[objNo];
        }
    }
}
void Land::display() const // Display const declaration
{

    cout << " ------------------------------------------" << endl;
    cout << " = Destroy all Zombies! Don't get killed! =" << endl; // Intro message
    cout << " ------------------------------------------" << endl;

    for (int i = 0; i < dimY_; ++i)
    {
        cout << " "; // Display row pattern
        for (int j = 0; j < dimX_; ++j)
        {
            cout << "+-"; // Pattern for border
        }
        cout << "+" << endl;
        
        cout << setw(2) << (dimY_ - i); // Display row numbers

        for (int j = 0; j < dimX_; ++j) // Display cell content and border
        {
            cout << "|" << map_[i][j]; // Grid spacing
        }
        cout << "|" << endl;
    }

    cout << " ";
    for (int j = 0; j < dimX_; ++j) // Display lower border of the last row
    {
        cout << "+-";
    }
    cout << "+" << endl;
 
    cout << " ";
    for (int j = 0; j < dimX_; ++j) // Display column numbers
    {
        int digit = (j + 1) / 10;
        cout << " ";
        if (digit == 0) // If number is above 9 
            cout << " ";
        else
            cout << digit;
    }
    cout << endl;
    cout << " ";
    for (int j = 0; j < dimX_; ++j) // Put 1 above numbers higher than 9
    {
        cout << " " << (j + 1) % 10;
    }
    cout << endl << endl;
}

void run()
{
    Land land;
    land.display(); // Display the map
}

int main()
{
    run(); // Runs the program
}