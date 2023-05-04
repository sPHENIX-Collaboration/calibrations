#ifndef INTT_DEAD_MAP_HELPER_H
#define INTT_DEAD_MAP_HELPER_H

//C style includes
#include <stdlib.h>
#include <math.h>

//CPP style includes
#include <iostream>

//ROOT and sPHENIX includes
//#include <coresoftware/offline/packages/trackbase/InttDefs.h
//for getHitSetKey

typedef int		InttDeadMap_HitKey_t;
typedef long double	InttDeadMap_Double_t;
typedef long long	InttDeadMap_Long_t;

enum InttDeadMap_Status_e
{
	InttDeadMap_Status_GOOD,
	InttDeadMap_Status_LOWER,
	InttDeadMap_Status_UPPER
};

struct InttDeadMap_Point_s
{
	InttDeadMap_HitKey_t	hitkey;
	InttDeadMap_Long_t	counts;
	InttDeadMap_Status_e	status;
};

struct InttDeadMap_Param_s
{	
	InttDeadMap_Point_s*	points;

	InttDeadMap_Long_t	N;
	InttDeadMap_Long_t	i_lower;
	InttDeadMap_Long_t	i_upper;

	InttDeadMap_Double_t	n_z;
	InttDeadMap_Double_t	n_t;
	InttDeadMap_Double_t	p;
	InttDeadMap_Double_t	q;

	InttDeadMap_Double_t	mu;
	InttDeadMap_Double_t	sg;

	InttDeadMap_Long_t	n_g;
	InttDeadMap_Long_t	n_l;
	InttDeadMap_Long_t	n_u;

	InttDeadMap_Long_t	D;
};

void quicksort_hitkey(struct InttDeadMap_Point_s*, InttDeadMap_Long_t, InttDeadMap_Long_t);
void quicksort_counts(struct InttDeadMap_Point_s*, InttDeadMap_Long_t, InttDeadMap_Long_t);
struct InttDeadMap_Point_s* get_point(struct InttDeadMap_Point_s*, InttDeadMap_Long_t, InttDeadMap_Long_t);


void init_params(struct InttDeadMap_Param_s*, struct InttDeadMap_Point_s*, InttDeadMap_Long_t, InttDeadMap_Double_t, InttDeadMap_Double_t);

int count_type_i(struct InttDeadMap_Param_s*);
int check_type_i(struct InttDeadMap_Param_s*);

void gen_points(struct InttDeadMap_Param_s*, InttDeadMap_Double_t);

#endif
