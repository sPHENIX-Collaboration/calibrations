#include "InttDeadMapHelper.c"

void InttDeadMap()
{
	struct InttDeadMap_Param_s params;

	InttDeadMap_Long_t N = 31310;
	InttDeadMap_Double_t frac = 0.2;

        struct InttDeadMap_Point_s* list = new struct InttDeadMap_Point_s[N];

        init_params(&params, list, N, 2.0, 2.0);
        gen_points(&params, frac);
	//quicksort_hitkey(&params);
	quicksort_counts(params.points, 0, N);

	std::cout << "smallest: " << params.points[0].counts << std::endl;
	std::cout << "largest: " << params.points[params.N - 1].counts << std::endl;

        check_type_i(&params);

	InttDeadMap_Double_t i_lower_ref = ceil(frac * N);
	InttDeadMap_Double_t i_upper_ref = N - ceil(frac * N);

        std::cout << "\ti_lower: " << params.i_lower << "\t(" << i_lower_ref << ")\t\trel err: " << (params.i_lower - i_lower_ref) / i_lower_ref * 100.0 << "\%" << std::endl;
        std::cout << "\ti_upper: " << params.i_upper << "\t(" << i_upper_ref << ")\trel err: " << (params.i_upper - i_upper_ref) / i_lower_ref * 100 << "\%" << std::endl;

        delete[] list;
}
