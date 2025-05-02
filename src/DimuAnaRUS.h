#ifndef _DIMU_ANA_RUS_H_
#define _DIMU_ANA_RUS_H_
#include <fun4all/SubsysReco.h>
#include <UtilAna/TrigRoadset.h>

class TFile;
class TTree;
class SQRun;
class SQSpillMap;
class SQEvent;
class SQHitVector;
class SQTrackVector;
class SQDimuonVector;

class DimuAnaRUS: public SubsysReco {
	SQEvent* m_evt;
	SQSpillMap* m_sp_map;
	SQHitVector* m_hit_vec;
	SQTrackVector * m_vec_trk;
	SQTrackVector*  m_sq_trk_vec;
	SQDimuonVector* m_sq_dim_vec;
	SQDimuonVector* m_true_vec_dim;
	SQRun* sq_run;

	std::string m_file_name;
	std::string m_tree_name;
	TFile*      m_file;
	TTree*      m_tree;
	public:

	UtilTrigger::TrigRoadset m_rs;
	bool true_mode;
	bool reco_mode;
	bool data_trig_mode;
	bool mc_trig_mode;
	bool saveDimuonOnly;
	bool true_dimu_mode;
	int process_id;
	int source_flag;

	void SetProcessId(int proc_id) { process_id = proc_id; }
	void SetSourceFlag(int src_flag) { source_flag = src_flag; }
	void SetMCTrueMode(bool enable) { true_mode = enable; }
	void SetMCDimuonMode(bool enable) { true_dimu_mode = enable; }
	void SetRecoMode(bool enable) { reco_mode = enable; }
	void SetDataTriggerEmu(bool enable) { 
		data_trig_mode = enable; 
		mc_trig_mode = !enable; // Automatically disable mc_trig_mode when data_trig_mode is enabled
	}   

	void SetMCTriggerEmu(bool enable) { 
		mc_trig_mode = enable; 
		data_trig_mode = !enable; // Automatically disable data_trig_mode when mc_trig_mode is enabled
	}   

	void SetSaveOnlyDimuon(bool enable) { saveDimuonOnly = enable; }

	unsigned int EncodeProcess(int processID, int sourceFlag);
	static int DecodeSourceFlag(unsigned int encoded);
	static int DecodeProcessID(unsigned int encoded);

	DimuAnaRUS(const std::string& name="DimuAnaRUS");
	virtual ~DimuAnaRUS();
	int Init(PHCompositeNode *startNode);
	int InitRun(PHCompositeNode *startNode);
	int process_event(PHCompositeNode *startNode);
	int End(PHCompositeNode *startNode);
	void SetOutputFileName(const std::string name) { m_file_name = name; }

	void SetTreeName(const std::string& name) { m_tree_name = name; }
	void SetFileName(const std::string& name) { m_file_name = name; }
	void ResetHitBranches();
	void ResetTrueBranches();
	void ResetTrackRecoBranches();
	void ResetDimuRecoBranches();

	private:

	// Scalar variables
	int eventID;
	/*
	int runID;
	int spillID;
	int eventID;
	int rfID;
	int turnID;
	int rfIntensity[33];
	int fpgaTrigger[5] = {0};
	int nimTrigger[5] = {0};
	*/

	std::vector<int> hitID;
	std::vector<int> processID;
	std::vector<int> hit_trackID;
	std::vector<int> detectorID;
	std::vector<int> elementID;
	std::vector<double> tdcTime;
	std::vector<double> driftDistance;
	//std::vector<bool> hitsInTime;
	// True track data
	std::vector<int> gCharge;
	//std::vector<int> sourceFlag;
	std::vector<int> trackID;
	std::vector<double> gvx;
	std::vector<double> gvy;
	std::vector<double> gvz;
	std::vector<double> gpx;
	std::vector<double> gpy;
	std::vector<double> gpz;

	std::vector<double> gx_st1, gy_st1, gz_st1;
	std::vector<double> gpx_st1, gpy_st1, gpz_st1;
	std::vector<double> gx_st3, gy_st3, gz_st3;
	std::vector<double> gpx_st3, gpy_st3, gpz_st3;

	std::vector<double> rec_dimu_vx;
        std::vector<double> rec_dimu_vy;
        std::vector<double> rec_dimu_vz;
        std::vector<double> rec_dimu_px;
        std::vector<double> rec_dimu_py;
        std::vector<double> rec_dimu_pz;
        std::vector<double> rec_dimu_mass;
        std::vector<double> rec_dimu_x1;
        std::vector<double> rec_dimu_x2;
        std::vector<double> rec_dimu_xf;
	//std::vector<bool> top_bot;
	//std::vector<bool> bot_top;

	std::vector<int> rec_charge;
	std::vector<double> rec_vx, rec_vy, rec_vz;
	std::vector<double> rec_px, rec_py, rec_pz;
	std::vector<double> rec_x_st1, rec_y_st1, rec_z_st1;
	std::vector<double> rec_px_st1, rec_py_st1, rec_pz_st1;
	std::vector<double> rec_x_st3, rec_y_st3, rec_z_st3;
	std::vector<double> rec_px_st3, rec_py_st3, rec_pz_st3;
};

#endif // _DimuAnaRUS.h_
