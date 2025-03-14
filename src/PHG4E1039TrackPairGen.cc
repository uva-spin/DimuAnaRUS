#include "PHG4E1039TrackPairGen.h"
#include <g4main/PHG4Particlev2.h>
#include <g4main/PHG4InEvent.h>
#include "g4main/PHG4VtxPoint.h"
#include "g4main/PHG4TruthInfoContainer.h"

#include <fun4all/Fun4AllReturnCodes.h>
#include <phool/getClass.h>
#include <phool/recoConsts.h>
#include <phool/PHCompositeNode.h>
#include <phool/PHIODataNode.h>
#include <interface_main/SQEvent_v1.h>
#include <interface_main/SQMCEvent_v1.h>

#include <Geant4/G4ParticleTable.hh>
#include <Geant4/G4ParticleDefinition.hh>

#include <gsl/gsl_randist.h>

#include <cstdlib>
#include <cmath>
#include <cassert>

#include <E906LegacyVtxGen/SQPrimaryVertexGen.h>

using namespace std;

PHG4E1039TrackPairGen::PHG4E1039TrackPairGen(const string &name): 
  PHG4ParticleGeneratorBase(name),
  _particle_codes(),
  _particle_names(),
  _vertex_func_x(Uniform),
  _vertex_func_y(Uniform),
  _vertex_func_z(Uniform),
  _t0(0.0),
  _vertex_x(0.0),
  _vertex_y(0.0),
  _vertex_z(0.0),
  _vertex_width_x(0.0),
  _vertex_width_y(0.0),
  _vertex_width_z(0.0),
  _vertex_offset_x(0.0),
  _vertex_offset_y(0.0),
  _vertex_offset_z(0.0),
  _vertex_size_func_r(Uniform),
  _vertex_size_mean(0.0),
  _vertex_size_width(0.0),
  _eta_min(-1.25),
  _eta_max(1.25),
  _phi_min(-M_PI),
  _phi_max(M_PI),
  _pt_min(0.0),
  _pt_max(5.0),
  _pt_gaus_width(0.0),
  _p_min(0.0),
  _p_max(NAN),
  _p_gaus_width(NAN),
  _px_min(NAN), _px_max(NAN),
  _py_min(NAN), _py_max(NAN),
  _pz_min(NAN), _pz_max(NAN),
  _theta_max(NAN),
  _px_par1_min(-6.0),
  _py_par1_min(-4.0),
  _pz_par1_min(10.0),
  _px_par1_max(6.0),
  _py_par1_max(4.0),
  _pz_par1_max(100.0),
  _px_par2_min(-6.0),
  _py_par2_min(-4.0),
  _pz_par2_min(10.0),
  _px_par2_max(6.0),
  _py_par2_max(4.0),
  _pz_par2_max(100.0),
  _eventcount(0),
  _ineve(NULL), 
  _evt(NULL),
  _mcevt(NULL),
  _legacy_vertexgenerator(false)
{

	_vertexGen = new SQPrimaryVertexGen();
	return;
}

void PHG4E1039TrackPairGen::add_particles(const std::string &name, const unsigned int num) {
  _particle_names.push_back(std::make_pair(name,num));
  return;
}

void PHG4E1039TrackPairGen::add_particles(const int pid, const unsigned int num) {
  _particle_codes.push_back(std::make_pair(pid,num));
  return;
}

void PHG4E1039TrackPairGen::set_t0(const double t0) {
  _t0 = t0;
  return;
}

void PHG4E1039TrackPairGen::set_eta_range(const double min, const double max) {
  if (min > max)
    {
      cout << "not setting eta bc etamin " << min << " > etamax: " << max << endl;
      return;
    }
  _eta_min = min;
  _eta_max = max;
  return;
}

void PHG4E1039TrackPairGen::set_phi_range(const double min, const double max) {
  if (min > max)
    {
      cout << "not setting phi bc phimin " << min << " > phimax: " << max << endl;
      return;
    }
  _phi_min = min;
  _phi_max = max;
  return;
}

void PHG4E1039TrackPairGen::set_pt_range(const double min, const double max, const double pt_gaus_width) {
  if (min > max)
    {
      cout << "not setting pt bc ptmin " << min << " > ptmax: " << max << endl;
      return;
    }
  assert(pt_gaus_width >=0 );

  _pt_min = min;
  _pt_max = max;
  _pt_gaus_width = pt_gaus_width;
  _p_min = NAN;
  _p_max = NAN;
  _p_gaus_width = NAN;
  return;
}

void PHG4E1039TrackPairGen::set_p_range(const double min, const double max, const double p_gaus_width) {
  if (min > max)
    {
      cout << "not setting p bc ptmin " << min << " > ptmax: " << max << endl;
      return;
    }
  assert(p_gaus_width >=0 );
  _pt_min = NAN;
  _pt_max = NAN;
  _pt_gaus_width = NAN;
  _p_min = min;
  _p_max = max;
  _p_gaus_width = p_gaus_width;
  return;
}

void PHG4E1039TrackPairGen::set_vertex_distribution_function(FUNCTION x, FUNCTION y, FUNCTION z) {
  _vertex_func_x = x;
  _vertex_func_y = y;
  _vertex_func_z = z;
  return;
}

void PHG4E1039TrackPairGen::set_vertex_distribution_mean(const double x, const double y, const double z) {
  _vertex_x = x;
  _vertex_y = y;
  _vertex_z = z;
  return;
}

void PHG4E1039TrackPairGen::set_vertex_distribution_width(const double x, const double y, const double z) {
  _vertex_width_x = x;
  _vertex_width_y = y;
  _vertex_width_z = z;
  return;
}

void PHG4E1039TrackPairGen::set_existing_vertex_offset_vector(const double x, const double y, const double z) {
  _vertex_offset_x = x;
  _vertex_offset_y = y;
  _vertex_offset_z = z;
  return;
}

void PHG4E1039TrackPairGen::set_vertex_size_function(FUNCTION r) {
  _vertex_size_func_r = r;
  return;
}

void PHG4E1039TrackPairGen::set_vertex_size_parameters(const double mean, const double width) {
  _vertex_size_mean = mean;
  _vertex_size_width = width;
  return;
}

int PHG4E1039TrackPairGen::InitRun(PHCompositeNode *topNode) {

  if ((_vertex_func_x != Uniform)&&(_vertex_func_x != Gaus)) {
    cout << PHWHERE << "::Error - unknown vertex distribution function requested" << endl;
    return Fun4AllReturnCodes::ABORTRUN;
  }
  if ((_vertex_func_y != Uniform)&&(_vertex_func_y != Gaus)) {
    cout << PHWHERE << "::Error - unknown vertex distribution function requested" << endl;
    return Fun4AllReturnCodes::ABORTRUN;
  }
  if ((_vertex_func_z != Uniform)&&(_vertex_func_z != Gaus)) {
    cout << PHWHERE << "::Error - unknown vertex distribution function requested" << endl;
    return Fun4AllReturnCodes::ABORTRUN;
  }

  PHNodeIterator iter( topNode );
  PHCompositeNode *dstNode = dynamic_cast<PHCompositeNode*>(iter.findFirst("PHCompositeNode", "DST"));
  if (!dstNode) {
    cout << PHWHERE << "DST Node missing.  ABORTRUN." << endl;
    return Fun4AllReturnCodes::ABORTRUN;
  }
  _ineve = findNode::getClass<PHG4InEvent>(topNode, "PHG4INEVENT");
  if (!_ineve) {
    _ineve = new PHG4InEvent();
    dstNode->addNode(new PHIODataNode<PHObject>(_ineve, "PHG4INEVENT", "PHObject"));
  }
  _evt = findNode::getClass<SQEvent>(topNode, "SQEvent");
  if (!_evt) {
    _evt = new SQEvent_v1();
    dstNode->addNode(new PHIODataNode<PHObject>(_evt, "SQEvent", "PHObject"));
  }
  _mcevt = findNode::getClass<SQMCEvent>(topNode, "SQMCEvent");
  if (!_mcevt) {
    _mcevt = new SQMCEvent_v1();
    dstNode->addNode(new PHIODataNode<PHObject>(_mcevt, "SQMCEvent", "PHObject"));
  }

  if (verbosity > 0) {
    cout << "================ PHG4E1039TrackPairGen::InitRun() ======================" << endl;
    cout << " Random seed = " << get_seed() << endl;
    cout << " Particles:" << endl;
    for (unsigned int i=0; i<_particle_codes.size(); ++i) {
      cout << "    " << _particle_codes[i].first << ", count = " << _particle_codes[i].second << endl;
    }
    for (unsigned int i=0; i<_particle_names.size(); ++i) {
      cout << "    " << _particle_names[i].first << ", count = " << _particle_names[i].second << endl;
    }
    if (get_reuse_existing_vertex()) {
      cout << " Vertex Distribution: Set to reuse a previously generated sim vertex" << endl;
      cout << " Vertex offset vector (x,y,z) = (" << _vertex_offset_x << ","<< _vertex_offset_y << ","<< _vertex_offset_z << ")" << endl;
    } else {
      cout << " Vertex Distribution Function (x,y,z) = ("; 
      if (_vertex_func_x == Uniform) cout << "Uniform,";
      else if (_vertex_func_x == Gaus) cout << "Gaus,";
      if (_vertex_func_y == Uniform) cout << "Uniform,";
      else if (_vertex_func_y == Gaus) cout << "Gaus,";
      if (_vertex_func_z == Uniform) cout << "Uniform";
      else if (_vertex_func_z == Gaus) cout << "Gaus";
      cout << ")" << endl;
      cout << " Vertex mean (x,y,z) = (" << _vertex_x << ","<< _vertex_y << ","<< _vertex_z << ")" << endl;
      cout << " Vertex width (x,y,z) = (" << _vertex_width_x << ","<< _vertex_width_y << ","<< _vertex_width_z << ")" << endl;
    }
    cout << " Vertex size function (r) = (";
    if (_vertex_size_func_r == Uniform) cout << "Uniform";
    else if (_vertex_size_func_r == Gaus) cout << "Gaus";
    cout << ")" << endl;
    cout << " Vertex size (mean) = (" << _vertex_size_mean << ")" << endl;
    cout << " Vertex size (width) = (" << _vertex_size_width << ")" << endl;
    cout << " Eta range = " << _eta_min << " - " << _eta_max << endl;
    cout << " Phi range = " << _phi_min << " - " << _phi_max << endl;
    cout << " pT range = " << _pt_min << " - " << _pt_max << endl;
    cout << " t0 = " << _t0 << endl;
    cout << "===========================================================================" << endl;
  }

  // the definition table should be filled now, so convert codes into names
  for (unsigned int i=0;i<_particle_codes.size();++i) {
    int pdgcode = _particle_codes[i].first;
    unsigned int count = _particle_codes[i].second;
    string pdgname = get_pdgname(pdgcode);
    _particle_names.push_back(make_pair(pdgname,count));
  }
  _vertexGen->InitRun(topNode);//abi

  recoConsts* rc = recoConsts::instance();
  ostringstream oss;
  for (unsigned int ii = 0; ii < _particle_names.size(); ++ii) {
    string      name = _particle_names[ii].first;
    unsigned int num = _particle_names[ii].second;
    if (ii > 0) oss << ":";
    oss << name << "*" << num;
  }
  rc->set_CharFlag("PHG4SEG_"+Name()+"_PARTICLES", oss.str());

  if(! std::isnan(_px_min)) {
    rc->set_DoubleFlag("PHG4SEG_"+Name()+"_PX_MIN", _px_min);
    rc->set_DoubleFlag("PHG4SEG_"+Name()+"_PX_MAX", _px_max);
    rc->set_DoubleFlag("PHG4SEG_"+Name()+"_PY_MIN", _py_min);
    rc->set_DoubleFlag("PHG4SEG_"+Name()+"_PY_MAX", _py_max);
    rc->set_DoubleFlag("PHG4SEG_"+Name()+"_PZ_MIN", _pz_min);
    rc->set_DoubleFlag("PHG4SEG_"+Name()+"_PZ_MAX", _pz_max);
  }
  if (! _legacy_vertexgenerator) {
    rc->set_IntFlag   ("PHG4SEG_"+Name()+"_VTX_FUNC_X"  , _vertex_func_x);
    rc->set_IntFlag   ("PHG4SEG_"+Name()+"_VTX_FUNC_Y"  , _vertex_func_y);
    rc->set_IntFlag   ("PHG4SEG_"+Name()+"_VTX_FUNC_Z"  , _vertex_func_z);
    rc->set_DoubleFlag("PHG4SEG_"+Name()+"_VTX_X"       , _vertex_x);
    rc->set_DoubleFlag("PHG4SEG_"+Name()+"_VTX_Y"       , _vertex_y);
    rc->set_DoubleFlag("PHG4SEG_"+Name()+"_VTX_Z"       , _vertex_z);
    rc->set_DoubleFlag("PHG4SEG_"+Name()+"_VTX_WIDTH_X" , _vertex_width_x);
    rc->set_DoubleFlag("PHG4SEG_"+Name()+"_VTX_WIDTH_Y" , _vertex_width_y);
    rc->set_DoubleFlag("PHG4SEG_"+Name()+"_VTX_WIDTH_Z" , _vertex_width_z);
    rc->set_DoubleFlag("PHG4SEG_"+Name()+"_VTX_OFFSET_X", _vertex_offset_x);
    rc->set_DoubleFlag("PHG4SEG_"+Name()+"_VTX_OFFSET_Y", _vertex_offset_y);
    rc->set_DoubleFlag("PHG4SEG_"+Name()+"_VTX_OFFSET_Z", _vertex_offset_z);
  }

  return Fun4AllReturnCodes::EVENT_OK;
}


int PHG4E1039TrackPairGen::process_event(PHCompositeNode *topNode) {
  if (verbosity > 0) {
    cout << "====================== PHG4SimpleEventGenerator::process_event() =====================" << endl;
    cout <<"PHG4SimpleEventGenerator::process_event - reuse_existing_vertex = "<<reuse_existing_vertex<<endl;
  }
  int vtxindex = -1;
  int trackid = -1;

  TLorentzVector muon1;
  TLorentzVector muon2;
  int muon_counter=0;
  if(! _particle_names.size() ==2) return Fun4AllReturnCodes::ABORTRUN;
  
  for (unsigned int i=0; i<_particle_names.size(); ++i) {
	  muon_counter++;

	  if (_legacy_vertexgenerator) {
		  TVector3 vtx = _vertexGen->generateVertex();
		  vtx_x = vtx.X();        
		  vtx_y = vtx.Y();
		  vtx_z = vtx.Z();
	  } else if (!ReuseExistingVertex(topNode)) { // vtx_x, vtx_y and vtx_z are doubles from the base class. common methods modify those, please no private copies. at some point we might rely on them being up to date.
		  // generate a new vertex point
		  vtx_x = smearvtx(_vertex_x,_vertex_width_x,_vertex_func_x);
		  vtx_y = smearvtx(_vertex_y,_vertex_width_y,_vertex_func_y);
		  vtx_z = smearvtx(_vertex_z,_vertex_width_z,_vertex_func_z);
	  } 

	  vtx_x += _vertex_offset_x;
	  vtx_y += _vertex_offset_y;
	  vtx_z += _vertex_offset_z;

	  if (verbosity > 0) {
		  cout <<"PHG4SimpleEventGenerator::process_event - vertex center"<<reuse_existing_vertex
			  << vtx_x<<", "<< vtx_y<<", "<< vtx_z<<" cm"
			  <<endl;
	  }
	  string pdgname = _particle_names[i].first;
	  int pdgcode = get_pdgcode(pdgname);
	  unsigned int nparticles = _particle_names[i].second;
	  for (unsigned int j=0; j<nparticles; ++j) {

		  if ((_vertex_size_width > 0.0)||(_vertex_size_mean != 0.0)) {
			  double r = smearvtx(_vertex_size_mean,_vertex_size_width,_vertex_size_func_r);
			  double x = 0.0;
			  double y = 0.0;
			  double z = 0.0;
			  gsl_ran_dir_3d(RandomGenerator,&x,&y,&z);
			  x *= r;
			  y *= r;
			  z *= r;

			  vtxindex = _ineve->AddVtx(vtx_x+x,vtx_y+y,vtx_z+z,_t0);
		  } else if ((i==0)&&(j==0)) {
			  vtxindex = _ineve->AddVtx(vtx_x,vtx_y,vtx_z,_t0);
		  }

		  ++trackid;

		  double px = std::numeric_limits<double>::max();
		  double py = std::numeric_limits<double>::max();
		  double pz = std::numeric_limits<double>::max();
		  double angle;

		  int iteration = 0;
		  const int max_iterations = 100;
		  if (muon_counter == 1) {
		  	vtxindex = _ineve->AddVtx(vtx_x,vtx_y,vtx_z,_t0);
			 if (verbosity > 0) cout << "Mu+ Vertex (first iteration): (" << vtx_x << ", " << vtx_y << ", " << vtx_z << ")" << endl;
		  	do {

			if (verbosity > 0) cout << "_pz_par1_min: "<< _pz_par1_min << "_pz_par1_max: " << _pz_par1_max << endl;

			  px = (_px_par1_max - _px_par1_min) * gsl_rng_uniform_pos(RandomGenerator) + _px_par1_min;
			  py = (_py_par1_max - _py_par1_min) * gsl_rng_uniform_pos(RandomGenerator) + _py_par1_min;
			  pz = (_pz_par1_max - _pz_par1_min) * gsl_rng_uniform_pos(RandomGenerator) + _pz_par1_min;


			  if (verbosity > 0) cout << "Mu- Vertex P (1St iteration): (" << px << ", " << py << ", " << pz << ")" << endl;

			  muon1.SetXYZM(px, py, pz, 0.1056);
			  iteration++;
			}while (!(muon1.Pt() > _pt_min && muon1.Pt() < _pt_max && iteration < max_iterations));
			//cout << "mu+  first iterations: "<< iteration <<endl; 
		  }

		  if((iteration==100))return Fun4AllReturnCodes::ABORTEVENT;

		  if (muon_counter == 2) {
		  	    vtxindex = _ineve->AddVtx(vtx_x,vtx_y,vtx_z,_t0);
			  double angle, xF;
			  do {
			if (verbosity > 0) {
				cout << "_pz_par2_min: "<< _pz_par2_min << "_pz_par2_max: " << _pz_par2_max << endl;
				cout << "Mu- Vertex (second iteration): (" << vtx_x << ", " << vtx_y << ", " << vtx_z << ")" << endl;
				cout << "Mu- Vertex P (second iteration): (" << px << ", " << py << ", " << pz << ")" << endl;
			   }
                          px = (_px_par2_max - _px_par2_min) * gsl_rng_uniform_pos(RandomGenerator) + _px_par2_min;
                          py = (_py_par2_max - _py_par2_min) * gsl_rng_uniform_pos(RandomGenerator) + _py_par2_min;
                          pz = (_pz_par2_max - _pz_par2_min) * gsl_rng_uniform_pos(RandomGenerator) + _pz_par2_min;

				  muon2.SetXYZM(px, py, pz, 0.1056); 
				  angle = muon1.Vect().Angle(muon2.Vect()) * (180.0 / M_PI);  // Convert from radians to degrees
				  Double_t mp = 0.938;
				  Double_t ebeam = 120.;
				  TLorentzVector p_beam(0., 0., sqrt(ebeam * ebeam - mp * mp), ebeam);
				  TLorentzVector p_target(0., 0., 0., mp);
				  TLorentzVector p_cms = p_beam + p_target;
				  TLorentzVector p_sum = muon1 + muon2;
				  double mass = p_sum.M();
				  Double_t s = p_cms.M2();
				  TVector3 bv_cms = p_cms.BoostVector();
				  p_sum.Boost(-bv_cms);
				  xF = 2. * p_sum.Pz() / TMath::Sqrt(s) / (1. - mass * mass / s);
				  }while (!((muon2.Pt() >= _pt_min && muon2.Pt() <= _pt_max) && angle < _theta_max && xF < 1.0) && iteration < max_iterations);
			  	//cout <<"iterations mu second track: "<< iteration <<"pt of the muon2: "<< muon2.Pt() << "angle: "<< angle <<endl;
			  iteration=0;
		  }
		  if((iteration==100))return Fun4AllReturnCodes::ABORTEVENT;
		  iteration=0;
		  if (verbosity > 0 &&  muon_counter == 2){
			  cout << "angle of the muons: "<< angle << endl;
			  cout << "px 1: " << muon1.Px() << endl;
			  cout << "py 1: " << muon1.Py() << endl;
			  cout << "pz 1: " << muon1.Pz() << endl;
			  cout << "px 2: " << muon2.Px() << endl;
			  cout << "py 2: " << muon2.Py() << endl;
			  cout << "pz 2: " << muon2.Pz() << endl;
		  }

		  double m = get_mass(pdgcode);
		  double e = sqrt(px*px+py*py+pz*pz+m*m);
	
		  //cout << "pdg code: "<< pdgcode << endl;
		  PHG4Particle *particle = new PHG4Particlev2();
		  particle->set_track_id(trackid);
		  particle->set_vtx_id(vtxindex);
		  particle->set_parent_id(0);
		  particle->set_name(pdgname);
		  particle->set_pid(pdgcode);
		  particle->set_px(px);
		  particle->set_py(py);
		  particle->set_pz(pz);
		  particle->set_e(e);

		  _ineve->AddParticle(vtxindex, particle);
		  if (embedflag != 0) _ineve->AddEmbeddedParticle(particle,embedflag);
	  }
  }

  _evt->set_run_id  (0);
  _evt->set_spill_id(0);
  _evt->set_event_id(++_eventcount);
  _mcevt->set_cross_section(0.0);
  _mcevt->set_weight       (1.0);

  if (verbosity > 0) {
	  _ineve->identify();
	  cout << "======================================================================================" << endl;
  } 

  return Fun4AllReturnCodes::EVENT_OK;
}


int PHG4E1039TrackPairGen::End(PHCompositeNode *topNode)
{
  recoConsts* rc = recoConsts::instance();
  rc->set_IntFlag("PHG4SEG_EVENT_COUNT", _eventcount); // Without "Name()" here to make this variable unique

  return Fun4AllReturnCodes::EVENT_OK;
}

void PHG4E1039TrackPairGen::set_pxpypz_range(const double x_min,
		const double x_max, const double y_min, const double y_max,
		const double z_min, const double z_max) {
	_px_min = x_min; _px_max = x_max;
	_py_min = y_min; _py_max = y_max;
	_pz_min = z_min; _pz_max = z_max;
}

void PHG4E1039TrackPairGen::set_par1_pxpypz_range(const double x_min,
                const double x_max, const double y_min, const double y_max,
                const double z_min, const double z_max) {
        _px_par1_min = x_min; _px_par1_max = x_max;
        _py_par1_min = y_min; _py_par1_max = y_max;
        _pz_par1_min = z_min; _pz_par1_max = z_max;
}

void PHG4E1039TrackPairGen::set_par2_pxpypz_range(const double x_min,
                const double x_max, const double y_min, const double y_max,
                const double z_min, const double z_max) {
        _px_par2_min = x_min; _px_par2_max = x_max;
        _py_par2_min = y_min; _py_par2_max = y_max;
        _pz_par2_min = z_min; _pz_par2_max = z_max;
}


void PHG4E1039TrackPairGen::set_max_opening_angle(const double x_max){
         _theta_max = x_max;
}


double
PHG4E1039TrackPairGen::smearvtx(const double position, const double width, FUNCTION dist) const
{
  double res = position;
  if (dist == Uniform)
    {
      res = (position-width) + 2*gsl_rng_uniform_pos(RandomGenerator)*width;
    }
  else if (dist == Gaus)
    {
      res = position + gsl_ran_gaussian(RandomGenerator,width);
    }
  return res;
}
