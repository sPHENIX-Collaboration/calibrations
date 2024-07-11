#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"

#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooPoisson.h"

#include "RooPlot.h"

R__LOAD_LIBRARY(libintt.so)
#include <intt/InttMapping.h>

typedef std::vector<std::pair<struct Intt::RawData_s, Long64_t>> Hitmap_t;

std::string default_filename = "/sphenix/user/jbertaux/commissioning/root_files/run_00008126.root";

bool save = true;
std::string save_dir = "/sphenix/user/jbertaux/commissioning/root_files/";
Long64_t num_bins = 20;

TTree* MakeDummyTree();

void DeadChannelClassifier(std::string filename=default_filename, std::string treename="tree")
{

//	TTree* tree = MakeDummyTree();

	//Load a file/tree and make an std::map of address -> num hits from it
	TFile* file = TFile::Open(filename.c_str(), "READ");
	if(!file)
	{
		std::cout << "Could not open file:" << std::endl;
		std::cout << "\t" << filename << std::endl;
		return;
	}
	TTree* tree = (TTree*)file->Get(treename.c_str());
	if(!file)
	{
		std::cout << "Could not get tree \"" << treename << "\"" << std::endl;
		std::cout << "From file:" << std::endl;
		std::cout << "\t" << filename << std::endl;
		return;
	}

	struct Intt::RawData_s raw_address;
	//std::map<struct Intt::RawData_s, Long64_t> hitmap;
	Hitmap_t hitmap = {};

	tree->SetBranchAddress("felix_server", &(raw_address.felix_server));
	tree->SetBranchAddress("felix_module", &(raw_address.felix_channel));
	tree->SetBranchAddress("chp", &(raw_address.chip));
	tree->SetBranchAddress("chn", &(raw_address.channel));

	Long64_t tot_hits = 0;

	std::size_t s;
	for(Long64_t n = 0; n < tree->GetEntriesFast(); ++n)
	{
		tree->GetEntry(n);

		//if(hitmap.find(raw_address) == hitmap.end())hitmap[raw_address] = 0;
		//++hitmap[raw_address];

		++tot_hits;

		s = 0;
		for(s = 0; s < hitmap.size(); ++s)
		{
			if(raw_address == hitmap[s].first)
			{
				++(hitmap[s].second);
				break;
			}
		}

		if(s < hitmap.size())continue;
		hitmap.push_back({raw_address, 1});
	}

	file->Close();
	delete file;

	//Given an std::map address -> num hits, make a new tree from this (one branch per address) and use RooFit to analyze hitrates
	Long64_t min_hits = INT_MAX;
	Long64_t max_hits = 0;
	Long64_t avg_hits = 0;

	tree = new TTree("hitmap", "hitmap");

	Long64_t num_hits;
	tree->Branch("num_hits", &num_hits);

	//for(std::map<struct Intt::RawData_s, Long64_t>::const_iterator itr = hitmap.begin(); itr != hitmap.end(); ++itr)
	for(Hitmap_t::const_iterator itr = hitmap.begin(); itr != hitmap.end(); ++itr)
	{
		num_hits = itr->second;
		tree->Fill();

		if(num_hits < min_hits)min_hits = num_hits;
		if(max_hits < num_hits)max_hits = num_hits;

		avg_hits += num_hits;
	}
	avg_hits /= tree->GetEntriesFast();

	RooRealVar x("num_hits", "num_hits", min_hits, max_hits);
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

	((RooRealVar&)lambda[0]).setVal(avg_hits);
	((RooRealVar&)coef[0]).setVal(tree->GetEntriesFast());

	((RooRealVar&)lambda[1]).setVal(1);
	((RooRealVar&)coef[1]).setVal(0);

	((RooRealVar&)lambda[2]).setVal(2 * ((RooRealVar&)lambda[0]).getValV());
	((RooRealVar&)coef[2]).setVal(0);
	
	RooDataSet* data_set = new RooDataSet("data_set", "data_set", RooArgSet(x), RooFit::Import(*tree));
	RooFitResult* result = model->fitTo(*data_set);

	RooPlot* plot = x.frame(RooFit::Title("Hitrates across INTT channels"));

	data_set->plotOn
	(
		plot//,
		//RooFit::RefreshNorm(),
		//RooFit::Binning(num_bins, min_hits, max_hits),
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

	std::size_t pos;
	while(true)
	{
		pos = filename.find("/");
		if(pos == std::string::npos)break;
		filename = filename.substr(pos+1);
	}
	pos = filename.find(".");
	filename = filename.substr(0, pos);
	filename = save_dir + filename;
	filename += ".png";
	std::cout << filename << std::endl;

	TCanvas* canvas = new TCanvas("cnvs", "Hitrates across INTT channels");
	canvas->cd();
	plot->Draw();
	if(save)canvas->SaveAs(filename.c_str());
	delete tree;


	//Make another tree, and using the original hitmap and now our model, fill the tree with the physical address and classification
	tree = new TTree("statusmap", "statusmap");
	int status;
	struct Intt::Online_s online_address;
	tree->Branch("num_hits", &num_hits);
	tree->Branch("status", &status);
	tree->Branch("lyr", &(online_address.lyr));
	tree->Branch("ldr", &(online_address.ldr));
	tree->Branch("arm", &(online_address.arm));
	tree->Branch("chp", &(online_address.chp));
	tree->Branch("chn", &(online_address.chn));

	std::cout << "tot_hits: " << tot_hits << std::endl;
	tot_hits = 0;
	for(Hitmap_t::const_iterator itr = hitmap.begin(); itr != hitmap.end(); ++itr)
	{
		tot_hits += itr->second;
	}
	std::cout << "tot_hits: " << tot_hits << std::endl;

	for(Hitmap_t::const_iterator itr = hitmap.begin(); itr != hitmap.end(); ++itr)
	{
		num_hits = itr->second;

		online_address = Intt::ToOnline(itr->first);
		x.setVal(itr->second);
		int i = 0;
		Double_t p = 0;
		Double_t q = 0;

		for(i = 0; i < 3; ++i)
		{
			q = ((RooPoisson&)poiss[i]).getValV() * ((RooRealVar&)coef[i]).getValV();

			if(q < p)continue;
			p = q;
			status = i;
		}

		tree->Fill();
	}

	tot_hits = 0;
	for(Long64_t n = 0; n < tree->GetEntriesFast(); ++n)
	{
		tree->GetEntry(n);
		tot_hits += num_hits;
	}
	std::cout << "tot_hits: " << tot_hits << std::endl;

	pos = filename.find(".");
	filename = filename.substr(0, pos);
	filename += "_channel_classification.root";
	if(save)
	{
		file = TFile::Open(filename.c_str(), "RECREATE");
		file->cd();
		tree->Write();
		file->Write();
		file->Close();
	}
};

TTree* MakeDummyTree()
{
	Double_t means[3] = {331, 6, 586};

	TTree* tree = new TTree("tree", "tree");

	int num_hits = 0;
	int channel = 0;
	tree->Branch("num_hits", &num_hits);
	tree->Branch("channel", &channel);

	TRandom rng;
	for(int i = 0; i < 3; ++i)
	{
		int N = (int)rng.Uniform(i == 0 ? 300 : 100);
		N += i == 0 ? 200 : 100;

		for(int n = 0; n < N; ++n)
		{
			num_hits = rng.Poisson(means[i]);
			tree->Fill();
			++channel;
		}
	}

	return tree;
}
