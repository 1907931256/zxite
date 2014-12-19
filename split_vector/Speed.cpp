// Speed.cpp : Defines the entry point for the console application.
// Performence test of split_vector class against vector class.
//

#include "stdafx.h"
#include <time.h>
#include "split_vector"

void make_upper(char &c) {
	if ((c >= 'a') && (c <= 'z'))
		c += 'A' - 'a';
}

void putwch(wchar_t c) {
	std::cout << static_cast<char>(c);
}

const int copies=30;	// Make file large by repeating it this many times
const int repetez=50;
const char *s_std = "std::";
const char *s_nonstd = "nonstd::";

int main(int, char* []) {
	
	char contents[100000];
	FILE *fp = fopen("Test.cpp", "rt");
	int n = fread(contents, 1, sizeof(contents), fp);
	std::cout << "Size is " << n << std::endl;
	fclose(fp);

	const size_t len_std = strlen(s_std);
	const char *end_std = s_std + len_std;
	const size_t len_nonstd = strlen(s_nonstd);
	const char *end_nonstd = s_nonstd + len_nonstd;
	{
		// First time the standard vector
		time_t t0 = time(0);
		std::vector<char> v(contents, contents + n);
		for (size_t copyNumber=1; copyNumber<copies; copyNumber++) {
			v.insert(v.end(), contents, contents + n);
		}
		for (int r=0; r<repetez; r++) {
			// Globally substitute "std::" with "nonstd::".
			{
				std::vector<char>::iterator p = v.begin();
				p = std::search(p, v.end(), s_std, end_std);
				while (p != v.end()) {
					const size_t position = p - v.begin();
					v.erase(p, p + len_std);
					v.insert(p, s_nonstd, end_nonstd);
					p = v.begin() + position + len_nonstd;
					p = std::search(p, v.end(), s_std, end_std);
				}
					
				//std::copy(v.begin(), v.end(),
				//	std::ostream_iterator<char>(std::cout));
			}

			// And change "nonstd::" back to "std::".
			{
				std::vector<char>::iterator p = v.begin();
				p = std::search(p, v.end(), s_nonstd, end_nonstd);
				while (p != v.end()) {
					const size_t position = p - v.begin();
					v.erase(p, p + len_nonstd);
					v.insert(p, s_std, end_std);
					p = v.begin() + position + len_std;
					p = std::search(p, v.end(), s_nonstd, end_nonstd);
				}
					
				//std::copy(v.begin(), v.end(),
				//	std::ostream_iterator<char>(std::cout));
			}
		}
		time_t t1 = time(0);
		std::cout << "Time for vector is " << (t1-t0) << std::endl;
	}
	{
		// Now the split vector
		time_t t0 = time(0);
		split_vector<char> v(contents, contents + n);
		for (size_t copyNumber=1; copyNumber<copies; copyNumber++) {
			v.insert(v.end(), contents, contents + n);
		}
		for (int r=0; r<repetez; r++) {
			// Globally substitute "std::" with "nonstd::".
			{
				split_vector<char>::iterator p = v.begin();
				p = std::search(p, v.end(), s_std, end_std);
				while (p != v.end()) {
					const size_t position = p - v.begin();
					v.erase(p, p + len_std);
					v.insert(p, s_nonstd, end_nonstd);
					p = v.begin() + position + len_nonstd;
					p = std::search(p, v.end(), s_std, end_std);
				}
					
				//std::copy(v.begin(), v.end(),
				//	std::ostream_iterator<char>(std::cout));
			}

			// And change "nonstd::" back to "std::".
			{
				split_vector<char>::iterator p = v.begin();
				p = std::search(p, v.end(), s_nonstd, end_nonstd);
				while (p != v.end()) {
					const size_t position = p - v.begin();
					v.erase(p, p + len_nonstd);
					v.insert(p, s_std, end_std);
					p = v.begin() + position + len_std;
					p = std::search(p, v.end(), s_nonstd, end_nonstd);
				}
					
				//std::copy(v.begin(), v.end(),
				//	std::ostream_iterator<char>(std::cout));
			}
		}
		time_t t1 = time(0);
		std::cout << "Time for split_vector is " << (t1-t0) << std::endl;
	}
}
