#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
const int SIZE = 32;
double distance(int x, int y, int targetX, int targetY);
void readMap(int[SIZE][SIZE]);
void writeMap(int[SIZE][SIZE]);

struct Node
{
    Node *parent;
    double gCost, hCost, fCost;
    int x, y, length;

    Node(Node *parent, int x, int y, int goalX, int goalY, int cost) : parent(parent),
                                                             hCost(distance(x, y, goalX, goalY)),
                                                             gCost(cost + ((parent != nullptr) ? (distance(x, y, parent->x, parent->y) + parent->gCost) : 0)),
                                                             fCost(hCost + gCost),
                                                             length((parent != nullptr) ? (1 + parent->length) : 1){};
};
s
vector<Node> getNeighbors(Node * parent, int map[SIZE][SIZE]);
int main(int argc, char const *argv[])
{

    return 0;
}
double distance(int x, int y, int targetX, int targetY)
{
    return sqrt(pow(x - targetX, 2) + pow(y - targetY, 2));
}
/**
 * writeMap
 *
 * Write a map into map-out.txt in such a way that the [0][0] element
 * ends up in the bottom left corner of the file (so that the contents
 * of the file look like the relevant occupancy grid.
 *
 **/
void writeMap(int map[SIZE][SIZE])
{
    std::ofstream mapFile;
    mapFile.open("map-out.txt");

    for (int i = SIZE - 1; i >= 0; i--)
    {
        for (int j = 0; j < SIZE; j++)
        {
            mapFile << map[i][j];
        }
        mapFile << std::endl;
    }

    mapFile.close();
}
/**
 * readMap
 *
 * Reads in the contents of the file map.txt into the array map
 * in such a way that the first element of the last row of the
 * file map.txt is in element [0][0].
 *
 * This means that whatever is in the file looks like the occupancy
 * grid would if you drew it on paper.
 *
 **/
void readMap(int map[SIZE][SIZE])
{
    std::ifstream mapFile;
    mapFile.open("map.txt");

    for (int i = SIZE - 1; i >= 0; i--)
    {
        for (int j = 0; j < SIZE; j++)
        {
            mapFile >> map[i][j];
        }
    }

    mapFile.close();
}
vector<Node> getNeighbors(Node * parent, int map[SIZE][SIZE]){
    if (parent.x < 0 || parent.x >= SIZE || parent.y < 0 || parent.y >= SIZE)
        throw invalid_argument;
    vector<Node> neighbors;
    for (int x = -1; x < 2; x++){
        for (int y = -1; y < 2; y++){
            if (x == 0 && y == 0) continue;
            int xIndex = parent->x + x; int yIndex = parent->y + y;
            if (xIndex < 0 || xIndex >= SIZE || yIndex < 0 || yIndex >= SIZE)
                continue;

        }
    }
}