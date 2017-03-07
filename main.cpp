#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <math.h>

using namespace std;

void printCentroid(vector<float> vec);

float Euclidian(vector<float> &first, vector<float> &second ){
    return sqrtf(powf((second[0]-first[0]),2) + powf((second[1]-first[1]),2) + powf((second[2]-first[2]),2));
}
void AddVectors(vector<float> &point, vector<float> &centroid){
    for (int i = 0; i<3; i++ ){
        centroid[i]+=point[i];
    }
}
// returns a vector of centroid vectors, the first three values in each centroid vector
// being the coordinates, the fourth being the cluster size
vector<vector<float>> RecalculateCentroids(vector<float> &c1, vector<float>&c2, vector<float>&c3,
                          vector<vector<float>> &data ){
    vector<vector<float>> statistics;
    int c1_pop, c2_pop, c3_pop = 0;
    c1 = c2 = c3 = {0,0,0};
    for(int i = 0; i < data.size(); i++){
        if(data[i][3] == 1.0){
            AddVectors(data[i], c1);
            c1_pop++; }
        else if (data[i][3] == 2.0){
            AddVectors(data[i], c2);
            c2_pop++; }
        else {AddVectors(data[i], c3);
            c3_pop++; }
        }
    for(int j = 0; j<3; j++){
        c1[j]/= c1_pop;
        c2[j]/= c2_pop;
        c3[j]/= c3_pop;
    }
    c1.push_back((float &&) c1_pop);
    c2.push_back((float &&) c2_pop);
    c3.push_back((float &&) c3_pop);
    statistics.push_back(c1);
    statistics.push_back(c2);
    statistics.push_back(c3);
    return statistics;
}
bool CentroidsStopMoving(vector<float> c1, vector<float> c2, vector<float> c3, vector<vector<float>> &prevTroids){
    vector<vector<float>>newTroids = {c1,c2,c3};
    for (int i = 0; i<3; i++){
        if (prevTroids[i] != newTroids[i])
            return false;
    }
    return true;
}
void printCentroid(vector<float> vec){
    for (int i = 0; i<3; i++){
        cout << " " << vec[i] << " ";
    }
}
int main(int argc, char *argv[]) {

    // Extract parameters
    ifstream in("seed_dataset.txt");
    if(!in){
        cout << "File error." << endl;
        return 1;
    }
    // Create and initialize data structures
    string buf;
    vector<vector<float>> dataset;
    while(getline(in, buf)){
        string temp;
        stringstream ss(buf);
        vector<float> tokens;
        while (ss >> temp) {
            tokens.push_back(stof(temp));
        }
        vector<float>temp_final;
        // Add the third, fourth and fifth space-delimited values from each
        // line of the input file after splitting and casting into a temp vector:
        temp_final.push_back(tokens[2]);
        temp_final.push_back(tokens[3]);
        temp_final.push_back(tokens[4]);
        //populate 4th value in vector as membership ID for centroids later:
        temp_final.push_back(0.0);
        dataset.push_back(temp_final);
    }
    srand(time(NULL));
    vector<float>centroid1 = dataset[(rand() % dataset.size())];
    vector<float>centroid2 = dataset[(rand() % dataset.size())];
    vector<float>centroid3 = dataset[(rand() % dataset.size())];
    int iteration = 0;
    vector<vector<float>> last_iter;
    while(iteration < 1000) {
        for (int i = 0; i < dataset.size(); i++) {
            float first = Euclidian(dataset[i], centroid1);
            float second = Euclidian(dataset[i], centroid2);
            float third = Euclidian(dataset[i], centroid3);
            if (first < second) {
                dataset[i][3] = 1.0;
            } else { dataset[i][3] = 2.0; }
            if (third < min(first,second))
                dataset[i][3] = 3.0;
        }
        vector<vector<float>> prevcentroids;
        if(iteration >0) {
            prevcentroids.push_back(last_iter[0]);
            prevcentroids.push_back(last_iter[1]);
            prevcentroids.push_back(last_iter[2]);
        }
        last_iter = RecalculateCentroids(centroid1, centroid2, centroid3, dataset);
        iteration++;
        if((iteration > 1) && CentroidsStopMoving(last_iter[0], last_iter[1], last_iter[2], prevcentroids))
            break;
    }
    cout << "\n Last centroid and population values from iteration " << iteration << " :" << endl;
    cout << "Populations: C1: " << last_iter[0][3] << " C2: " << last_iter[1][3] << " C3: " << last_iter[2][3] << endl;
    cout << "Coordintes: \n C1: ";
    printCentroid(last_iter[0]);
    cout << "\n C2: ";
    printCentroid(last_iter[1]);
    cout << "\n C3: ";
    printCentroid(last_iter[2]);
    return 0;
}