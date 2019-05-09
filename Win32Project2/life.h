#ifndef LIFE_H_INCLUDED
#define LIFE_H_INCLUDED

#define THREADS_ENABLED

// TODO
/*
    cuda
    переключение многопоточного режима
    предпредыдущий кадр для отлова статики
    перемещение карты
    при увеличении размера карты - раздвигать ее, а не перегенерировать
    доделать настройку потоков и пр
*/


#include <cstdlib>
#include <chrono>
#include <vector>
#include <iostream>


#ifdef THREADS_ENABLED
#include <thread>
#include <mutex>
#include <condition_variable>
#endif

/*
#ifndef NDEBUG
srand(666);
#endif
*/

typedef unsigned char cellType;


void removeMap(cellType** map, int width);


struct ThreadConfig {
    bool alive = true;
    bool run;

    int startX;
    int startY;
    int stopX;
    int stopY;
    long aliveCells;
};


struct HistoryItem {
    unsigned long alive;
    int width;
    cellType** map;

    HistoryItem(unsigned int alive, int width, cellType** map) : width(width), alive(alive), map(map) { }
    ~HistoryItem() { removeMap(map, width); }
};


class Life {
public:
    cellType maxAge;
    int mapWidth;
    int mapHeight;

    unsigned long alive;
    unsigned long frame;
    unsigned long durationStep;

    cellType** map;
    cellType** newMap;

    int historySize;
    bool historyEnabled = true;
    std::vector <HistoryItem*> history;

    Life();
    Life(int threadsCount);
    Life(int width, int height, int threadsCount = 0);
    ~Life();

    void clearHistory(int start = 0, int end = -1);

    void resizeMap(int width, int height);
    void newGame(bool empty = false);
    void generateMap(bool empty = false);

    cellType** copyMap(cellType** sourceMap);
    void copyMap(cellType** sourceMap, cellType** targetMap);

    void normalize(int& x, int& y);
    cellType getCell(int x, int y);
    void setCell(int x, int y, cellType v = 1);

    char getSumMur(int x, int y);
    char getSumMurFast(int x, int y);
    char handleCell(int x, int y);
    char handleCellFast(int x, int y);
    void save();
    void step();
    void back();

    int threadsCount;

#ifdef THREADS_ENABLED
    std::mutex waitLock;
    std::condition_variable threadsStart;
    std::condition_variable threadsFinish;

    std::vector<ThreadConfig*> configs;
    std::vector<std::thread*> threads;

    void partStep(ThreadConfig& config);
    void waitThreads();
#endif
};

#endif
