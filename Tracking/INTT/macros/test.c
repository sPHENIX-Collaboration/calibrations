#include "InttDeadMapHelper.h"

int main()
{
	struct InttDeadMap_Param_s params;

	InttDeadMap_Long_t N = 154614;
	struct InttDeadMap_Point_s* list = new struct InttDeadMap_Point_s[N];

	init_params(&params, list, N, 2.0, 2.0);
	gen_points(&params);

	count_type_i(&params);
	check_type_i(&params);

	delete[] list;

	return EXIT_SUCCESS;
}
