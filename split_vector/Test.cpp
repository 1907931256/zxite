// Test.cpp
// Demonstration and testing of split_vector class.

#include "stdafx.h"
#include "split_vector"

void make_upper(char &c) {
	if ((c >= 'a') && (c <= 'z'))
		c += 'A' - 'a';
}

void putwch(wchar_t c) {
	std::cout << static_cast<char>(c);
}

int main(int, char* []) {

	const char *hw = "Hello Moon!\n";
	{
		std::vector<char> v;
		std::copy(hw, hw + strlen(hw), std::back_inserter(v));
		for (size_t i=0;i<v.size(); i++) {
			std::cout << v[i];
		}
		std::for_each(v.begin(), v.end(), make_upper);
		std::reverse(v.begin(), v.end());
		std::copy(v.begin(), v.end(),
			std::ostream_iterator<char>(std::cout));
	}
	std::cout << std::endl;
	{
		split_vector<char> v;
		std::cout << "\nNarrow:" << v.max_size() << std::endl;
		std::copy(hw, hw + strlen(hw), std::back_inserter(v));
		//for (const char *s=hw; *s; *s++) {
		//	v.push_back(*s);
		//}
		for (size_t i=0;i<v.size(); i++) {
			std::cout << v[i];
		}
		{
			// Move the gap around to make sure it works OK.
			for (size_t c=0;c<v.size(); c++) {
				v.move_gap(c);
				for (split_vector<char>::iterator j=v.begin(); j!=v.end(); j++) {
					std::cout << *j;
				}
			}
			for (int d=v.size()-1;d>=0; d--) {
				v.move_gap(d);
				for (split_vector<char>::iterator j=v.begin(); j!=v.end(); j++) {
					std::cout << *j;
				}
			}
		}
		//split_vector<char> u(v);
		split_vector<char> u(v.begin(), v.end());
		std::for_each(u.begin(), u.end(), make_upper);
		for (split_vector<char>::iterator j=u.begin(); j!=u.end(); j++) {
			std::cout << *j;
		}
		std::reverse(u.begin(), u.end());
		u.insert(u.begin() + 5, '#');
		std::cout << "capacity:" << u.capacity() << std::endl;
		split_vector<char>(u).swap(u);
		std::cout << "capacity:" << u.capacity() << std::endl;
		std::copy(u.begin(), u.end(),
			std::ostream_iterator<char>(std::cout));
		u.push_front('$');
		std::copy(u.rbegin(), u.rend(),
			std::ostream_iterator<char>(std::cout));
		std::copy(u.rbegin(), u.rend(),
			std::ostream_iterator<char>(std::cout));
		u.clear();
		std::copy(u.rbegin(), u.rend(),
			std::ostream_iterator<char>(std::cout));
	}
	std::cout << "\nWide:\n";
	{
		const wchar_t *whw = L"Hello Moon!\n";
		split_vector<wchar_t> v;
		std::copy(whw, whw + wcslen(whw), std::back_inserter(v));
		v.erase(std::find(v.begin(), v.end(), L'M'));
		std::for_each(v.begin(), v.end(), putwch);
	}
	std::cout << "Ints:\n";
	{
		// Trigger the two iterator constructor to ensure it 
		// understands this is [n * a] form instead.
		split_vector<int> v(5,3);	
		v.push_front(1);
		(v.data_pointer())[1] = 9;	// Low level direct access
		std::copy(v.begin(), v.end(),
			std::ostream_iterator<int>(std::cout, ","));
		split_vector<int> u(v);	// Check copy constructor works
		std::copy(v.begin(), v.end(),
			std::ostream_iterator<int>(std::cout, "!"));
	}
	std::cout << "\nStrings:\n";
	{
		//std::vector<std::string> v;
		split_vector<std::string> v;
		v.push_back("alpha");
		v.push_back("beta");
		v.push_back("gamma");
		v.push_back("delta");
		v.resize(6);
		for (int i=v.size()-1; i>=0; i--) {
			std::cout << v.at(i) << "|";
		}
		std::sort(v.begin(), v.end());
		std::copy(v.begin(), v.end(),
			std::ostream_iterator<std::string>(std::cout, ","));
	}
	std::cout << "\nPress Enter to continue\n";
	char zzz[200];
	gets(zzz);
	return 0;
}
