
#include "dataformatters.h"
#include <cctype>
#include <sstream>

namespace QuantLib {
	
	std::string IntegerFormatter::toString(int i, int digits) {
		std::ostringstream out;
		if (!IsNull(digits)) {
			out.width(digits);
			out.fill('0');
		}
		out << i;
		return out.str();
	}

	std::string DoubleFormatter::toString(double x, int precision, int digits) {
		std::ostringstream out;
		if (!IsNull(precision)) {
			out.setf(std::ios_base::fixed);
			out.precision(precision);
		}
		if (!IsNull(digits)) {
			out.width(digits);
			out.fill(' ');
		}
		out << x;
		return out.str();
	}

	std::string EuroFormatter::toString(double amount) {
		std::string output;
		if (amount < 0.0) {
			output = "-";
			amount = -amount;
		} else {
			output = " ";
		}
		int triples = 0;
		while (amount >= 1000.0) {
			amount /= 1000;
			triples++;
		}
		output += IntegerFormatter::toString(int(amount));
		amount -= int(amount);
		while (triples > 0) {
			amount *= 1000;
			output += ","+IntegerFormatter::toString(int(amount),3);
			amount -= int(amount);
			triples--;
		}
		amount *= 100;
		output += "."+IntegerFormatter::toString(int(amount+0.5),2);
		return output;
	}

	std::string RateFormatter::toString(double rate, int precision) {
		return DoubleFormatter::toString(rate*100,precision)+"%";
	}

	std::string DateFormatter::toString(const Date& d, bool shortFormat) {
		static const std::string monthName[] = {
			"January", "February", "March", "April", "May", "June",
			"July", "August", "September", "October", "November", "December" };
		std::string output;
		int dd = d.dayOfMonth(), mm = int(d.month()), yyyy = d.year();
		if (shortFormat) {
			output = (mm < 10 ? "0" : "") + IntegerFormatter::toString(mm);
			output += (dd < 10 ? "/0" : "/") + IntegerFormatter::toString(dd);
			output += "/" + IntegerFormatter::toString(yyyy);
		} else {
			output = monthName[mm] + " ";
			output += IntegerFormatter::toString(dd);
			switch (dd) {
			  case 1,21,31:
				output += "st, ";
				break;
			  case 2,22:
				output += "nd, ";
				break;
			  case 3,23:
				output += "rd, ";
				break;
			  default:
				output += "th, ";
			}
			output += IntegerFormatter::toString(yyyy);
		}
		return output;
	}

	std::string StringFormatter::toLowercase(const std::string& s) {
		std::string output = s;
		for (std::string::iterator i=output.begin(); i!=output.end(); i++)
			*i = QL_TOLOWER(*i);
		return output;
	}

	std::string StringFormatter::toUppercase(const std::string& s) {
		std::string output = s;
		for (std::string::iterator i=output.begin(); i!=output.end(); i++)
			*i = QL_TOUPPER(*i);
		return output;
	}

}
