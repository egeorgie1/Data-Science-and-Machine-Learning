#include <iostream>
#include <algorithm>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
using namespace std;

struct iris
{
    double attributes[4];
    string classType;

    iris( double sL = 0.0, double sW = 0.0, double pL = 0.0, double pW = 0.0, string cT = "" )
    {
        attributes[0] = sL;
        attributes[1] = sW;
        attributes[2] = pL;
        attributes[3] = pW;
        classType = cT;
    }

};
const int K = 3;
vector<iris> flowers;
iris testIris;

bool operator<( const iris& iP1, const iris& iP2 )
{   //calculate Euclidean distance to the test example
    double dist1 =  pow((iP1.attributes[0] - testIris.attributes[0]),2.0) +
                    pow((iP1.attributes[1]  - testIris.attributes[1]),2.0) +
                    pow((iP1.attributes[2] - testIris.attributes[2]),2.0) +
                    pow((iP1.attributes[3]  - testIris.attributes[3]),2.0);

    double dist2 =  pow((iP2.attributes[0] - testIris.attributes[0]),2.0) +
                    pow((iP2.attributes[1]  - testIris.attributes[1]),2.0) +
                    pow((iP2.attributes[2] - testIris.attributes[2]),2.0) +
                    pow((iP2.attributes[3]  - testIris.attributes[3]),2.0);

    return sqrt(dist1) < sqrt(dist2);
}

void readData()
{
    ifstream fin;
    fin.open("iris.txt");

    char inputLine[100];
    while( fin.getline(inputLine, 100) )
    {
        double attr[4];
        int idxAttr = 0;
        string value = "";
        int j = 0;
        while( !isalpha(inputLine[j]) )
        {
            if( inputLine[j] == ',' )
            {
                attr[idxAttr++] = atof(value.c_str());
                value = "";
            }
            else
            {
                value += inputLine[j];
            }

            j++;

        }

        string type = "";
        while( inputLine[j] != '\0' )
        {
            type += inputLine[j];
            j++;
        }

        flowers.push_back(iris(attr[0], attr[1], attr[2], attr[3], type));
    }
    fin.close();
}

void normalize(int column){ //normalize the given attribute column in the examples table flowers
    //find max
    double max = flowers[0].attributes[column];
    for(int i=0; i<flowers.size(); i++){
        if(flowers[i].attributes[column] > max)
            max = flowers[i].attributes[column];
    }
    //find min
    double min = flowers[0].attributes[column];
    for(int i=0; i<flowers.size(); i++){
        if(flowers[i].attributes[column] < min)
            min = flowers[i].attributes[column];
    }

    for(int i=0; i<flowers.size(); i++){
        double curr = flowers[i].attributes[column];
        flowers[i].attributes[column] = (curr - min)/(max - min);
    }
}

string kNN(){
  testIris = flowers[0]; //we will predict its class
  //sort the rest in ascending order according to the distance to testIris
  sort(flowers.begin() + 1, flowers.end());

  unordered_map<string, int> classCnt;
  int maxCnt = 0;
        // counting how many flowers of each type we have among the K nearest neighbours and which is the maximum count
        for( int j = 1; j < (K + 1); j++ )
        {
            string classType = flowers[j].classType;
            if( classCnt.find(classType) == classCnt.end() ) //ne sme imali tozi klas
                classCnt[classType] = 1;
            else
                classCnt[classType]++;

            if( classCnt[classType] > maxCnt )
                maxCnt = classCnt[classType];
        }
        vector<string> maxCntClasses; // contains all classes which have maximum count of flowers among the K nearest neighbours
        unordered_map<string, int>::iterator it = classCnt.begin();
        while( it != classCnt.end() )
        {
            if( it->second == maxCnt )
                maxCntClasses.push_back(it->first);
            it++;
        }
        //these are the classes with equal maxCnt votes among the K nearest neighbours
        //if it is only one class we return it
        //if it is more than one class we take the nearest neighbour
        if(maxCntClasses.size() == 1){
            return maxCntClasses[0];
        }else{ // size > 1
            return flowers[1].classType; //nearest neighbour to testIris
        }

}

int main(){
    
    readData();
    for(int col=0; col<4; col++)
        normalize(col);

    string prediction = kNN();
    cout << "The class of testIris is " << prediction << endl;

return 0;
}
