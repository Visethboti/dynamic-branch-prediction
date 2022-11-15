// CSCI 620 - Dynamic Branchse
//

#include <iostream>
#include <deque>
#include <string>

using namespace std;

class DynamicBranchPredictor {
    public:
        DynamicBranchPredictor(int theM, int theN, int theNumOfBranch) {
            m = theM; // m num of history bit
            n = theN; // n-bits
            numOfBranch = theNumOfBranch;
            bufferSizeForEachBranch = pow(2, m);
            bufferSize = (bufferSizeForEachBranch*numOfBranch);
            

            // init all statistic to 0
            numOfTakenBranches = 0;
            numOfNotTakenBranches = 0;
            numOfPredictions = 0;
            numOfMissPredictions = 0;
            
            nbitCounter = 0;
        }

        void init(bool taken) {
            // init the buffer with the given init value
            for (int i = 0; i < bufferSize; i++)
                buffer.push_back(taken);

            // init the historyBits with the given init value
            for (int i = 0; i < m; i++) {
                historyBits.push_back(taken);
                allHistoryBits.push_back(taken);
            }
        }

        bool predict(bool actualTaken, int numOfBranch) {
            int index = (numOfBranch-1) * 2;
            if (m > 0) {
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

            cout << "Branch predict to be " << predictTaken << endl;

            numOfPredictions++;

            if (predictTaken)
                numOfTakenBranches++;
            else
                numOfNotTakenBranches++;

            int max = (pow(2, n) - 1);
            if (nbitCounter < max && nbitCounter < 0)
                if (actualTaken == true)
                    nbitCounter++;
                else
                    nbitCounter--;

            if (predictTaken == actualTaken) { // correct prediction
                
            } 
            else { // miss prediction
                numOfMissPredictions++;
                
                // flip
                if(nbitCounter >= (max/2))
                    buffer[index] = true;
                else
                    buffer[index] = false;
            }

            // add to history bit
            historyBits.push_back(actualTaken);
            allHistoryBits.push_back(actualTaken);

            historyBits.pop_front();

            return predictTaken;
        }

        void printStat() {
            double accuracy = 100-(((double)numOfMissPredictions / numOfPredictions)*100);
            cout << "========== Statistic ==========" << endl 
                << "Number of Taken Branches: " << numOfTakenBranches << endl
                << "Number of Not Taken Branches: " << numOfNotTakenBranches << endl
                << "Number of Predictions: " << numOfPredictions << endl
                << "Number of Missed Predictions: " << numOfMissPredictions << endl
                << "Accuracy: " << accuracy << endl
                << "===============================" << endl;
        }
        
        void printBuffer() {
            cout << "========== Buffer ==========" << endl;
            for (int i = 0; i < bufferSize; i++)
                cout << buffer[i] << endl;
        }

    private:
        int m, n, bufferSize, numOfBranch, bufferSizeForEachBranch, nbitCounter;
        deque<bool> buffer, historyBits, allHistoryBits;

        int numOfTakenBranches, numOfNotTakenBranches, numOfPredictions, numOfMissPredictions;
};

int main()
{
    DynamicBranchPredictor DynamicBranchPredictor(3,1,1);
    DynamicBranchPredictor.init(false);
    DynamicBranchPredictor.printBuffer();
    /*
    DynamicBranchPredictor.predict(false, 1);
    DynamicBranchPredictor.predict(false, 1);
    DynamicBranchPredictor.predict(false, 1);
    DynamicBranchPredictor.predict(true, 1);
    DynamicBranchPredictor.printBuffer();

    DynamicBranchPredictor.predict(false, 1);
    DynamicBranchPredictor.predict(false, 1);
    DynamicBranchPredictor.predict(false, 1);
    DynamicBranchPredictor.predict(true, 1);
    DynamicBranchPredictor.printBuffer();
    */

    DynamicBranchPredictor.predict(false, 1);
    DynamicBranchPredictor.predict(false, 1);
    DynamicBranchPredictor.predict(false, 1);
    DynamicBranchPredictor.predict(true, 1);
    DynamicBranchPredictor.printBuffer();

    DynamicBranchPredictor.predict(false, 1);
    DynamicBranchPredictor.predict(false, 1);
    DynamicBranchPredictor.predict(false, 1);
    DynamicBranchPredictor.predict(true, 1);
    DynamicBranchPredictor.printBuffer();

    DynamicBranchPredictor.predict(false, 1);
    DynamicBranchPredictor.predict(false, 1);
    DynamicBranchPredictor.predict(false, 1);
    DynamicBranchPredictor.predict(true, 1);
    DynamicBranchPredictor.printBuffer();

    DynamicBranchPredictor.printStat();
}