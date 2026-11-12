#include <TFile.h>
#include <TTree.h>
#include <TChain.h>

#include <iostream>

void TrimROOTTree(
    const char* inputpattern,
    const char* outputfile,
    const char* treename = "tree",
    bool is_simulation_input = false)
{
    //-----------------------------------------
    // Build chain from input pattern
    //-----------------------------------------
    TChain chain(treename);

    int nfiles = chain.Add(inputpattern);

    if (nfiles == 0) {
        std::cerr << "ERROR: no files matched input pattern: "
                  << inputpattern << std::endl;
        return;
    }

    std::cout << "Added " << nfiles << " file(s)." << std::endl;
    //std::cout << "Total entries: " << chain.GetEntries() << std::endl;

    //-----------------------------------------
    // Disable ALL branches
    //-----------------------------------------
    chain.SetBranchStatus("*", 0);

    //-----------------------------------------
    // Enable wanted branches
    //-----------------------------------------
    chain.SetBranchStatus("pcax", 1);
    chain.SetBranchStatus("pcay", 1);
    chain.SetBranchStatus("pcaz", 1);

    chain.SetBranchStatus("dcaxy", 1);
    chain.SetBranchStatus("dcaz", 1);

    chain.SetBranchStatus("px", 1);
    chain.SetBranchStatus("py", 1);
    chain.SetBranchStatus("pz", 1);

    chain.SetBranchStatus("pt", 1);
    chain.SetBranchStatus("eta", 1);
    chain.SetBranchStatus("phi", 1);

    chain.SetBranchStatus("charge", 1);
    chain.SetBranchStatus("quality", 1);
    chain.SetBranchStatus("chisq", 1);
    chain.SetBranchStatus("ndf", 1);

    chain.SetBranchStatus("nhits", 1);
    chain.SetBranchStatus("nmaps", 1);
    chain.SetBranchStatus("nintt", 1);
    chain.SetBranchStatus("ntpc", 1);
    chain.SetBranchStatus("nmms", 1);

    chain.SetBranchStatus("vertex_ntracks", 1);

    chain.SetBranchStatus("run", 1);
    chain.SetBranchStatus("segment", 1);
    chain.SetBranchStatus("event", 1);
    chain.SetBranchStatus("crossing", 1);

    chain.SetBranchStatus("vx", 1);
    chain.SetBranchStatus("vy", 1);
    chain.SetBranchStatus("vz", 1);

    chain.SetBranchStatus("vertexid", 1);

    //-----------------------------------------
    // Optional truth branches
    //-----------------------------------------
    if (is_simulation_input)
    {
        chain.SetBranchStatus("true_vx", 1);
        chain.SetBranchStatus("true_vy", 1);
        chain.SetBranchStatus("true_vz", 1);

        chain.SetBranchStatus("true_dcaxy", 1);
        chain.SetBranchStatus("true_dcaz", 1);

        chain.SetBranchStatus("true_px", 1);
        chain.SetBranchStatus("true_py", 1);
        chain.SetBranchStatus("true_pz", 1);
    }

    //-----------------------------------------
    // Create output file
    //-----------------------------------------
    TFile* fout = TFile::Open(outputfile, "RECREATE");

    if (!fout || fout->IsZombie()) {
        std::cerr << "ERROR: could not create output file: "
                  << outputfile << std::endl;
        return;
    }

    fout->cd();

    //-----------------------------------------
    // Clone ONLY active branches from all files
    //-----------------------------------------
    TTree* tout = chain.CloneTree(-1, "fast");

    if (!tout) {
        std::cerr << "ERROR: CloneTree failed." << std::endl;
        fout->Close();
        return;
    }

    //-----------------------------------------
    // Write output
    //-----------------------------------------
    tout->Write(treename);

    fout->Close();

    std::cout << "Wrote trimmed tree to: "
              << outputfile << std::endl;
}
