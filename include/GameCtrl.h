#ifndef SNAKE_GAMECTRL_H_
#define SNAKE_GAMECTRL_H_

#include "model/Snake.h"
#include "util/Console.h"
#include <thread>
#include <mutex>

/*
Game controller.
*/
class GameCtrl {
public:
    typedef Map::SizeType SizeType;

    ~GameCtrl();

    /*
    Return the only instance
    */
    static GameCtrl* getInstance();

    /*
    Game configuration setters.
    */
    void setFPS(const double fps_);
    void setEnableAI(const bool enableAI_);
    void setMoveInterval(const long ms);
    void setRecordMovements(const bool b);
    void setRunTest(const bool b);
    void setMapRow(const SizeType n);
    void setMapCol(const SizeType n);

    /*
    Sleep current thread for a few milliseconds.
    */
    void sleepFor(const long ms) const;

    /*
    Run the game.

    @return The exit status of the program.
    */
    int run();

private:
    static const std::string MSG_BAD_ALLOC;
    static const std::string MSG_LOSE;
    static const std::string MSG_WIN;
    static const std::string MSG_ESC;
    static const std::string MAP_INFO_FILENAME;

    SizeType mapRowCnt = 10;
    SizeType mapColCnt = 10;
    double fps = 60.0;
    long moveInterval = 30;
    bool enableAI = true;
    bool runTest = false;
    bool recordMovements = true;

    Snake snake;
    std::shared_ptr<Map> map;

    volatile bool pause = false;  // Control pause/resume game

    volatile bool runMainThread = true;  // Switch of the main thread
    volatile bool runSubThread = true;   // Switch of sub-threads

    std::thread drawThread;      // Thread to draw the map
    std::thread keyboardThread;  // Thread to receive keyboard instructions
    std::thread foodThread;      // Thread to create food
    std::thread moveThread;      // Thread to move the snake

    std::mutex mutexMove;  // Mutex of moveSnake()
    std::mutex mutexExit;  // Mutex of exitGame()

    FILE *movementFile = nullptr;  // File to save snake movements

    GameCtrl();

    /*
    Sleep for a few time according to FPS.
    */
    void sleepByFPS() const;

    /*
    Print a message and exit the game.
    */
    void exitGame(const std::string &msg);

    /*
    Print an error message and exit the game.
    */
    void exitGameErr(const std::string &err);

    /*
    Move the snake and check if the game is over.
    */
    void moveSnake(Snake &s);

    /*
    Write the map content to file.
    */
    void writeMapToFile() const;

    /*
    Initialize.
    */
    void init();
    void initMap();
    void initSnake();
    void initFiles();

    /*
    Start all threads.
    */
    void startThreads();

    /*
    Draw thread.
    */
    void draw();
    void drawMapContent() const;
    void drawTestPoint(const Point &p, const ConsoleColor &consoleColor) const;

    /*
    Keyboard thread.
    */
    void keyboard();
    void keyboardMove(Snake &s, const Direction d);

    /*
    Food thread.
    */
    void createFood();

    /*
    Move thread
    */
    void autoMove();

    /*
    Test functions.
    */
    void testFood();
    void testSearch();
};

#endif
