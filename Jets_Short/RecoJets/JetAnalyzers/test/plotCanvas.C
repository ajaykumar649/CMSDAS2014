{
  gStyle->SetOptStat(111111);
  gStyle->SetStatX(0.9);
  gStyle->SetStatY(0.9);

  TFile *f = TFile::Open("PFJetPlotsExample_MC.root");

  TH1F *h1 = (TH1F*)f->Get("JetEta");
  TH1F *h2 = (TH1F*)f->Get("JetPhi");
  TH1F *h3 = (TH1F*)f->Get("JetPt");
  TH1F *h4 = (TH1F*)f->Get("NumberOfJets");

  TCanvas *c1 = new TCanvas("c1","n1",400,400);
  c1->cd(); 
  h1->GetXaxis()->SetTitle("#eta");
  h1->GetYaxis()->SetTitle("Events/0.1");
  h1->SetTitle("Jet #eta");
  h1->Draw();
  c1->Print("pfeta.gif");

  TCanvas *c2 = new TCanvas("c2","n2",400,400);
  c2->cd();
  h2->GetXaxis()->SetTitle("#phi");
  h2->GetYaxis()->SetTitle("Events/(2#pi/100)");
  h2->SetTitle("Jet #phi");
  h2->Draw();
  c2->Print("pfphi.gif");

  TCanvas *c3 = new TCanvas("c3","n3",400,400);
  c3->cd();
  h3->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  h3->GetYaxis()->SetTitle("Events/(10 GeV/c)");
  h3->SetTitle("Jet p_{T}");
  h3->GetXaxis()->SetRangeUser(0,200);
  h3->Draw();
  c3->Print("pfpt.gif");

  TCanvas *c4 = new TCanvas("c4","n4",400,400);
  c4->cd();
  h4->GetXaxis()->SetTitle("# Jets");
  h4->GetYaxis()->SetTitle("Events");
  h4->SetTitle("Number of Jets");
  h4->GetXaxis()->SetRangeUser(0,35);
  h4->Draw();
  c4->Print("pfnjets.gif");
}
