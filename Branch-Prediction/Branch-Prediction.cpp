
// CSCI 620 - Dynamic Branchse ---- Refactor Code

#include <iostream>
#include <deque>
#include <string>
#include <vector>
#include <cmath>
#include <stdlib.h>

using namespace std;

class DynamicBranchPredictor {
public:
    DynamicBranchPredictor(int theM, int theN, int theNumOfBranch) {
        m = theM; // m num of history bit
        n = theN; // n-bits
        numOfBranch = theNumOfBranch;
        bufferSizeForEachBranch = pow(2, m);
        bufferSize = (bufferSizeForEachBranch * numOfBranch);


        // init to  0
        nbitCounter = 0;
    }

    void init(bool taken) {
        // init all statistic to 0
        for (int i = 0; i < numOfBranch; i++) {
            numOfTakenBranches.push_back(0);
            numOfNotTakenBranches.push_back(0);
            numOfPredictions.push_back(0);
            numOfMissPredictions.push_back(0);
        }
        // init the buffer with the given init value
        for (int i = 0; i < bufferSize; i++)
            buffer.push_back(taken);

        // init the historyBits with the given init value
        if (m > 0) {
            for (int i = 0; i < m; i++) {
                historyBits.push_back(taken);
            }
        }
        else
            historyBits.push_back(taken);
    }

    bool predict(bool actualTaken, int numOfBranch) {
        int numBranchIndex = numOfBranch - 1;
        int index = (bufferSizeForEachBranch * numOfBranch) - 1;
        if (m > 0) {
            index = (bufferSizeForEachBranch * (numOfBranch - 1)); // calculate index of each branch buffer index

            // covert historybit to binary and int to look up buffer
            string binary = "";
            for (int i = 0; i < m; i++) {
                if (historyBits[i])
                    binary += "1";
                else
                    binary += "0";
            }
            index += stoi(binary, 0, 2);
        }


        // check buffer
        bool predictTaken = buffer[index];

        //cout << "Branch " << numOfBranch << " predict to be " << predictTaken << endl;

        numOfPredictions[numBranchIndex]++;


        if (predictTaken)
            numOfTakenBranches[numBranchIndex]++;
        else
            numOfNotTakenBranches[numBranchIndex]++;

        int max = (pow(2, n) - 1) ;
        if (nbitCounter < max && actualTaken == true)
            nbitCounter++;
        else if(nbitCounter > 0 && actualTaken == false)
            nbitCounter--;
        if (predictTaken != actualTaken) { // miss prediction
            numOfMissPredictions[numBranchIndex]++;
            
            // flip
            if (nbitCounter >= (max / 2) + (max % 2 != 0)) { // division and round up
                buffer[index] = true;
            }
            else
                buffer[index] = false;
        }

        // add to history bit
        historyBits.push_back(actualTaken);

        // remove earliest history bit
        historyBits.pop_front();


        return predictTaken;
    }

    void printStat() {
        double accuracy;
        int totalNumOfTakenBranches = 0;
        int totalNumOfNotTakenBranches = 0;
        int totalNumOfPredictions = 0;
        int totalNumOfMissPredictions = 0;
        cout << "========== Statistic for (" << m << "," << n << ") ==========" << endl;
        for (int i = 0; i < numOfBranch; i++) {
            accuracy = 100 - (((double)numOfMissPredictions[i] / numOfPredictions[i]) * 100);
            cout << "=== Branch " << (i + 1) << " ===" << endl
                << "Number of Taken Branches: " << numOfTakenBranches[i] << endl
                << "Number of Not Taken Branches: " << numOfNotTakenBranches[i] << endl
                << "Number of Predictions: " << numOfPredictions[i] << endl
                << "Number of Missed Predictions: " << numOfMissPredictions[i] << endl
                << "Accuracy: " << accuracy << endl;

            totalNumOfTakenBranches += numOfTakenBranches[i];
            totalNumOfNotTakenBranches += numOfNotTakenBranches[i];
            totalNumOfPredictions += numOfPredictions[i];
            totalNumOfMissPredictions += numOfMissPredictions[i];
        }
        accuracy = 100 - (((double)totalNumOfMissPredictions / totalNumOfPredictions) * 100);
        cout << "========== Total ==========" << endl
            << "Number of Taken Branches: " << totalNumOfTakenBranches << endl
            << "Number of Not Taken Branches: " << totalNumOfNotTakenBranches << endl
            << "Number of Predictions: " << totalNumOfPredictions << endl
            << "Number of Missed Predictions: " << totalNumOfMissPredictions << endl
            << "Accuracy: " << accuracy << endl;
        cout << "===============================" << endl;
    }

    void printBuffer() {
        cout << "========== Buffer ==========" << endl;
        for (int i = 0; i < bufferSize; i++) {
            if (i == (bufferSizeForEachBranch - 1))
                cout << "Buffer " << ((i + 1) / bufferSizeForEachBranch) << endl;
            cout << buffer[i] << endl;
        }

    }

private:
    int m, n, bufferSize, numOfBranch, bufferSizeForEachBranch, nbitCounter;
    deque<bool> buffer, historyBits;

    deque<int> numOfTakenBranches, numOfNotTakenBranches, numOfPredictions, numOfMissPredictions;
};

//Swap function definition to swap two elements
void swap(int* x, int* y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}


//Function definition for quick sort iterative method
void quickSort(vector<int> arr, DynamicBranchPredictor& dynamicBranchPredictor)
{
    int lowest = 0;
    int highest = arr.size() - 1;

    // Create a new array as stack
    vector<int> stack;
    for (int i = 0; i < (highest - lowest + 1); i++)
        stack.push_back(0);

    // Initializing top of stack as -1
    int top = -1;

    //Pushing the values of highest and lowest on stack
    stack[++top] = lowest;
    stack[++top] = highest;



    // Keep popping from stack while is not empty
    while (top >= 0) { // branch 1

        dynamicBranchPredictor.predict(true, 1);

        //Popping the values of highest and lowest from stack 
        highest = stack[top--];
        lowest = stack[top--];

        int x = arr[highest];
        int i = (lowest - 1);

        //For loop & swap function to set the pivot element at its correct position in an array
        for (int j = lowest; j <= highest - 1; j++) { // branch 2
            dynamicBranchPredictor.predict(true, 2);
            if (arr[j] <= x) { // branch 3 
                i++;
                swap(&arr[i], &arr[j]);
                dynamicBranchPredictor.predict(true, 3);
            }
            dynamicBranchPredictor.predict(false, 3);
        }
        dynamicBranchPredictor.predict(false, 2);
        swap(&arr[i + 1], &arr[highest]);
        int pivot = (i + 1);


        //Pushing elements of left side of pivot to let side of stack
        for (int i = 0; pivot - 1 > lowest && i == 0; i++) { // branch 4
            dynamicBranchPredictor.predict(true, 4);
            stack[++top] = lowest;
            stack[++top] = pivot - 1;
        }
        dynamicBranchPredictor.predict(false, 4);

        //Pushing elements of right side of pivot to right side of stack
        for (int i = 0; pivot + 1 < highest && i == 0; i++) { // branch 5
            dynamicBranchPredictor.predict(true, 5);
            stack[++top] = pivot + 1;
            stack[++top] = highest;
        }
        dynamicBranchPredictor.predict(false, 5);
    }
    dynamicBranchPredictor.predict(false, 1);
}

// Function to print the array
void printArray(vector<int> arr)
{
    int n = arr.size();
    cout << "output array:";
    for (int i = 0; i < n; ++i)
        cout << arr[i] << " ";
    cout << endl;
}

int main()
{
    vector<int> arr = { 15,7,1,3,12,17,5,13,14,100,19,9,50,8,80,2,16,4,10,11,60,15,10,1};
    DynamicBranchPredictor dynamicBranchPredictor1(0, 2, 5); // (0,2,5) for 2-bit and (3,1,5) for (3,1) predictor scheme
    DynamicBranchPredictor dynamicBranchPredictor2(3, 1, 5);
    dynamicBranchPredictor1.init(false);
    dynamicBranchPredictor2.init(false);

    // input array
    vector<int> arr1;
    copy(arr.begin(), arr.end(), back_inserter(arr1));
    
    quickSort(arr1, dynamicBranchPredictor1);
    dynamicBranchPredictor1.printStat();
    printArray(arr1);

    // input array
    vector<int> arr2;
    copy(arr.begin(), arr.end(), back_inserter(arr2));

    quickSort(arr2, dynamicBranchPredictor2);
    dynamicBranchPredictor2.printStat();
    printArray(arr2);
   
}