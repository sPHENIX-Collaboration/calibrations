#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"

#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooPoisson.h"

#include "RooPlot.h"

Long64_t num_bins = 20;

void MakeDummyTree();

void DeadChannelClassifier(std::string filename, std::string treename="tree")
{
	//TFile* file = TFile::Open(filename.c_str());
	//if(!file)
	//{
	//	std::cout << "Could not open file:" << std::endl;
	//	std::cout << "\t" << filename << std::endl;
	//	return;
	//}
	//TTree* tree = (TTree*)file->Get(treename.c_str());
	//if(!file)
	//{
	//	std::cout << "Could not get tree \"" << treename << "\"" << std::endl;
	//	std::cout << "From file:" << std::endl;
	//	std::cout << "\t" << filename << std::endl;
	//	return;
	//}
	TTree* tree = nullptr;
	MakeDummyTree(tree);

	Long64_t min_hits = INT_MAX;
	Long64_t max_hits = 0;
	Long64_t avg_hits = 0;

	int num_hits;
	tree->SetBranchAddress("num_hits", &num_hits);
	for(Long64_t n = 0; n < tree->GetEntriesFast(); ++n)
	{
		tree->GetEntry(n);

		if(num_hits < min_hits)min_hits = num_hits;
		if(max_hits < num_hits)max_hits = num_hits;

		avg_hits += num_hits;
	}
	avg_hits /= tree->GetEntriesFast();
	RooRealVar x("num_hits", "num_hits", count_min, count_max);

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

	//change initial estimates based on average hit rate
	((RooRealVar&)lambda[0]).setVal(avg_hits);
	((RooRealVar&)coef[0]).setVal(tree->GetEntriesFast());

	((RooRealVar&)lambda[1]).setVal(1);
	((RooRealVar&)coef[1]).setVal(0);

	((RooRealVar&)lambda[2]).setVal(2 * ((RooRealVar&)lambda[0]).getValV());
	((RooRealVar&)coef[2]).setVal(N);
	
	RooDataSet* data_set = new RooDataSet("data_set", "data_set", RooArgSet(x), RooFit::Import(*tree));
	RooFitResult* result = model->fitTo(*data_set);

	RooPlot* plot = x.frame(RooFit::Title("Hitrates across INTT channels"));

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

	TCanvas* canvas = new TCanvas("cnvs", "Hitrates across INTT channels");
	canvas->cd();
	plot->Draw();

	//channel classification
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

};

void MakeDummyTree(TTree*& tree)
{
	if(tree)delete tree;

	tree = new TTree("tree", "tree");

	int num_hits = 0;
	int channel = 0;
	tree->Branch("num_hits", &num_hits);
	tree->Branch("channel", &channel);

	RooArgList lambda;
	RooArgList poiss;
	for(int i = 0; i < 3; ++i)
	{
		lambda.addOwned(*(new RooRealVar(Form("lambda_%d", i), Form("lambda_%d", i), 0, FLT_MAX)));
		poiss.addOwned(*(new RooPoisson(Form("poiss_%d", i), Form("poiss_%d", i), x, (RooRealVar&)lambda[i])));
	}

	int N;
	for(int i = 0; i < 3; ++i)
	{
		N = (int)TRandom::Uniform(100);
		for(int n = 0; n < N; ++n)
		{
			num_hits = poiss[i])
			++channel;
			tree->Fill();
		}
	}
}
