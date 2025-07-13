#include <iostream>
#include <ctime>
#include <vector>
using namespace std;

int main() {
    srand(time(0));

    double selectionProb = static_cast<double>(rand()) / RAND_MAX;
    cout<<"Selection: "<<selectionProb<<endl;
}