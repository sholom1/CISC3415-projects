#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <string>
#include <queue>
#include <algorithm>
#include <iomanip>

using namespace std;
const int SIZE = 32;
double distance(int x, int y, int targetX, int targetY);
void readMap(int[SIZE][SIZE]);
void writeMap(int[SIZE][SIZE]);
void printMap(int map[SIZE][SIZE]);
void invertMap(int map[SIZE][SIZE]);
template <typename Queue> void printQueue(Queue queue);

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
    int resultMap[SIZE][SIZE];
    MapRange MapPos(0, 31, -8, 8);
    const int startX = ceil(MapPos(-6)); const int startY = ceil(MapPos(-6)); const int endX = ceil(MapPos(6.5)); const int endY = ceil(MapPos(6.5));
    readMap(map);
    readMap(resultMap);
    invertMap(map);
    invertMap(resultMap);
    printMap(map);
    auto compare = [](const Node* lhs, const Node* rhs){return lhs < rhs;};
    priority_queue<Node, vector<Node*>, decltype(compare)> open(compare);
    vector<const Node*> closed;
    open.push(new Node(nullptr, startX, startY, endX, endY, 1));
    // Node *startPos = new Node(nullptr, startX, startY, endX, endY, 1);
    while (open.size() > 0)
    {
        printQueue(open);
        const Node* currentNode = open.top();
        cout << "Visiting Node: " << currentNode->to_string() << endl;
        if(currentNode->x == endX && currentNode->y == endY){
            cout << "Goal reached!" << endl;
            return 0; //TODO: trace path
        }
        open.pop();
        resultMap[currentNode->x][currentNode->y] = currentNode->fCost;
        printMap(resultMap);
        closed.push_back(currentNode);
        for(Node* n : getNeighbors(currentNode, map, endX, endY)){
            auto closedIter = find(closed.begin(), closed.end(), n);
            if (closedIter != closed.end())
                if ((**closedIter) < (*n))
                    continue;
                else
                    closed.erase(closedIter);
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
/**
 * printMap
 *
 * Print map[][] out on the screen. The first element to be printed
 * is [SIZE][0] so that the result looks the same as the contents of
 * map.txt
 *
 **/
void printMap(int map[SIZE][SIZE])
{
  for(int i = SIZE -1; i >= 0; i--){
    for(int j = 0; j < SIZE; j++)
      {
	    std::cout << std::setw(3) << map[i][j] << " ";
      }
    std::cout << std::endl;
  }

}
void invertMap(int map[SIZE][SIZE]){
    for (int x = 0; x < SIZE; x++){
        for (int y = 0; y < SIZE; y++){
            if (map[x][y] == 1)
                map[x][y] = -1;
            else if (map[x][y] == 0)
                map[x][y] = 1;
        }
    }
}
template <typename Queue> void printQueue(Queue queue){
    while (!queue.empty()){
        cout << queue.top()->to_string() << endl;
        queue.pop();
    }
}