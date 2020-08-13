/*
 * Overview of categorize.h:
 *
 * Generic functor to create categorical labels for quantitative data based on thresholds
 * established during class instantiation. Thresholds for various features used in stock
 * are divided into 7 categories using the "binning" technique. The first 6 bins reflect
 * progressively increasing bin ranges. The "negative" and "high" bins should capture metrics
 * below zero or excessively high. The remaining range should be divided into 4 bins using 
 * the equidistant binning method. Ideally, the sector average is the upper bound for the 
 * second bin if possible, but this is not a requirement. A 7th bin is used to capture 
 * cases where a metric cannot be defined, denoted by a capital "X" in the training data.
 */

#pragma once
#include <iostream>

using namespace std;

class Categorize {

	//Thresholds represent upper bounds
	const double negative_threshold = 0.0;
	const double first_threshold;
	const double second_threshold;
	const double third_threshold;
	const double fourth_threshold;

	//String keywords for categories
	const string NEGATIVE = "Negative";
	const string FIRST = "First Bin";
	const string SECOND = "Second Bin";
	const string THIRD = "Third Bin";
	const string FOURTH = "Fourth Bin";
	const string HIGH = "High";
	const string NA = "N/A";

public:

	//Constructor
	Categorize(double first_in, double second_in, double third_in, double fourth_in)
		:first_threshold(first_in), second_threshold(second_in),
		third_threshold(third_in), fourth_threshold(fourth_in) {}

	//Function call operator
	string operator()(const string target);

};