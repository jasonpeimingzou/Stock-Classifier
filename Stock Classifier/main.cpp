/*
 * Naive Bayes Classifier for technology sector stocks.
 * Author: Jason Zou
 *
 * Overview:
 *
 * This command line program uses generative machine learning to predict
 * the outcome of stock prices after one year based off metrics from the
 * preceding year. This program relies on the Naive Bayes Classifier 
 * Model.
 *
 * Training data should be added to trainingdata.csv, following the 
 * column headers accordingly. The outcome should reflect the result
 * in the 1 year period following when the metrics are reported. For
 * instance, average metrics for 2018 are entered along with the 
 * outcome in 2019. The outcome is only a "Gain" when throughout the
 * entire following year, a clear upward trend is visible along with
 * a higher ending price compared to the starting price. Stocks that
 * clearly lose value or those that do not have a definitive upward
 * trend should be given the "N/L" label.
 * 
 * Data for historical stock metrics was obtained from macrotrends.net,
 * while data for historical sector performance was obtained from
 * csimarket.com. A list of all technology sector stocks used in the 
 * training dataset can be found on finviz.com and applying the 
 * "technology" sector filter.
 */

#include "classifier.h";


int main() {
	
	Classifier test;
	test.initialize("trainingdata.csv");

    cout << endl << "Menu choices:" << endl;
    cout << "  1: Predict Stock Outcome" << endl << "  2: Exit" << endl;
    cout << "Enter a choice: ";
    int choice;
    cin >> choice;
    while (choice != 2) {
        if (choice == 1) {
            test.classify();
        }
        else {
            cout << "Invalid choice" << endl;
        }
        cout << endl << "Menu choices:" << endl;
        cout << "  1: Predict Stock Outcome" << endl << "  2: Exit" << endl;
        cout << "Enter a choice: ";
        cin >> choice;
    }

    cout << endl << "Program ended." << endl;
	
	
}