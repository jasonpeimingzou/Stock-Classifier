/*
 * Overview of classifier.h:
 *
 * Main container class for data structures and functions used for stock outcome 
 * classification. Each feature used for prediction has a map and corresponding
 * categorization functor. Features can be added by adding an additional map and 
 * categorization functor to the private section. Classifier::initialize() and 
 * Classifier::classify() should initialize and add the feature for prediction
 * accordingly.
 */


#pragma once

#include <map>
#include <iostream>
#include "categorize.h"

using namespace std;

class Classifier {

	double total_stocks = 0;

	//Print summary of outcomes
	void print_outcomes(const string &filename);

	//Print summary of values for each key of a specified map
	void print_category(map<std::pair<string, string>, int> &feature);

	//Maps
	std::pair<string, string> make_pair_key(string outcome, string category);
	map<string, double> outcome_freq;

	//Conditional frequencies key = pair<outcome, category>
	map<std::pair<string, string>, int> roa_freq;
	map<std::pair<string, string>, int> roe_freq;
	map<std::pair<string, string>, int> nig_freq;
	map<std::pair<string, string>, int> de_freq;
	map<std::pair<string, string>, int> pe_freq;

	//Functors
	Categorize categorize_roa;
	Categorize categorize_roe;
	Categorize categorize_nig;
	Categorize categorize_de;
	Categorize categorize_pe;

	//Compute likelihood for classification
	double compute_label_likelihood(const string label, const string& roa_cat, 
		const string& roe_cat, const string& nig_cat, const string& de_cat, const string& pe_cat);

	//Computes p(category | outcome) and uses additive smoothing when necessary
	double smooth(map<std::pair<string, string>, int>& feature, std::pair<string, string> key, const string &label);

public:

	//Constructor
	Classifier()
		: categorize_roa(5.0, 10.0, 15.0, 20.0), categorize_roe(10.0, 20.0, 30.0, 40.0), 
		  categorize_nig(25.0, 50.0, 75.0, 100.0), categorize_de(0.25, 0.50, 0.75, 1.0), 
		  categorize_pe(12.0, 24.0, 36.0, 48.0) {}

	//Read training data and initialize private variables
	void initialize(string filename);

	//Classifier function
	void classify();

};

