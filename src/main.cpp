#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <vector>
#include <climits>
#include <algorithm>

void readInput(const std::string&      fileName,
                     std::vector<int>& termX_arr,
                     std::vector<int>& termY_arr,
                     std::vector<int>& obsX1_arr,
                     std::vector<int>& obsY1_arr,
                     std::vector<int>& obsX2_arr,
                     std::vector<int>& obsY2_arr)
{
  int maxX = 0;
  int maxY = 0;
  int numPin = 0;
  int numObs = 0;

  std::ifstream fileStream(fileName);
  std::string line;
  std::istringstream iss;

  // First line shoud be "XXYYZZ STP File, STP Format Version 1.0"
  std::getline(fileStream, line); 

  while(line != "SECTION Comments")
    std::getline(fileStream, line);

  // SECTION Comments
  while(line != "END")
    std::getline(fileStream, line);

  do std::getline(fileStream, line);
  while(line != "SECTION Graph");

  // SECTION Graph
  while(line != "END")
  {
    std::getline(fileStream, line);
    iss.clear();
    iss.str(line);

    std::string tempStr;
    unsigned int tempInt;
    iss >> tempStr >> tempInt;

    if(tempStr == "Nodes")
      numPin = tempInt;
    if(tempStr == "Obstacles")
      numObs = tempInt;
  }

  printf("Nodes     : %d\n", numPin);
  printf("Obstacles : %d\n", numObs);

  while(line != "SECTION Coordinates")
    std::getline(fileStream, line);

  std::getline(fileStream, line);

  // SECTION Coordinates
  int numPinRead = 0;
  while(line != "END")
  {
    iss.clear();
    iss.str(line);

    std::string tempStr;
    int termX;
    int termY;
    iss >> tempStr >> numPinRead >> termX >> termY;

    termX_arr.push_back(termX);
    termY_arr.push_back(termY);

    if(termX > maxX) maxX = termX;
    if(termY > maxY) maxY = termY;

    std::getline(fileStream, line);
  }

  assert(numPinRead == numPin);

  while(line != "SECTION Obstacles")
    std::getline(fileStream, line);

  std::getline(fileStream, line);

  // SECTION Obstacles
  int numObsRead = 0;
  while(line != "END")
  {
    iss.clear();
    iss.str(line);

    std::string tempStr;
    int x1;
    int y1;
    int x2;
    int y2;
    iss >> tempStr >> x1 >> y1 >> x2 >> y2;
    numObsRead++;

    obsX1_arr.push_back(x1);
    obsY1_arr.push_back(y1);
    obsX2_arr.push_back(x2);
    obsY2_arr.push_back(y2);

    if(x1 > maxX) maxX = x1;
    if(y1 > maxY) maxY = y1;
    if(x2 > maxX) maxX = x2;
    if(y2 > maxY) maxY = y2;

    std::getline(fileStream, line);
  }

  assert(numObsRead == numObs);
}

int main(int argc, char** argv)
{
  std::vector<int> termX;
  std::vector<int> termY;
  std::vector<int> obsX1;
  std::vector<int> obsY1;
  std::vector<int> obsX2;
  std::vector<int> obsY2;

  int maxX;
  int maxY;

  if(argc < 2)
    printf("Error - No Input file!\n");
  else
    readInput(std::string(argv[1]), termX, termY, obsX1, obsY1, obsX2, obsY2, maxX, maxY);

  return 0;
}
