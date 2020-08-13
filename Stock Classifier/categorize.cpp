#pragma once

#include "categorize.h"
#include <string>

using namespace std;

string Categorize::operator()(const string target) {
		
		if (target == "X") {
			return NA;
		}

		double conv_target = stod(target);

		if (conv_target <= negative_threshold) {
			return NEGATIVE;
		}
		if (conv_target < first_threshold) {
			return FIRST;
		}
		if (conv_target < second_threshold) {
			return SECOND;
		}
		if (conv_target < third_threshold) {
			return THIRD;
		}
		if (conv_target < fourth_threshold) {
			return FOURTH;
		}
		else {
			return HIGH;
		}
}