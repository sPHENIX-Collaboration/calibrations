R__LOAD_LIBRARY(libintt.so)
#include <intt/InttDeadMapHelper.h>

#include <iostream>

void InttDeadMap()
{
	InttDeadMapHelper::InttDeadMap_Long_t N = 156117;
	InttDeadMapHelper::InttDeadMap_Double_t frac = 0.01;

	InttDeadMapHelper dmh(N, 2.0, 2.0);

	dmh.gen_points(frac);

	dmh.quicksort_counts(0, N);
	dmh.check_type_i();

	InttDeadMapHelper::InttDeadMap_Double_t i_lower_ref = ceil(frac * N);
	InttDeadMapHelper::InttDeadMap_Double_t i_upper_ref = N - ceil(frac * N);

	std::cout << "\ti_lower: " << dmh.i_lower << "\t(" << i_lower_ref << ")\t";
	std::cout << "\trel err: " << (dmh.i_lower - i_lower_ref) / i_lower_ref * 100.0 << "\%" << std::endl;
	std::cout << "\ti_upper: " << dmh.i_upper << "\t(" << i_upper_ref << ")\t";
	std::cout << "\trel err: " << (dmh.i_upper - i_upper_ref) / i_lower_ref * 100 << "\%" << std::endl;
}
