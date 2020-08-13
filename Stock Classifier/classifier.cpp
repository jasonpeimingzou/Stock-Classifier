#pragma once
#include "classifier.h"
#include "csvstream.h"
#include "math.h"


/*****************************************************************************************
									Public Functions
*****************************************************************************************/

void Classifier::initialize(string filename) {
	csvstream csvin(filename);
	map<string, string> row;

	int file_rows = 1;

	cout << "Summary of " << filename << " training dataset: " << endl;

	while (csvin >> row) {

		//Print Test Data
		cout << "  "<< file_rows++ << ": ";
		cout << row["Ticker"] << " " << row["Outcome"] << " " << row["%ROA"]
			<< " " << row["%ROE"] << " " << row["%NIG"] << " " << row["D/E"]
			<< " " << row["P/E"] << endl;

		//Initialize private variables
		total_stocks++;
		outcome_freq[row["Outcome"]]++;

		//Make and map ROA key
		std::pair<string, string> roa_key = make_pair_key(row["Outcome"],
			categorize_roa(row["%ROA"]));
		roa_freq[roa_key]++;
		
		//Make and map ROE key
		std::pair<string, string> roe_key = make_pair_key(row["Outcome"],
			categorize_roe(row["%ROE"]));
		roe_freq[roe_key]++;

		//Make and map NIG key
		std::pair<string, string> nig_key = make_pair_key(row["Outcome"],
			categorize_nig(row["%NIG"]));
		nig_freq[nig_key]++;

		//Make and map D/E key
		std::pair<string, string> de_key = make_pair_key(row["Outcome"],
			categorize_de(row["D/E"]));
		de_freq[de_key]++;

		//Make and map P/E key
		std::pair<string, string> pe_key = make_pair_key(row["Outcome"],
			categorize_pe(row["P/E"]));
		pe_freq[pe_key]++;
	}

	//Printing
	cout << endl << "Outcomes:" << endl;
	print_outcomes(filename);
	cout << endl << "ROA mapping:" << endl;
	print_category(roa_freq);
	cout << endl << "ROE mapping:" << endl;
	print_category(roe_freq);
	cout << endl << "NIG mapping:" << endl;
	print_category(nig_freq);
	cout << endl << "D/E mapping:" << endl;
	print_category(de_freq);
	cout << endl << "P/E mapping:" << endl;
	print_category(pe_freq);

}

void Classifier::classify() {
	
	//Parameters
	string roa_in;
	string roe_in;
	string nig_in;
	string de_in;
	string pe_in;

	cout << endl << "**************************************************************************************" << endl;
	cout << endl << "Enter stock metrics in the form: ROA ROI %NIG D/E P/E" << endl;
	cout << "Ex: 5 10 15 0.5 30" << endl << endl << "Predict for metrics: ";
	cin >> roa_in >> roe_in >> nig_in >> de_in >> pe_in;

	cout << endl << "Classifying a stock with: " << endl <<
		"  ROA: " << roa_in << endl <<
		"  ROE: " << roe_in << endl <<
		"  %NIG: " << nig_in << endl <<
		"  D/E: " << de_in << endl << 
		"  P/E: " << pe_in << endl;

	//Categories
	string roa_cat = categorize_roa(roa_in);
	string roe_cat = categorize_roe(roe_in);
	string nig_cat = categorize_nig(nig_in);
	string de_cat = categorize_de(de_in);
	string pe_cat = categorize_pe(pe_in);

	cout << endl << "Categorization summary: " << endl;
	cout << "  ROA: " << roa_cat << endl;
	cout << "  ROE: " << roe_cat << endl;
	cout << "  %NIG: " << nig_cat << endl;
	cout << "  D/E: " << de_cat << endl;
	cout << "  P/E: " << pe_cat << endl;

	//Classifier
	double gain_likelihood = compute_label_likelihood("Gain", roa_cat, roe_cat, nig_cat, de_cat, pe_cat);
	double nl_likelihood = compute_label_likelihood("N/L", roa_cat, roe_cat, nig_cat, de_cat, pe_cat);

	cout << endl << "ln p(Gain) = " << gain_likelihood << " vs ln p(N/L) = " << nl_likelihood << endl;
	cout << endl << "Prediction: ";

	if (gain_likelihood > nl_likelihood) {
		cout << "Gain" << endl;
	}
	else {
		cout << "Neutral/Loss" << endl;
	}

	
}

/*****************************************************************************************
								   Private Functions
*****************************************************************************************/

std::pair<string, string> Classifier::make_pair_key(string outcome, string category) {
	std::pair<string, string> return_pair;
	return_pair.first = outcome;
	return_pair.second = category;

	return return_pair;
}

void Classifier::print_outcomes(const string& filename) {

	cout << "  " << outcome_freq["Gain"] << " gains" << endl;
	cout << "  " << outcome_freq["N/L"] << " neutrals/losses" << endl;

}

void Classifier::print_category(map<std::pair<string, string>, int>& feature) {

	//Iterate through labels
	const string labels[] = { "Gain", "N/L" };
	const string categories[] = { "Negative", "First Bin", "Second Bin", 
		"Third Bin" , "Fourth Bin", "High", "N/A" };

	for (auto i : labels) {
		for (auto j : categories) {
			std::pair<string, string> key;
			key.first = i;
			key.second = j;

			cout << "  Key: <" << key.first << ", " << key.second << ">"
				<< " Frequency: " << feature[key] << endl;

		}
	}
}

double Classifier::compute_label_likelihood(const string label, const string& roa_cat,
	const string& roe_cat, const string& nig_cat, const string& de_cat, const string& pe_cat) {

	//Make keys
	std::pair<string, string> roa_key = make_pair_key(label, roa_cat);
	std::pair<string, string> roe_key = make_pair_key(label, roe_cat);
	std::pair<string, string> nig_key = make_pair_key(label, nig_cat);
	std::pair<string, string> de_key = make_pair_key(label, de_cat);
	std::pair<string, string> pe_key = make_pair_key(label, pe_cat);

	double likelihood = log(outcome_freq[label] / total_stocks)
		+ log(smooth(roa_freq, roa_key, label))
		+ log(smooth(roe_freq, roe_key, label))
		+ log(smooth(nig_freq, nig_key, label))
		+ log(smooth(de_freq, de_key, label))
		+ log(smooth(pe_freq, pe_key, label));

	return likelihood;
}

double Classifier::smooth(map<std::pair<string, string>, int>& feature, std::pair<string, string> key, const string &label) {

	if (feature[key] == 0) {
		return 1.0 / total_stocks;
	}
	else {
		return feature[key] / outcome_freq[label];
	}

}