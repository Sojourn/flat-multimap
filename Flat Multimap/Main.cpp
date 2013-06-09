#include <iostream>
#include <vector>
#include <functional>
#include <cstdint>
#include <cassert>
#include "Table.h"

struct Test_t
{
	const char *name;
	std::function<bool (void)> run;
};

void prepareTests(std::vector<Test_t> &tests);

int main(int argc, char **argv)
{
	std::vector<Test_t> tests;
	prepareTests(tests);

	size_t passed = 0;
	std::for_each(std::begin(tests), std::end(tests), [&](const Test_t &test)
	{
		bool result = test.run();

		std::cout << test.name << " : " << (result ? "passed" : "failed") << std::endl;
	});

	std::cout << passed << " of " << tests.size() << " passed" << std::endl;

	std::system("pause");
	return 0;
}

void prepareTests(std::vector<Test_t> &tests)
{
	auto addTest = [&](const char *name, std::function<bool (void)> run)
	{
		Test_t test = {name, run};
		tests.push_back(test);
	};

	addTest("True", []() -> bool
	{
		return true;
	});

	addTest("False", []() -> bool
	{
		return false;
	});
}