#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <vector>
#include <string>
using namespace std;

struct iris
{
	double attributes[4];

	iris(double sL = 0.0, double sW = 0.0, double pL = 0.0, double pW = 0.0)
	{
		attributes[0] = sL;
		attributes[1] = sW;
		attributes[2] = pL;
		attributes[3] = pW;
	}

	iris& operator=(const iris& other) {
		if (this != &other) {
			for (int i = 0; i < 4; i++)
				attributes[i] = other.attributes[i];
		}

		return *this;
	}

	friend ostream& operator<<(ostream& os, const iris& ir)
	{
		os << ir.attributes[0] << " " << ir.attributes[1] << " " << ir.attributes[2] << " " << ir.attributes[3] << endl;
		return os;
	}

};

struct cluster {
	vector<iris> data;
	iris centroid;
};

const int MAX_ITER = 1000;
const int K = 3;  //number of clusters
vector<iris> flowers;

double euclidDistance(const iris& i1, const iris& i2) {
	double sum = 0.0;
	for (int i = 0; i < 4; i++)
		sum += pow((i2.attributes[i] - i1.attributes[i]), 2.0);

	return sum;
}

void readData()
{
	ifstream fin;
	fin.open("iris.txt");

	char inputLine[100];
	while (fin.getline(inputLine, 100))
	{
		double attr[4];
		int idxAttr = 0;
		string value = "";
		int j = 0;
		while (!isalpha(inputLine[j]))
		{
			if (inputLine[j] == ',')
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

        flowers.push_back(iris(attr[0], attr[1], attr[2], attr[3]));
	}
	fin.close();
}

void normalize(int column) { //normalize the given attribute column in the examples table flowers
	//find max
	double max = flowers[0].attributes[column];
	for (int i = 0; i<flowers.size(); i++) {
		if (flowers[i].attributes[column] > max)
			max = flowers[i].attributes[column];
	}
	//find min
	double min = flowers[0].attributes[column];
	for (int i = 0; i<flowers.size(); i++) {
		if (flowers[i].attributes[column] < min)
			min = flowers[i].attributes[column];
	}

	for (int i = 0; i<flowers.size(); i++) {
		double curr = flowers[i].attributes[column];
		flowers[i].attributes[column] = (curr - min) / (max - min);
	}
}

void kMeans() {
	int sizeDataSet = flowers.size();
	vector<cluster> clusters;  //we will have K clusters

	//Initialization
	for (int i = 0; i < K; i++) {
		int centroidNr = rand() % sizeDataSet;
		cluster cl;
		cl.centroid = flowers[centroidNr];
		clusters.push_back(cl);

	}
	// We have K clusters with random centroids
	for (int i = 0; i < MAX_ITER; i++) {

		//assign all examples in the dataset to clusters
		for (int j = 0; j < sizeDataSet; j++) {
			double min = euclidDistance(flowers[j], clusters[0].centroid);
			int nearestCluster = 0;

			for (int c = 1; c < K; c++) {
				double curr = euclidDistance(flowers[j], clusters[c].centroid);
				if (curr < min) {
					min = curr;
					nearestCluster = c;
				}
			}
			//add example to the found cluster
			clusters[nearestCluster].data.push_back(flowers[j]);
		}

		//update cluster centroids
		for (int i = 0; i < K; i++) {
			int sizeCluster = clusters[i].data.size();
			for (int col = 0; col < 4; col++) {
				double sum = 0.0;
				for (int j = 0; j < sizeCluster; j++)
					sum += clusters[i].data[j].attributes[col];

				clusters[i].centroid.attributes[col] = sum / sizeCluster;
			}
			//we have a new i-th centroid => we will form a new cluster around it
			clusters[i].data.clear();
		}
	}

	//after MAX_ITER iterations print the final centroids
	for (int i = 0; i < K; i++) {
		cout << clusters[i].centroid;
	}

}

int main() {

	readData();
	for (int col = 0; col<4; col++)
		normalize(col);

	kMeans();

	return 0;
}