#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"

#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooPoisson.h"

#include "RooPlot.h"

void DeadChannelClassifier()
{
	//v for debugging
	Double_t num_cold = 0.2;
	Double_t lambda_cold = 2.2;

	Double_t lambda_nom = 12.3;

	Double_t num_hot = 0.3;
	Double_t lambda_hot = 62.0;
	//^ for debugging

	Long64_t count_min = 0;
	Long64_t count_max = 100;
	Long64_t num_bins = 20;
	Long64_t N = 1000;
	RooRealVar x("x", "x", count_min, count_max);

	RooArgList lambda;
	RooArgList poiss;
	RooArgList coef;
	for(int i = 0; i < 3; ++i)
	{
		lambda.addOwned(*(new RooRealVar(Form("lambda_%d", i), Form("lambda_%d", i), 0, FLT_MAX)));
		poiss.addOwned(*(new RooPoisson(Form("poiss_%d", i), Form("poiss_%d", i), x, (RooRealVar&)lambda[i])));
		coef.addOwned(*(new RooRealVar(Form("coef_%d", i), Form("coef_%d", i), 0, FLT_MAX)));
	}
	RooAddPdf* model = new RooAddPdf("model", "model", poiss, coef);

	//make a TTree* from a PRDF file and RooFit::Import(*tree)
	RooDataSet* data_set = new RooDataSet("data_set", "data_set", RooArgSet(x));

	//v for debugging
	((RooRealVar&)lambda[0]).setVal(lambda_nom);
	((RooRealVar&)coef[0]).setVal(N - num_cold * N - num_hot * N);

	((RooRealVar&)lambda[1]).setVal(lambda_cold);
	((RooRealVar&)coef[1]).setVal(num_cold * N);

	((RooRealVar&)lambda[2]).setVal(lambda_hot);
	((RooRealVar&)coef[2]).setVal(num_hot * N);

	RooDataSet* data[3];
	for(int i = 0; i < 3; ++i)
	{
		data[i] = ((RooPoisson&)poiss[i]).generate(x, ((RooRealVar&)coef[i]).getValV());
		data_set->append(*data[i]);
	}
	//^for debugging

	//change initial estimates based on average hit rate
	//need to get the first value as an average; other defaults should be fine
	((RooRealVar&)lambda[0]).setVal(lambda_hot * num_hot + lambda_cold * num_cold + (1.0 - num_hot - num_cold) * lambda_nom);
	((RooRealVar&)coef[0]).setVal(N);

	((RooRealVar&)lambda[1]).setVal(1);
	((RooRealVar&)coef[1]).setVal(0);

	((RooRealVar&)lambda[2]).setVal(2 * ((RooRealVar&)lambda[0]).getValV());
	((RooRealVar&)coef[2]).setVal(N);
	
	RooFitResult* result = model->fitTo(*data_set);

	//given a count
	Long64_t count;

	//Unnormalized probabilities of the count taking a particular value
	//choose the category for which the unnormalized prior is largest
	count = (count_max + count_min) / 2;
	std::cout << "Count:\t" << count << std::endl;
	x.setVal(count);
	std::cout << "P good:\t"	<< ((RooPoisson&)poiss[0]).getValV() * ((RooRealVar&)coef[0]).getValV() << std::endl;
	std::cout << "P cold:\t"	<< ((RooPoisson&)poiss[1]).getValV() * ((RooRealVar&)coef[1]).getValV() << std::endl;
	std::cout << "P hot:\t"		<< ((RooPoisson&)poiss[2]).getValV() * ((RooRealVar&)coef[2]).getValV() << std::endl;

	count = 10;
	std::cout << "Count:\t" << count << std::endl;
	x.setVal(count);
	std::cout << "P good:\t" 	<< ((RooPoisson&)poiss[0]).getValV() * ((RooRealVar&)coef[0]).getValV() << std::endl;
	std::cout << "P cold:\t"	<< ((RooPoisson&)poiss[1]).getValV() * ((RooRealVar&)coef[1]).getValV() << std::endl;
	std::cout << "P hot:\t"		<< ((RooPoisson&)poiss[2]).getValV() * ((RooRealVar&)coef[2]).getValV() << std::endl;

	count = 2;
	std::cout << "Count:\t" << count << std::endl;
	x.setVal(count);
	std::cout << "P good:\t" 	<< ((RooPoisson&)poiss[0]).getValV() * ((RooRealVar&)coef[0]).getValV() << std::endl;
	std::cout << "P cold:\t"	<< ((RooPoisson&)poiss[1]).getValV() * ((RooRealVar&)coef[1]).getValV() << std::endl;
	std::cout << "P hot:\t"		<< ((RooPoisson&)poiss[2]).getValV() * ((RooRealVar&)coef[2]).getValV() << std::endl;


	RooPlot* plot = x.frame(RooFit::Title("Poiss"));
	//...

	data_set->plotOn
	(
		plot//,
		//RooFit::RefreshNorm(),
		//RooFit::Binning(num_bins, count_min, count_max),
		//RooFit::MarkerColor(kBlack),
		//RooFit::MarkerStyle(8)
	);

	model->plotOn
	(
		plot//,
		//RooFit::LineColor(kRed),
		//RooFit::LineStyle(1),
		//RooFit::LineWidth(3),
		//RooFit::Normalization(data_set->sumEntries())
	);

	TCanvas* canvas = new TCanvas("foo", "bar");
	canvas->cd();
	plot->Draw();
};
