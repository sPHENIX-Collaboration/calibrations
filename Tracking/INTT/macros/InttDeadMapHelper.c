#include "InttDeadMapHelper.h"

void quicksort_hitkey(struct InttDeadMap_Point_s* points, InttDeadMap_Long_t i_lower, InttDeadMap_Long_t i_upper)
{
	if(i_lower < 0)return;
	if(i_upper < 1)return;
	if(i_upper <= i_lower)return;

	InttDeadMap_Long_t i_pivot = i_lower - 1;

	{
		InttDeadMap_Long_t i;

		struct InttDeadMap_Point_s pivot = points[i_upper - 1];
		struct InttDeadMap_Point_s temp;

		for(i = i_lower; i < i_upper - 1; ++i)
		{
			if(points[i].hitkey <= pivot.hitkey)
			{
				++i_pivot;
				temp = points[i];
				points[i] = points[i_pivot];
				points[i_pivot] = temp;
			}
		}

		++i_pivot;
		temp = points[i_pivot];
		points[i_pivot] = points[i_upper - 1];
		points[i_upper - 1] = temp;
	}

	quicksort_hitkey(points, i_lower, i_pivot);
	quicksort_hitkey(points, i_pivot + 1, i_upper);
}

void quicksort_counts(struct InttDeadMap_Point_s* points, InttDeadMap_Long_t i_lower, InttDeadMap_Long_t i_upper)
{
	if(i_lower < 0)return;
	if(i_upper < 1)return;
	if(i_upper <= i_lower)return;

	InttDeadMap_Long_t i_pivot = i_lower - 1;

	{
		InttDeadMap_Long_t i;

		struct InttDeadMap_Point_s pivot = points[i_upper - 1];
		struct InttDeadMap_Point_s temp;

		for(i = i_lower; i < i_upper - 1; ++i)
		{
			if(points[i].counts <= pivot.counts)
			{
				++i_pivot;
				temp = points[i];
				points[i] = points[i_pivot];
				points[i_pivot] = temp;
			}
		}

		++i_pivot;
		temp = points[i_pivot];
		points[i_pivot] = points[i_upper - 1];
		points[i_upper - 1] = temp;
	}

	quicksort_counts(points, i_lower, i_pivot);
	quicksort_counts(points, i_pivot + 1, i_upper);
}

struct InttDeadMap_Point_s* get_point(InttDeadMap_HitKey_t hitkey, struct InttDeadMap_Point_s* points, InttDeadMap_Long_t i_lower, InttDeadMap_Long_t i_upper)
{
	if(points[(i_upper + i_lower) / 2].hitkey == hitkey)	return &(points[(i_upper + i_lower) / 2]);

	if(i_upper - i_lower <= 1)				return NULL;

	if(hitkey < points[(i_upper + i_lower) / 2].hitkey)	return get_point(hitkey, points, i_lower, (i_upper + i_lower) / 2);
	if(points[(i_upper + i_lower) / 2].hitkey < hitkey)	return get_point(hitkey, points, (i_upper + i_lower) / 2, i_upper);

	return NULL;
}

void init_params(struct InttDeadMap_Param_s* params, struct InttDeadMap_Point_s* points, InttDeadMap_Long_t N, InttDeadMap_Double_t n_z, InttDeadMap_Double_t n_t)
{
	params->points = points;

	params->N = N;
	params->i_lower = 0;
	params->i_upper = N;

	params->n_z = n_z;
	params->n_t = n_t;
	params->p = 1.0 - erfl(n_z / sqrtl(2.0));
	params->q = sqrtl(params->p * (1.0 - params->p));

	params->n_g = 0;
	params->n_l = 0;
	params->n_u = 0;

	params->D = 0;
}

void count_type_i(struct InttDeadMap_Param_s* params)
{
	struct InttDeadMap_Point_s* points = params->points;

	InttDeadMap_Long_t i_lower = params->i_lower;
	InttDeadMap_Long_t i_upper = params->i_upper;

	InttDeadMap_Long_t n_g = 0;
	InttDeadMap_Long_t n_l = 0;
	InttDeadMap_Long_t n_u = 0;

	InttDeadMap_Double_t mu = 0;
	InttDeadMap_Double_t sg = 0;

	InttDeadMap_Double_t lower = 0;
	InttDeadMap_Double_t upper = 0;

	InttDeadMap_Long_t i = 0;

	for(i = i_lower; i < i_upper; ++i)mu += points[i].counts;
	mu /= (i_upper - i_lower);

	for(i = i_lower; i < i_upper; ++i)sg += (points[i].counts - mu) * (points[i].counts - mu);
	sg /= (i_upper - i_lower);
	sg = sqrtl(sg);

	lower = mu - params->n_z * sg;
	upper = mu + params->n_z * sg;

	for(i = i_lower; i < i_upper; ++i)
	{
		if(points[i].counts < lower)
		{
			++n_l;
			points[i].status = InttDeadMap_Status_LOWER;
			continue;
		}

		if(upper < points[i].counts)
		{
			++n_u;
			points[i].status = InttDeadMap_Status_UPPER;
			continue;
		}

		++n_g;
		points[i].status = InttDeadMap_Status_GOOD;
	}

	params->n_g = n_g;
	params->n_l = n_l;
	params->n_u = n_u;
}

void check_type_i(struct InttDeadMap_Param_s* params)
{
	InttDeadMap_Double_t lower = params->p * (params->i_upper - params->i_lower) - params->q * params->n_t * sqrtl(params->i_upper - params->i_lower);
	InttDeadMap_Double_t upper = params->p * (params->i_upper - params->i_lower) + params->q * params->n_t * sqrtl(params->i_upper - params->i_lower);

	std::cout << "p: " << params->p << std::endl;
	std::cout << lower << "\tto\t" << upper << std::endl;
	std::cout << params->p * (params->i_upper - params->i_lower) << std::endl;
	std::cout << params->n_l << std::endl;
	std::cout << params->n_u << std::endl;
}

void gen_points(struct InttDeadMap_Param_s* params)
{
	struct InttDeadMap_Point_s* points = params->points;
	InttDeadMap_Long_t N = params->N;

	InttDeadMap_Double_t mu = 4121.1324151;
	InttDeadMap_Double_t sg = 124.14124;

	InttDeadMap_Double_t z[2];
	InttDeadMap_Double_t u[2];

	InttDeadMap_Long_t i;
	InttDeadMap_HitKey_t h;
	InttDeadMap_Double_t c;

	for(i = 0; i < N; ++i)
	{
		if(i % 2 == 0)
		{
			u[0] = (InttDeadMap_Double_t)rand() / (InttDeadMap_Double_t)RAND_MAX;
			u[1] = (InttDeadMap_Double_t)rand() / (InttDeadMap_Double_t)RAND_MAX;

			z[0] = sqrt(-2.0 * log(u[0])) * cos(2.0 * 3.14159265358979 * u[1]);
			z[1] = sqrt(-2.0 * log(u[0])) * sin(2.0 * 3.14159265358979 * u[1]);
		}

		c = mu + sg * z[i % 2];

		points[i].hitkey = i;
		points[i].counts = c;
		points[i].status = InttDeadMap_Status_GOOD;
	}
}



