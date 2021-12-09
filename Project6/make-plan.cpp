#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <string>
#include <queue>
#include <algorithm>

using namespace std;
const int SIZE = 32;
double distance(int x, int y, int targetX, int targetY);
void readMap(int[SIZE][SIZE]);
void writeMap(int[SIZE][SIZE]);

struct Node
{
    const Node *parent;
    double gCost, hCost, fCost;
    int x, y, length;

    Node(const Node *parent, int x, int y, int goalX, int goalY, int cost) : parent(parent), x(x), y(y),
                                                             hCost(distance(x, y, goalX, goalY)),
                                                             gCost(cost + ((parent != nullptr) ? (distance(x, y, parent->x, parent->y) + parent->gCost) : 0)),
                                                             fCost(hCost + gCost),
                                                             length((parent != nullptr) ? (1 + parent->length) : 1){};
    string to_string() const {
        return "X: " + std::to_string(x) + " Y: " + std::to_string(y) + " G: " + std::to_string(gCost) + " H: " + std::to_string(hCost) + " F: " + std::to_string(fCost);
    }
    bool operator<(const Node& rhs) const{
        return fCost < rhs.fCost;
    }
    bool operator==(const Node& rhs) const{
        return x == rhs.x && y == rhs.y;
    }
};
class MapRange{
    int RealStart, RealEnd;
    int MapStart, MapEnd;
    double slope;
    public:
        MapRange(int RealStart, int RealEnd, int MapStart, int MapEnd):RealStart(RealStart),RealEnd(RealEnd),
            MapStart(MapStart),MapEnd(MapEnd),
            slope(1.0 * (RealEnd-RealStart)/(MapEnd-MapStart)){};
        double operator()(double n){
            return RealStart + slope * (n - MapStart);
        }
};
vector<Node*> getNeighbors(const Node * parent, int map[SIZE][SIZE], int goalX, int goalY);
int main(int argc, char const *argv[])
{
    int map[SIZE][SIZE];
    MapRange MapPos(0, 31, -8, 8);
    const int startX = ceil(MapPos(-6)); const int startY = ceil(MapPos(-6)); const int endX = ceil(MapPos(6.5)); const int endY = ceil(MapPos(6.5));
    readMap(map);
    priority_queue<Node*> open;
    vector<const Node*> closed;
    open.push(new Node(nullptr, startX, startY, endX, endY, 1));
    // Node *startPos = new Node(nullptr, startX, startY, endX, endY, 1);
    while (open.size() > 0)
    {
        const Node* currentNode = open.top();
        cout << "Visiting Node: " << currentNode->to_string() << endl;
        if(currentNode->x == endX && currentNode->y == endY){
            cout << "Goal reached!" << endl;
            break; //TODO: trace path
        }
        open.pop();
        closed.push_back(currentNode);
        for(Node* n : getNeighbors(currentNode, map, endX, endY)){
            if (find(closed.begin(), closed.end(), n) != closed.end())
                continue;
            open.push(n);
        }
    }
    
    // cout << startPos->to_string() << endl;
    // for (auto* n : getNeighbors(startPos, map, endX, endY))
    //     cout << n->to_string() << endl;
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
vector<Node*> getNeighbors(const Node * parent, int map[SIZE][SIZE], int goalX, int goalY){
    if (parent->x < 0 || parent->x >= SIZE || parent->y < 0 || parent->y >= SIZE)
        throw invalid_argument("Invalid argument out of bounds parent");
    vector<Node*> neighbors;
    for (int x = -1; x < 2; x++){
        for (int y = -1; y < 2; y++){
            if (x == 0 && y == 0) continue;
            int xIndex = parent->x + x; int yIndex = parent->y + y;
            if (xIndex < 0 || xIndex >= SIZE || yIndex < 0 || yIndex >= SIZE)
                continue;
            if (map[xIndex][yIndex] != -1){
                Node * n = new Node(parent, xIndex, yIndex, goalX, goalY, map[xIndex][yIndex]);
                neighbors.push_back(n);
            }
        }
    }
    return neighbors;
}