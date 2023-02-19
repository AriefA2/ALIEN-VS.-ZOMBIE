// ********************************************************* 
// Course: TCP1101 PROGRAMMING FUNDAMENTALS 
// Year: Trimester 1, 2022/23 (T2215) 
// Lab: T14L 
// Names: Arief Aiman | Clement
// IDs: 1211312245 | 1211103039
// Emails: 1211312245@student.mmu.edu.my | 1211103039@student.mmu.edu.my 
// Phones: 017-4137890 | 012-4829822 
// ********************************************************* 

#include "pf/helper.h" 
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip> 
#include <random>

using namespace std;

class Land // Land class, for map (Land)
{
private:
    vector< vector<char> > map_; // Vector declaration
    int row_, column_; // Map dimensions

public:
    Land(int landColumns, int landRows) // Default dimensions, 15 x 5
    {
        init(landColumns, landRows);
    }
    
    void init(int landColumns, int landRows) // Declare map dimensions
    {
        column_ = landColumns; 
        row_ = landRows; 

        char objects[] = {'^', 'v', '>', '<', 'p', 'h', 'r', ' ', ' ', ' ', ' ', ' '}; 
        int noOfObjects = 12; // Objects

        map_.resize(row_); // Map generator
        for (int i = 0; i < row_; ++i)
        {
            map_[i].resize(column_); 
        }
        
        // Object positions
        for (int i = 0; i < row_; ++i) 
        {
            for (int j = 0; j < column_; ++j) 
            {
                int objNo = rand() % noOfObjects;
                map_[i][j] = objects[objNo];
            }
        }
    }
    void display() const // Display const declaration
    {

        cout << setw(15);
        cout << " ------------------------------------------" << endl;
        cout << " =            Alien vs. Zombie            =" << endl; // Intro message
        cout << " ------------------------------------------" << endl;

        for (int i = 0; i < row_; ++i)  
        {
            cout << " "; // Empty spots
            for (int j = 0; j < column_; ++j)
            {
                cout << "+-"; // Upper border 
            }
            cout << "+" << endl;
            
            cout << setw(2) << (row_ - i) << " "; 

            for (int j = 0; j < column_; ++j) 
            {
                cout << "|" << map_[i][j]; // Board dividers
            }
            cout << "|" << endl;
        }

        cout << " ";
        for (int j = 0; j < column_; ++j) // Display lower border of the last row
        {
            cout << "+-";
        }
        cout << "+" << endl;
    
        cout << " ";
        for (int j = 0; j < column_; ++j) // Display column numbers
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
        for (int j = 0; j < column_; ++j) // Put 1 above numbers higher than 9
        {
            cout << " " << (j + 1) % 10;
        }
        cout << "\n\n";
    }

    int getColumn() const
    {
        return column_;
    }

    int getRow() const
    {
        return row_;
    }

    void setObject(int &x, int &y, char &ch) 
    {
        map_[-(y - row_)][x - 1] = ch;
    }

    char getObject(int x, int y) const
    {
        return map_[-(y - row_)][x - 1];
    }

    bool isEmpty(int x, int y)
    {
        return isspace(map_[-(y - row_)][x - 1]);
    }

    bool inBoard(int x, int y)
    {
        return (x > 0 && x <= column_ && y > 0 && y <= row_);
    }

    void arrowDirection()
    {
        int row, column;
        string direction;
        char oldDirection, newDirection;

        cout << "Enter row, column, and direction: ";
        cin >> row >> column >> direction;

        oldDirection = getObject(column, row);

        if (direction == "right")
        {
            newDirection = '>';
        }
        else if (direction == "left")
        {
            newDirection = '<';
        }
        else if (direction == "up")
        {
            newDirection = '^';
        }
        else if (direction == "down")
        {
            newDirection = 'v';
        }

        setObject(column, row, newDirection);

        cout << "\nArrow " << oldDirection << " is switched to " << newDirection << ".\n\n";
        system("pause");
    }

    char randomObj()
    {
        char objects[] = {'^', 'v', '>', '<', 'p', 'h', 'r', ' ', ' ', ' ', ' ', ' '};
        int noObj = 12;

        static random_device random;
        uniform_int_distribution<int> obj(0, noObj); // Object randomizer

        return objects[obj(random)];
    }
};

class Alien // Alien class
{
private:
    int life_{100}, attack_{0}, x_, y_; 
    char heading_{'A'}, trail_{'.'}; // Alien character and trail
    bool turns_{1}; // Begin turn

public:
    Alien(){};

    int getX() const
    {
        return x_;
    }

    int getY() const
    {
        return y_;
    }

    void alienStatus()
    {
        cout << "Alien: Life: " << life_ << " Attack: " << attack_ << endl; // Alien stats display
    }

    void lifeUpdate(int &damage) // Life status
    {
        life_ = life_ - damage;
    }

    void initPos(Land &playMap) // Places Alien at the middle of the board
    {
        x_ = playMap.getColumn() / 2 + 1; // Odd number position
        y_ = (playMap.getRow() / 2) + 1;

        playMap.setObject(x_, y_, heading_);
    }

    void move(const string &input, Land &playMap) // Alien moveset
    {
        if (input == "up")
        {
            moveUp(playMap);
        }
        else if (input == "down")
        {
            moveDown(playMap);
        }
        else if (input == "left")
        {
            moveLeft(playMap);
        }
        else if (input == "right")
        {
            moveRight(playMap);
        }
    }

    void moveStep(int &oldPosX, int &oldPosY, int &newPosX, int &newPosY, Land &playMap) 
    {
        char obj = playMap.getObject(newPosX, newPosY);

        playMap.setObject(x_, y_, heading_);
        playMap.setObject(oldPosX, oldPosY, trail_);

        if (obj == '>' || '<' || '^' || 'v') // Arrow direction changes
        {
            moveArrow(playMap, obj);
        }
    }

    void moveUp(Land &playMap) // Moving up
    {
        while (turns_)
        {
            int oldPosX{x_}, oldPosY{y_};
            int newposX{x_}, newPosY{y_ + 1};

            moveObstacles(playMap, newposX, newPosY);

            if (turns_)
            {
                y_++;
                moveStep(oldPosX, oldPosY, newposX, newPosY, playMap);
            }
        }
    }

    void moveDown(Land &playMap) // Moving down
    {
        while (turns_)
        {
            int oldPosX{x_}, oldPosY{y_};
            int newposX{x_}, newPosY{y_ - 1};

            moveObstacles(playMap, newposX, newPosY);

            if (turns_)
            {
                y_--;
                moveStep(oldPosX, oldPosY, newposX, newPosY, playMap);
            }
        }
    }

    void moveLeft(Land &playMap) // Moving left
    {
        while (turns_)
        {
            int oldPosX{x_}, oldPosY{y_};
            int newposX{x_ - 1}, newPosY{y_};

            moveObstacles(playMap, newposX, newPosY);

            if (turns_)
            {
                x_--;
                moveStep(oldPosX, oldPosY, newposX, newPosY, playMap);
            }
        }
    }

    void moveRight(Land &playMap) // Moving right
    {
        while (turns_)
        {
            int oldPosX{x_}, oldPosY{y_};
            int newposX{x_ + 1}, newPosY{y_};

            moveObstacles(playMap, newposX, newPosY);

            if (turns_)
            {
                x_++;
                moveStep(oldPosX, oldPosY, newposX, newPosY, playMap);
            }
        }
    }

    void moveObstacles(Land &playMap, int &newposX, int &newPosY) // Obstacles
    {
        if (playMap.getObject(newposX, newPosY) == 'h')
        {
            healthObstacles(playMap);
        }
        else if (playMap.getObject(newposX, newPosY) == 'p')
        {
            podObstacles(playMap);
        }
        else if (playMap.getObject(newposX, newPosY) == 'r')
        {
            rockObstacles(playMap, newposX, newPosY);
        }
        else if (!playMap.inBoard(newposX, newPosY))
        {
            borderObstacles(playMap);
        }
        else if ((playMap.getObject(newposX, newPosY) == '>') || (playMap.getObject(newposX, newPosY) == '<') || playMap.getObject(newposX, newPosY) == '^' || playMap.getObject(newposX, newPosY) == 'v')
        {
            arrowObstacles(playMap);
        }
        else if (isdigit(playMap.getObject(newposX, newPosY)))
        {
            attack(playMap, newposX, newPosY);
        }
        
    }

    void healthObstacles(Land &playMap)
    {
        cout << "Alien finds a health pack.\n";
        cout << "Life is increased by 20\n\n"; // Getting a health pack

        life_ += 20;
        cout << "Alien: \nLife: " << life_ << " Attack: " << attack_ << "\n\n";

        playMap.display();

        system("pause");
    }

    void podObstacles(Land &playMap) // Pods
    {
        playMap.display();
        system("pause");
    }

    void rockObstacles(Land &playMap, int &x, int y) // Rocks
    {
        cout << "Alien stumbles upon a rock\n"; // Reaching a rock

        char objects[] = {'h', 'p', ' ', '>', '<', '^', 'v', ' ', ' ', ' ', ' ', ' '};
        int noOfObjects = 12;

        static random_device device;
        uniform_int_distribution<int> random(0, noOfObjects - 1);

        char newObj = objects[random(device)];

        playMap.display();

        switch (newObj)
        {
        case 'h':
            cout << "There's a health pack beneath the rock.\n\n"; // Health pack under rock
            break;
        case 'p':
            cout << "There's a pod beneath the rock.\n\n"; // Pod under rock
            break;
        case '>':
        case '<':
        case '^':
        case 'v':
            cout << "There's an arrow beneath the rock.\n\n"; // Arrow under rock
            break;
        default:
            cout << "There's nothing beneath the rock.\n\n"; // Empty rock
            break;
        }
        playMap.setObject(x, y, newObj);
        system("pause");

        turns_ = 0; // End turn
    }

    void borderObstacles(Land &playMap) // Alien reaches board boundary
    {
        playMap.display();
        cout << "Alien hits a border.\n\n";
        system("pause");

        turns_ = 0; // End turn
    }

    void arrowObstacles(Land &playMap)
    {
        playMap.display();
        cout << "Alien finds an arrow\n";
        cout << "Attack increased by 10\n\n";

        attack_ += 20;
        cout << "Alien: \nLife: " << life_ << " Attack: " << attack_ << "\n\n"; // Alien stats

        system("pause");
    }

    void moveArrow(Land &playMap, const char &arrowObj)
    {

        switch (arrowObj) // Arrow changes direction
        {
        case '>':
            moveRight(playMap);
            break;
        case 'v':
            moveDown(playMap);
            break;
        case '<':
            moveLeft(playMap);
            break;
        case '^':
            moveUp(playMap);
            break;
        }
    }

    void attack(Land &playMap, int &newX, int &newY)
    {
        turns_ = 0;
    }
};

class Zombie // Zombie class
{
private:
    int life_, attack_, range_, x_, y_, zombieIndex_; // Zombie stats
    char zombie_;
    bool turns_{0};
    vector<int> xCoord_, yCoord_;

public:
    Zombie(){};

    int getX() const
    {
        return x_;
    }

    int getY() const
    {
        return y_;
    }

    int zombieTurns(){ // Zombie turns
        return turns_ = 1;
        
    }

    void setPosX(const int &x){ 
        
        xCoord_.push_back(x);
    }

    void getPosX(){
        x_ = xCoord_[zombieIndex_];
    }

    void setPosY(const int &y){ 
        
        yCoord_.push_back(y);
    }

    void getPosY(){
        y_ = yCoord_[zombieIndex_];
    }

    int getAttack()
    {
        return attack_;
    }

    void zombieStatus(int &numOfZombies) // Zombie status
    {
        for (int x = 1; x <= numOfZombies; x++)
        {
            range_ = 1 + (rand() % 5); // Attack range
            life_ = 100 + (rand() % 201); // Life
            attack_ = 5 + (rand() % 30); // Damage
            cout << "Zombie " << x << " : Life " << life_ << ", Attack " << attack_ << ", Range " << range_ << endl;
        };
        cout << endl;
    }

    void zombiePos(Land &playMap, const int &numOfZombies) // Zombie positions
    {

        char zombieNum_[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'}; // Zombies

        static random_device random;
        uniform_int_distribution<int> column(0, playMap.getColumn() - 1); // Column
        uniform_int_distribution<int> row(0, playMap.getRow() - 1); // Row

        for (int x = 0; x < numOfZombies; x++)
        {

            x_ = column(random);
            y_ = row(random);

            setPosX(x_);
            setPosY(y_);

            playMap.setObject(x_, y_, zombieNum_[x]); // Prevent objects and zombies from intersecting
        };
    }

    void savePos(){ // Zombie positions record

        getPosX();
        getPosY();

        xCoord_[zombieIndex_] = x_;
        yCoord_[zombieIndex_] = y_;
    }

    void zombieMainMove(Land &playMap, Alien &player, Zombie &bots, const int &numOfZombies)
    {
        if (turns_)
        {
            for (int x = 0; x < numOfZombies; x++)
            {
                 char zombieNum_[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'}; // Zombies. Only max 9
                 zombie_ = zombieNum_[x];

                 zombieIndex_ = x;

                 getPosX();
                 getPosY();

                zombieMove(playMap);
                zombieAttack(playMap, player);
                playMap.display();
            }
            turns_ = 1;
        }

    }

    void zombieMove(Land &playMap) // Zombie moveset
    {
        int noOfMoves = 4;
        char zombie_;
    
        srand(time(NULL)); // Randomise movement
        int moves = rand() % 4 + 1;

        switch (moves) // Possible zombie movesets
        {
        case 1:
            zombieUp(playMap);
            break;

        case 2:
            zombieDown(playMap);
            break;

        case 3:
            zombieLeft(playMap);
            break;

        case 4:
            zombieRight(playMap);
            break;
        }
    }

    void zombieUp(Land &playMap) // Zombie moves up
    {
        int oldZomPosX{x_}, oldZomPosY{y_};
        int newZomPosX{x_}, newZomPosY{y_ + 1};

        char newObj = playMap.randomObj();

        if (playMap.getObject(newZomPosX, newZomPosY) != 'A' || zombie_ )
        {
            playMap.setObject(newZomPosX, newZomPosY, zombie_);
            playMap.setObject(oldZomPosX, oldZomPosY, newObj);

            cout << "Zombie " << zombie_ << " moves up.\n";

            savePos();

            system("pause");
        }

        else{
            zombieMove(playMap);
        }

    }

    void zombieDown(Land &playMap) // Zombie moves down
    {
        int oldZomPosX{x_}, oldZomPosY{y_};
        int newZomPosX{x_}, newZomPosY{y_ - 1};

        char newObj = playMap.randomObj();

        if (playMap.getObject(newZomPosX, newZomPosY) != 'A' || zombie_ )
        {
            playMap.setObject(newZomPosX, newZomPosY, zombie_);
            playMap.setObject(oldZomPosX, oldZomPosY, newObj);

            cout << "Zombie " << zombie_ << " moves down.\n"; // Zombie moving nortification

            savePos();

            system("pause");
        }

         else{
            zombieMove(playMap);
        }       
    }

    void zombieLeft(Land &playMap) //Zombie moves left
    {
        int oldZomPosX{x_}, oldZomPosY{y_};
        int newZomPosX{x_ - 1}, newZomPosY{y_};

        char newObj = playMap.randomObj();

        if (playMap.getObject(newZomPosX, newZomPosY) != 'A' || zombie_ )
        {
            playMap.setObject(newZomPosX, newZomPosY, zombie_);
            playMap.setObject(oldZomPosX, oldZomPosY, newObj);

            cout << "Zombie " << zombie_ << " moves left.\n"; // Zombie moving nortification

            savePos();

            system("pause");
        }

         else{
            zombieMove(playMap);
        }
        
    }

    void zombieRight(Land &playMap) // Zombie moves right
    {
        int oldZomPosX{x_}, oldZomPosY{y_};
        int newZomPosX{x_ + 1}, newZomPosY{y_}; // Zombie moveset check

        char newObj = playMap.randomObj();

        if (playMap.getObject(newZomPosX, newZomPosY) != 'A' || zombie_ )
        {
            playMap.setObject(newZomPosX, newZomPosY, zombie_);
            playMap.setObject(oldZomPosX, oldZomPosY, newObj);

            cout << "Zombie " << zombie_ << " moves right.\n";

            savePos();

            system("pause");
        }

         else{
            zombieMove(playMap);
        }
        
    }

    void zombieAttack(Land &playMap, Alien &player) // When a zombie attacks Alien
    { 
        int newZomPosX{x_}, newZomPosY{y_};
        int attackValue = getAttack();
        int i = range_;

        if (playMap.getObject(x_ + i, y_) == 'A' || playMap.getObject(x_ - i, y_) == 'A' || playMap.getObject(x_, y_+i) == 'A' || playMap.getObject(x_ , y_-i) == 'A')
        {
            player.lifeUpdate(attackValue);
            cout << "\nZombie" << zombie_ <<  " has attacked Alien\n" ; // Hitting a zombie
        }
        else
        {
            cout << "\nZombie " << zombie_ << " failed to attack\n"; // No damage received
        }
        
    }
    }

;

void displayMenu();
void changeGameSettings(int &boardRows, int &boardColumns, int &numOfZombies, bool &settingsDone);
void startGame(const int &boardColumns, const int &boardRows, const int &numOfZombies);
void gameDashboard(Zombie &bots, Alien &player);
void gameControl(Alien &player, Land &playMap);
void commandHelp();

int main()
{


    char choice;
    int numOfRows{19}, numOfColumns{3}, numOfZombies{1}; // Default settings
    bool done{0}, settingsDone{0};
    Alien player;
    Zombie bots;

    displayMenu();
    cin >> choice;
    switch (choice)
    {
    case '1':
        GameSettings:
        cout << "\nGame Settings\n";
        cout << "----------------\n";

        cout << "Board Columns : " << numOfRows << "\n";
        cout << "Board Rows : " << numOfColumns << "\n";
        cout << "Zombie Count : " << numOfZombies << "\n\n";

        char changeSettings;

        cout << "Do you wish to change the game settings? (y/n) => "; // Setting change prompt
        cin >> changeSettings;
        switch (changeSettings)
        {
        case 'y':
            while (!settingsDone)
            {
                changeGameSettings(numOfRows, numOfColumns, numOfZombies, settingsDone);
            }
            
            startGame(numOfRows, numOfColumns, numOfZombies);
        case 'n':
            startGame(numOfRows, numOfColumns, numOfZombies);
        default:
            cout << "Invalid selection, try again!";
            goto GameSettings;
        }

    case '2':
        // create a load game function
        break;

    case 'Q':
        done = true;
        break;

    default:
        cout << "\nInvalid selection, try again!" << endl;
        cout << endl;
        displayMenu();
    }
    system("pause");
}

void displayMenu()
{
    cout << endl; // Start up menu
    cout << "+---------------------------------------------+" << endl;
    cout << "|               ALIEN VS ZOMBIE               |" << endl;
    cout << "|=============================================|" << endl;
    cout << "| Select:                                     |" << endl;
    cout << "|    1 => New Game                            |" << endl;
    cout << "|    2 => Load Game                           |" << endl;
    cout << "|---------------------------------------------|" << endl;
    cout << "|    Q => Quit                                |" << endl;
    cout << "+---------------------------------------------+" << endl;
    cout << endl;
    cout << "Choice => "; // Input
}

void changeGameSettings(int &boardRows, int &boardColumns, int &numOfZombies, bool &exit)
{
    // system("cls");
    cout << "\nBoard Settings\n";
    
    cout << "Enter rows => "; // Row input
    cin >> boardColumns;
    cout << endl;


    cout << "Enter columns => "; // Column input
    cin >> boardRows;
    cout << endl;


    cout << "\nZombie Settings\n";

    cout << "Enter number of zombies => ";
    cin >> numOfZombies;
    cout << endl;

    if(boardColumns%2 == 0 || boardRows%2 == 0){ // Error pop up
        cout << "\nERROR ! Only odd numbers are accepted! Try again.\n\n";
    }
    else{
        exit = true;
    }

}

void startGame(const int &boardColumns, const int &boardRows, const int &numOfZombies) // Start game
{
    Land playMap = Land(boardColumns, boardRows);

    Alien player;
    player.initPos(playMap); // Place alien at center

    Zombie bots;
    bots.zombiePos(playMap, numOfZombies); // Generates zombies

    while (true)
    {

        playMap.display();
        gameControl(player, playMap);
        playMap.display();
        bots.zombieTurns();
        bots.zombieMainMove(playMap, player, bots, numOfZombies);
        gameDashboard(bots, player);
   
    }
}

void gameDashboard(Zombie &bots, Alien &player)
{
    int zombieIndex_;

    player.alienStatus();
    bots.zombieStatus(zombieIndex_);
    
}

void gameControl(Alien &player, Land &playMap) // Game controls
{
    string userInput;
    cout << "Command> ";
    cin >> userInput;
    cout << "\n";

    if (userInput == "up" || "down" || "left" || "right") // Moveset command
    {
        player.move(userInput, playMap);
    }
    if (userInput == "arrow")
    {
        playMap.arrowDirection(); // Change arrow direction
    }
        if (userInput == "help") // Help
    {
        commandHelp();
    }
        if (userInput == "save") // Save
    {
    }
        if (userInput == "load") // Load
    {
    }
        if (userInput == "quit") // Quit
    {
        exit(0);
    }
}

void commandHelp() // Help display
{
    cout << "Commands\n";
    cout << "1. up - Move Up.\n";
    cout << "2. down - Move down.\n";
    cout << "3. left - Move left.\n";
    cout << "4. right - Move right.\n";
    cout << "5. arrow - Change the direction of an arrow.\n";
    cout << "6. help - Display these user commands.\n";
    cout << "7. save - Save the game.\n";
    cout << "8. load - Load a game\n";
    cout << "9. quit - Quit the game.\n";
}
