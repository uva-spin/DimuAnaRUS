#include <fstream>
#include <iomanip>
#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <interface_main/SQRun.h>
#include <interface_main/SQHitVector.h>
#include <interface_main/SQEvent.h>
#include <interface_main/SQMCEvent.h>
#include <interface_main/SQRun.h>
#include <interface_main/SQTrackVector.h>
#include <interface_main/SQDimuonVector.h>
#include <ktracker/SRecEvent.h>
#include <fun4all/Fun4AllReturnCodes.h>
#include <phool/PHNodeIterator.h>
#include <phool/PHIODataNode.h>
#include <phool/getClass.h>
#include <geom_svc/GeomSvc.h>
#include "DimuAnaRUS.h"
using namespace std;

DimuAnaRUS::DimuAnaRUS(const std::string& name)
  : SubsysReco  (name),
    m_file(0),
    m_tree(0),
    m_tree_name("tree"),
    m_file_name("output.root"),
    m_evt(0),
    m_sp_map(0),
    m_hit_vec(0),
    m_sq_trk_vec(0),
    m_sq_dim_vec(0),
    saveDimuonOnly(false),
    true_mode(false),
    reco_dimu_mode(false),
    reco_mode(false),
    data_trig_mode(false),
    mc_trig_mode(true)
{
  ;
}

DimuAnaRUS::~DimuAnaRUS()
{
  ;
}

int DimuAnaRUS::Init(PHCompositeNode* startNode)
{
  return Fun4AllReturnCodes::EVENT_OK;
}

int DimuAnaRUS::InitRun(PHCompositeNode* startNode)
{
	m_file = new TFile(m_file_name.c_str(), "RECREATE");

	if (!m_file || m_file->IsZombie()) {
		std::cerr << "Error: Could not create file " << m_file_name << std::endl;
		exit(1);
	} else {
		std::cout << "File " << m_file->GetName() << " opened successfully." << std::endl;
	}

	m_tree = new TTree(m_tree_name.c_str(), "Tree for storing events");
	if (!m_tree) {
		std::cerr << "Error: Could not create tree " << m_tree_name << std::endl;
		exit(1);
	} else {
		std::cout << "Tree " << m_tree->GetName() << " created successfully." << std::endl;
	}

	m_tree->Branch("eventID", &eventID, "eventID/I");
/*
	m_tree->Branch("runID", &runID, "runID/I");
	m_tree->Branch("spillID", &spillID, "spillID/I");
	m_tree->Branch("eventID", &eventID, "eventID/I");
	m_tree->Branch("rfID", &rfID, "rfID/I");
	m_tree->Branch("turnID", &turnID, "turnID/I");
	m_tree->Branch("rfIntensity", rfIntensity, "rfIntensity[33]/I");
	m_tree->Branch("fpgaTrigger", fpgaTrigger, "fpgaTrigger[5]/I");
	m_tree->Branch("nimTrigger", nimTrigger, "nimTrigger[5]/I");
*/
    if(sqhit_flag){
        m_tree->Branch("hitID", &hitID);
        m_tree->Branch("hit_trackID", &hit_trackID);
        m_tree->Branch("processID", &processID);
        m_tree->Branch("detectorID", &detectorID);
        m_tree->Branch("elementID", &elementID);
        m_tree->Branch("tdcTime", &tdcTime);
        m_tree->Branch("driftDistance", &driftDistance);
    }

	if (true_mode) {
        m_tree->Branch("gCharge", &gCharge);
        m_tree->Branch("trackID", &trackID);
        m_tree->Branch("gvx", &gvx);
        m_tree->Branch("gvy", &gvy);
        m_tree->Branch("gvz", &gvz);
        m_tree->Branch("gpx", &gpx);
        m_tree->Branch("gpy", &gpy);
        m_tree->Branch("gpz", &gpz);

        m_tree->Branch("gx_st1", &gx_st1);
        m_tree->Branch("gy_st1", &gy_st1);
        m_tree->Branch("gz_st1", &gz_st1);
        m_tree->Branch("gpx_st1", &gpx_st1);
        m_tree->Branch("gpy_st1", &gpy_st1);
        m_tree->Branch("gpz_st1", &gpz_st1);

        m_tree->Branch("gx_st3", &gx_st3);
        m_tree->Branch("gy_st3", &gy_st3);
        m_tree->Branch("gz_st3", &gz_st3);
        m_tree->Branch("gpx_st3", &gpx_st3);
        m_tree->Branch("gpy_st3", &gpy_st3);
        m_tree->Branch("gpz_st3", &gpz_st3);
}

	if (reco_mode==true){
		m_tree->Branch("rec_charge", &rec_charge);
		m_tree->Branch("rec_vx", &rec_vx);
		m_tree->Branch("rec_vy", &rec_vy);
		m_tree->Branch("rec_vz", &rec_vz);
		m_tree->Branch("rec_px", &rec_px);
		m_tree->Branch("rec_py", &rec_py);
		m_tree->Branch("rec_pz", &rec_pz);
		m_tree->Branch("rec_x_st1", &rec_x_st1);
		m_tree->Branch("rec_y_st1", &rec_y_st1);
		m_tree->Branch("rec_z_st1", &rec_z_st1);
		m_tree->Branch("rec_px_st1", &rec_px_st1);
		m_tree->Branch("rec_py_st1", &rec_py_st1);
		m_tree->Branch("rec_pz_st1", &rec_pz_st1);
		m_tree->Branch("rec_x_st3", &rec_x_st3);
		m_tree->Branch("rec_y_st3", &rec_y_st3);
		m_tree->Branch("rec_z_st3", &rec_z_st3);
		m_tree->Branch("rec_px_st3", &rec_px_st3);
		m_tree->Branch("rec_py_st3", &rec_py_st3);
		m_tree->Branch("rec_pz_st3", &rec_pz_st3);
        m_tree->Branch("rec_chisq_dump", &rec_chisq_dump);
        m_tree->Branch("rec_chisq_upstream", &rec_chisq_upstream);
        m_tree->Branch("rec_chisq_target", &rec_chisq_target);

        if(reco_dimu_mode==true){
            // ===== Dimuon target
            m_tree->Branch("dimu_px_tgt", &dimu_px_tgt);
            m_tree->Branch("dimu_py_tgt", &dimu_py_tgt);
            m_tree->Branch("dimu_pz_tgt", &dimu_pz_tgt);
            m_tree->Branch("dimu_mass_tgt", &dimu_mass_tgt);

            // ===== Dimuon dump
            m_tree->Branch("dimu_px_dmp", &dimu_px_dmp);
            m_tree->Branch("dimu_py_dmp", &dimu_py_dmp);
            m_tree->Branch("dimu_pz_dmp", &dimu_pz_dmp);
            m_tree->Branch("dimu_mass_dmp", &dimu_mass_dmp);

            // ===== Mu+ target
            m_tree->Branch("mu_pos_px_tgt", &mu_pos_px_tgt);
            m_tree->Branch("mu_pos_py_tgt", &mu_pos_py_tgt);
            m_tree->Branch("mu_pos_pz_tgt", &mu_pos_pz_tgt);

            // ===== Mu+ dump
            m_tree->Branch("mu_pos_px_dmp", &mu_pos_px_dmp);
            m_tree->Branch("mu_pos_py_dmp", &mu_pos_py_dmp);
            m_tree->Branch("mu_pos_pz_dmp", &mu_pos_pz_dmp);

            // ===== Mu− target
            m_tree->Branch("mu_neg_px_tgt", &mu_neg_px_tgt);
            m_tree->Branch("mu_neg_py_tgt", &mu_neg_py_tgt);
            m_tree->Branch("mu_neg_pz_tgt", &mu_neg_pz_tgt);

            // ===== Mu− dump
            m_tree->Branch("mu_neg_px_dmp", &mu_neg_px_dmp);
            m_tree->Branch("mu_neg_py_dmp", &mu_neg_py_dmp);
            m_tree->Branch("mu_neg_pz_dmp", &mu_neg_pz_dmp);

            m_tree->Branch("mu_neg_pz_dmp", &mu_neg_pz_dmp);
            m_tree->Branch("mu_neg_pz_dmp", &mu_neg_pz_dmp);
            m_tree->Branch("mu_neg_pz_dmp", &mu_neg_pz_dmp);

            m_tree->Branch("top_bot", &top_bot);
            m_tree->Branch("bot_top", &bot_top);
        }
	}

	if (true_mode) {
		m_vec_trk = findNode::getClass<SQTrackVector>(startNode, "SQTruthTrackVector");
		m_evt_true = findNode::getClass<SQMCEvent     >(startNode, "SQMCEvent");
		if (!m_vec_trk || !m_evt_true) {
			return Fun4AllReturnCodes::ABORTEVENT;
		}
	}

	if (reco_mode) {
		m_sq_trk_vec = findNode::getClass<SQTrackVector>(startNode, "SQRecTrackVector");
		if(reco_dimu_mode==true) m_sq_dim_vec = findNode::getClass<SQDimuonVector>(startNode, "SQRecDimuonVector_PM");

		if (!m_sq_trk_vec) {
			std::cerr << "Error: m_sq_trk_vec is null!" << std::endl;
			return Fun4AllReturnCodes::ABORTEVENT;
		}

    if(reco_dimu_mode==true){
        if (!m_sq_dim_vec) {
            std::cerr << "Error: m_sq_dim_vec is null!" << std::endl;
            return Fun4AllReturnCodes::ABORTEVENT;
        }
    }
	}
	m_evt = findNode::getClass<SQEvent>(startNode, "SQEvent");
    if(sqhit_flag){
        m_hit_vec = findNode::getClass<SQHitVector>(startNode, "SQHitVector");
        if (!m_evt || !m_hit_vec) {
            return Fun4AllReturnCodes::ABORTEVENT;
        }
   }
	if(reco_mode ==true && data_trig_mode ==true){
		//cout << "inside the data roadset mode: "<<endl;
		SQRun* sq_run = findNode::getClass<SQRun>(startNode, "SQRun");
		if (!sq_run) std::cout << "Error: SQRun  is null!" << std::endl;
		if (!sq_run) return Fun4AllReturnCodes::ABORTEVENT;
		int LBtop = sq_run->get_v1495_id(2);
		int LBbot = sq_run->get_v1495_id(3);
		int ret = m_rs.LoadConfig(LBtop, LBbot);
		if (ret != 0) {
			cout << "!!WARNING!!  OnlMonTrigEP::InitRunOnlMon():  roadset.LoadConfig returned " << ret << ".\n";
		}
		cout <<"Roadset " << m_rs.str(1) << endl;

	}

	if(reco_mode ==true && mc_trig_mode ==true){
		int ret = m_rs.LoadConfig(131);
		if (ret != 0) {
			cout << "!!WARNING!!  OnlMonTrigEP::InitRunOnlMon():  roadset.LoadConfig returned " << ret << ".\n";
		}
		//cout <<"Roadset " << m_rs.str(1) << endl;
	}
	return Fun4AllReturnCodes::EVENT_OK;
}


int DimuAnaRUS::process_event(PHCompositeNode* startNode)
{

	int proc_id=0;
	int SourceFlag=0;
	int true_proc = m_evt_true->get_process_id();
	//cout <<" true process id "<< true_proc <<endl;
	/*   //if (! m_evt->get_trigger(SQEvent::MATRIX1)) {
	   if (! m_evt->get_trigger(SQEvent::NIM4)) {
	   return Fun4AllReturnCodes::EVENT_OK;
	   }
	*/
	//if (saveDimuonOnly && m_sq_dim_vec->empty()) {
	//	return 0;  // Skip this event if no dimuons are present and the mode is enabled
	//}
 
	eventID = m_evt->get_event_id();
	/*
	   runID = m_evt->get_run_id();
	   spillID = m_evt->get_spill_id();
	   rfID = m_evt->get_qie_rf_id();
	   turnID = m_evt->get_qie_turn_id();

	   fpgaTrigger[0] = m_evt->get_trigger(SQEvent::MATRIX1);
	   fpgaTrigger[1] = m_evt->get_trigger(SQEvent::MATRIX2);
	   fpgaTrigger[2] = m_evt->get_trigger(SQEvent::MATRIX3);
	   fpgaTrigger[3] = m_evt->get_trigger(SQEvent::MATRIX4);
	   fpgaTrigger[4] = m_evt->get_trigger(SQEvent::MATRIX5);

	   nimTrigger[0] = m_evt->get_trigger(SQEvent::NIM1);
	   nimTrigger[1] = m_evt->get_trigger(SQEvent::NIM2);
	   nimTrigger[2] = m_evt->get_trigger(SQEvent::NIM3);
	   nimTrigger[3] = m_evt->get_trigger(SQEvent::NIM4);
	   nimTrigger[4] = m_evt->get_trigger(SQEvent::NIM5);
	   for (int i = -16; i <= 16; ++i) {
	   rfIntensity[i + 16] = m_evt->get_qie_rf_intensity(i);
	   }
	   */

	const double muon_mass = 0.10566;
	TLorentzVector mu1;
	TLorentzVector mu2;
	if(true_mode == true){
		ResetTrueBranches();
		ResetHitBranches();
		for (unsigned int ii = 0; ii < m_vec_trk->size(); ii++) {

			//cout << "inside the true track loop: "<<endl;
			SQTrack* trk = m_vec_trk->at(ii);
			gCharge.push_back(trk->get_charge());
			trackID.push_back(trk->get_track_id());
			gvx.push_back(trk->get_pos_vtx().X());
			gvy.push_back(trk->get_pos_vtx().Y());
			gvz.push_back(trk->get_pos_vtx().Z());
			gpx.push_back(trk->get_mom_vtx().Px());
			gpy.push_back(trk->get_mom_vtx().Py());
			gpz.push_back(trk->get_mom_vtx().Pz());

			gx_st1.push_back(trk->get_pos_st1().X());
			gy_st1.push_back(trk->get_pos_st1().Y());
			gz_st1.push_back(trk->get_pos_st1().Z());
			gpx_st1.push_back(trk->get_mom_st1().Px());
			gpy_st1.push_back(trk->get_mom_st1().Py());
			gpz_st1.push_back(trk->get_mom_st1().Pz());

			gx_st3.push_back(trk->get_pos_st3().X());
			gy_st3.push_back(trk->get_pos_st3().Y());
			gz_st3.push_back(trk->get_pos_st3().Z());
			gpx_st3.push_back(trk->get_mom_st3().Px());
			gpy_st3.push_back(trk->get_mom_st3().Py());
			gpz_st3.push_back(trk->get_mom_st3().Pz());

			/*
			// Only calculate dimuon if we have exactly 2 tracks AND the true_proc is between 400 and 412
			if (m_vec_trk->size() == 2 && true_proc >= 400 && true_proc <= 412) {
				if (ii == 0) {
					mu1.SetXYZM(trk->get_mom_vtx().Px(),
							trk->get_mom_vtx().Py(),
							trk->get_mom_vtx().Pz(),
							muon_mass);
				}

				if (ii == 1) {
					mu2.SetXYZM(trk->get_mom_vtx().Px(),
							trk->get_mom_vtx().Py(),
							trk->get_mom_vtx().Pz(),
							muon_mass);

					TLorentzVector dimuon = mu1 + mu2;
					double dimuon_mass = dimuon.M();
					std::cout << "Dimuon invariant mass: " << dimuon_mass << " GeV" << std::endl;
					std::cout << "true_proc " << true_proc << std::endl;

					// Assign proc_id by mass
					if (std::abs(dimuon_mass - 3.097) < 0.05) {
						proc_id = 12;  // J/ψ → μ+μ−
					}
					else if (std::abs(dimuon_mass - 3.686) < 0.05) {
						proc_id = 13;  // ψ′ → μ+μ−
					}
				}
			}			
			*/

			//if (true_proc == 223)proc_id = 11;
			//if (true_proc == 0)proc_id = 14;

			int SourceFlag=0;
			double z = trk->get_pos_vtx().Z();
			if (z <= -296. && z >= -304.) SourceFlag = 1; // target
			else if (z >= 0. && z < 250) SourceFlag = 2;   // dump
			else if (z > -296. && z < 0.) SourceFlag = 3;  // gap
			else if (z < -304) SourceFlag = 0;              // upstream

            if(sqhit_flag){
                if (m_hit_vec) {
                    for (int ihit = 0; ihit < m_hit_vec->size(); ++ihit) {
                        SQHit* hit = m_hit_vec->at(ihit);
                        //if (!hit->is_in_time()) continue;
                        proc_id=12;
                        if(hit->get_track_id() != trk->get_track_id()) continue;
                        int processID_;
                        if(trk->get_charge() >0) processID_ =proc_id;
                        else processID_ = proc_id +10 ;
                        int sourceFlag_= SourceFlag;
                        unsigned int encodedValue = EncodeProcess(processID_, sourceFlag_);
                        //cout << "charge: "<< trk->get_charge() <<endl;
                        //cout << "encodedValue: "<< encodedValue<< endl;
                        //cout << "DecodeSourceFlag: "<< DecodeSourceFlag(encodedValue) << endl;
                        //cout << "DecodeProcessID: "<< DecodeProcessID(encodedValue) << endl;
                        processID.push_back(encodedValue);
                        hitID.push_back(hit->get_hit_id());
                        hit_trackID.push_back(hit->get_track_id());
                        detectorID.push_back(hit->get_detector_id());
                        elementID.push_back(hit->get_element_id());
                        tdcTime.push_back(hit->get_tdc_time());
                        driftDistance.push_back(hit->get_drift_distance());
                    }   
                }
            }
		}
	}

	//cout << "reco mode: "<< reco_mode << endl;
	//std::cout << "Size of m_sq_dim_vec: " << m_sq_dim_vec->size() << std::endl;

	if(reco_mode == true){
		ResetRecoBranches();

		for (auto it = m_sq_trk_vec->begin(); it != m_sq_trk_vec->end(); ++it) {
			SRecTrack* trk = dynamic_cast<SRecTrack*>(*it);
			rec_charge.push_back(trk->get_charge());
			rec_vx.push_back(trk->get_pos_vtx().X());
			rec_vy.push_back(trk->get_pos_vtx().Y());
			rec_vz.push_back(trk->get_pos_vtx().Z());

			rec_px.push_back(trk->get_mom_vtx().X());
			rec_py.push_back(trk->get_mom_vtx().Y());
			rec_pz.push_back(trk->get_mom_vtx().Z());

			rec_x_st1.push_back(trk->get_pos_st1().X());
			rec_y_st1.push_back(trk->get_pos_st1().Y());
			rec_z_st1.push_back(trk->get_pos_st1().Z());

			rec_px_st1.push_back(trk->get_mom_st1().X());
			rec_py_st1.push_back(trk->get_mom_st1().Y());
			rec_pz_st1.push_back(trk->get_mom_st1().Z());

			rec_x_st3.push_back(trk->get_pos_st3().X());
			rec_y_st3.push_back(trk->get_pos_st3().Y());
			rec_z_st3.push_back(trk->get_pos_st3().Z());

			rec_px_st3.push_back(trk->get_mom_st3().X());
			rec_py_st3.push_back(trk->get_mom_st3().Y());
			rec_pz_st3.push_back(trk->get_mom_st3().Z());

            rec_chisq_dump.push_back(trk->get_chisq_dump());
            rec_chisq_upstream.push_back(trk->get_chsiq_upstream());
            rec_chisq_target.push_back(trk->getChisqTarget());
		}
        if(reco_dimu_mode==true){
            ResetRecoDimuBranches();
            for (auto it = m_sq_dim_vec->begin(); it != m_sq_dim_vec->end(); it++) {
                SRecDimuon& sdim = dynamic_cast<SRecDimuon&>(**it);
                sdim.calcVariables(1); //eventually need to assign 1 or 2 depending on the target or dump; more at https://github.com/E1039-Collaboration/e1039-core/pull/149
                int trk_id_pos = sdim.get_track_id_pos();
                int trk_id_neg = sdim.get_track_id_neg();
                SRecTrack& trk_pos = dynamic_cast<SRecTrack&>(*(m_sq_trk_vec->at(trk_id_pos))); 
                SRecTrack& trk_neg = dynamic_cast<SRecTrack&>(*(m_sq_trk_vec->at(trk_id_neg))); 
                // Fill dimuon vectors
                // ===== Target hypothesis =====
                sdim.calcVariables(1);  // 1 = target

                // Dimuon (mu+ + mu-)
                TLorentzVector mom_target = sdim.p_pos_target + sdim.p_neg_target;
                dimu_px_tgt.push_back(mom_target.Px());
                dimu_py_tgt.push_back(mom_target.Py());
                dimu_pz_tgt.push_back(mom_target.Pz());
                dimu_mass_tgt.push_back(mom_target.M());

                // μ⁺
                mu_pos_px_tgt.push_back(sdim.p_pos_target.Px());
                mu_pos_py_tgt.push_back(sdim.p_pos_target.Py());
                mu_pos_pz_tgt.push_back(sdim.p_pos_target.Pz());

                // μ⁻
                mu_neg_px_tgt.push_back(sdim.p_neg_target.Px());
                mu_neg_py_tgt.push_back(sdim.p_neg_target.Py());
                mu_neg_pz_tgt.push_back(sdim.p_neg_target.Pz());


                // ===== Dump hypothesis =====
                sdim.calcVariables(2);  // 2 = dump

                // Dimuon (mu+ + mu-)
                TLorentzVector mom_dump = sdim.p_pos_dump + sdim.p_neg_dump;
                dimu_px_dmp.push_back(mom_dump.Px());
                dimu_py_dmp.push_back(mom_dump.Py());
                dimu_pz_dmp.push_back(mom_dump.Pz());
                dimu_mass_dmp.push_back(mom_dump.M());

                // μ⁺
                mu_pos_px_dmp.push_back(sdim.p_pos_dump.Px());
                mu_pos_py_dmp.push_back(sdim.p_pos_dump.Py());
                mu_pos_pz_dmp.push_back(sdim.p_pos_dump.Pz());

                // μ⁻
                mu_neg_px_dmp.push_back(sdim.p_neg_dump.Px());
                mu_neg_py_dmp.push_back(sdim.p_neg_dump.Py());
                mu_neg_pz_dmp.push_back(sdim.p_neg_dump.Pz());

                bool pos_top = m_rs.PosTop()->FindRoad(trk_pos.getTriggerRoad());
                bool pos_bot = m_rs.PosBot()->FindRoad(trk_pos.getTriggerRoad());
                bool neg_top = m_rs.NegTop()->FindRoad(trk_neg.getTriggerRoad());
                bool neg_bot = m_rs.NegBot()->FindRoad(trk_neg.getTriggerRoad());
                bool top_bot_ = pos_top && neg_bot;
                bool bot_top_ =  pos_bot && neg_top;
                top_bot.push_back(top_bot_);
                bot_top.push_back(bot_top_);

            }   
        }
	}
	m_tree->Fill();
	return Fun4AllReturnCodes::EVENT_OK;
}

int DimuAnaRUS::End(PHCompositeNode* startNode)
{
  m_file->cd();
  m_file->Write();
  m_file->Close();  
  return Fun4AllReturnCodes::EVENT_OK;
}


void DimuAnaRUS::ResetHitBranches() {
 	hitID.clear();
	processID.clear();
 	hit_trackID.clear();
	detectorID.clear();
	elementID.clear();
	tdcTime.clear();
	driftDistance.clear();
}
void DimuAnaRUS::ResetTrueBranches() {
	gCharge.clear(); trackID.clear();
	gvx.clear(); gvy.clear(); gvz.clear();
	gpx.clear(); gpy.clear(); gpz.clear();
	gx_st1.clear(); gy_st1.clear(); gz_st1.clear();
	gpx_st1.clear(); gpy_st1.clear(); gpz_st1.clear();
	gx_st3.clear(); gy_st3.clear(); gz_st3.clear();
	gpx_st3.clear(); gpy_st3.clear(); gpz_st3.clear();
}



void DimuAnaRUS::ResetRecoBranches() {
    rec_charge.clear();
    rec_vx.clear(); rec_vy.clear(); rec_vz.clear();
    rec_px.clear(); rec_py.clear(); rec_pz.clear();
    rec_x_st1.clear(); rec_y_st1.clear(); rec_z_st1.clear();
    rec_px_st1.clear(); rec_py_st1.clear(); rec_pz_st1.clear();
    rec_x_st3.clear(); rec_y_st3.clear(); rec_z_st3.clear();
    rec_px_st3.clear(); rec_py_st3.clear(); rec_pz_st3.clear();
    rec_chisq_dump.clear();
    rec_chisq_upstream.clear();
    rec_chisq_target.clear();
}
void DimuAnaRUS::ResetRecoDimuBranches() {

	// Clear dimuon target variables
	dimu_px_tgt.clear();
	dimu_py_tgt.clear();
	dimu_pz_tgt.clear();
	dimu_mass_tgt.clear();

	// Clear dimuon dump variables
	dimu_px_dmp.clear();
	dimu_py_dmp.clear();
	dimu_pz_dmp.clear();
	dimu_mass_dmp.clear();

	// Clear mu+ target variables
	mu_pos_px_tgt.clear();
	mu_pos_py_tgt.clear();
	mu_pos_pz_tgt.clear();

	// Clear mu+ dump variables
	mu_pos_px_dmp.clear();
	mu_pos_py_dmp.clear();
	mu_pos_pz_dmp.clear();

	// Clear mu− target variables
	mu_neg_px_tgt.clear();
	mu_neg_py_tgt.clear();
	mu_neg_pz_tgt.clear();

	// Clear mu− dump variables
	mu_neg_px_dmp.clear();
	mu_neg_py_dmp.clear();
	mu_neg_pz_dmp.clear();
	top_bot.clear();
	bot_top.clear();
}

unsigned int DimuAnaRUS::EncodeProcess(int processID, int sourceFlag) {
        return ( (sourceFlag & 0x3) << 5 ) |  // 2 bits for sourceFlag (1-3)
	                (processID & 0x1F);            // 5 bits for processID (11-24)
			               }

int DimuAnaRUS::DecodeSourceFlag(unsigned int encoded) { 
	return (encoded >> 5) & 0x3;
}
int DimuAnaRUS::DecodeProcessID(unsigned int encoded) {
	return encoded & 0x1F;
}
