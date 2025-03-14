## Goal
The goal of this `DimuAnaRUS` module is to get the input data from a ROOT file (in DST format) and convert them into RUS (ROOT Universal Structure) format. This includes triggers, hits, true/reconstructed track or dimuon information, and event details. It supports a wide range of data formats such as integers, doubles, and booleans, and can store this data in flat, vector, or array formats.

### Instructions for Adding Reconstructed Variables to RUS File
To include true track or reconstructed variables in your RUS file, follow these steps:

1. **Set the appropriate dimuon road option:**
   - For simulated data, set `SetMCTriggerEmu()` to `true`.
   - For experimental data, set `SetDataTriggerEmu()` to `true`. The `SetDataTriggerEmu()` and `SetMCTriggerEmu()` functions are mutually exclusive, so if you set one to `true`, the other will automatically be set to `false`.
	- These functions ensure that the dimuons are satisfied in the `top-bot` or `bot-top` roads.

2. **Enable reconstruction mode:**
   - Set `SetRecoMode()` to `true` to add the reconstructed variables.
   - Set `SetMCMode()` to `true` to add the MC true variables.

# Updated Variable Names and Types
## Event-Level Variables
| Variable Name      | Type               | Description                          |   
|--------------------|--------------------|--------------------------------------|
| `runID`            | `int`              | Identifier for the current run       |   
| `spillID`          | `int`              | Identifier for the spill in the run  |
| `eventID`          | `int`              | Unique identifier for the event      |   
| `rfID`             | `int`              | Identifier for the RF                |   
| `turnID`           | `int`              | Identifier for the Turn                |   
| `fpgaTrigger`      | `int[5]`           | Array of FPGA trigger                |   
| `nimTrigger`       | `int[5]`           | Array of NIM trigger                 |   
| `rfIntensity`      | `int[33]`          | Array for QIE RF intensities         |   

## Hit-Level Variables
| Variable Name          | Type                     | Description                                  |   
|------------------------|--------------------------|----------------------------------------------|
| `hitID`                | `std::vector<int>`       | Hit IDs for all hits                         |   
| `hit_trackID`          | `std::vector<int>`       | Track IDs for all hits                         |   
| `processID`            | `std::vector<int>`       | Track IDs for all hits                         |   
| `detectorID`           | `std::vector<int>`       | Detector IDs for all hits                    |   
| `elementID`            | `std::vector<int>`       | Element IDs associated with each hit         |   
| `driftDistance`        | `std::vector<double>`    | Drift distances for each hit                 |   
| `tdcTime`              | `std::vector<double>`    | TDC timing values for each hit               |   

## Monte Carlo Truth-Level Variables
| Variable Name  | Type                  | Description                                |
|---------------|----------------------|----------------------------------------------|
| `gCharge`    | `std::vector<int>`    | Charges of the Monte Carlo tracks            |
| `gvx`        | `std::vector<double>` | X-coordinate of the generated track vertex   |
| `gvy`        | `std::vector<double>` | Y-coordinate of the generated track vertex   |
| `gvz`        | `std::vector<double>` | Z-coordinate of the generated track vertex   |
| `gpx`        | `std::vector<double>` | X-component of the momentum at the vertex    |
| `gpy`        | `std::vector<double>` | Y-component of the momentum at the vertex    |
| `gpz`        | `std::vector<double>` | Z-component of the momentum at the vertex    |
| `gx_st1`     | `std::vector<double>` | X-coordinate at station 1                    |
| `gy_st1`     | `std::vector<double>` | Y-coordinate at station 1                    |
| `gz_st1`     | `std::vector<double>` | Z-coordinate at station 1                    |
| `gpx_st1`    | `std::vector<double>` | X-component of momentum at station 1         |
| `gpy_st1`    | `std::vector<double>` | Y-component of momentum at station 1         |
| `gpz_st1`    | `std::vector<double>` | Z-component of momentum at station 1         |
| `gx_st3`     | `std::vector<double>` | X-coordinate at station 3                    |
| `gy_st3`     | `std::vector<double>` | Y-coordinate at station 3                    |
| `gz_st3`     | `std::vector<double>` | Z-coordinate at station 3                    |
| `gpx_st3`    | `std::vector<double>` | X-component of momentum at station 3         |
| `gpy_st3`    | `std::vector<double>` | Y-component of momentum at station 3         |
| `gpz_st3`    | `std::vector<double>` | Z-component of momentum at station 3         |


## Reco-Level Variables
| Variable Name     | Type                  | Description                                       |
|-------------------|-----------------------|--------------------------------------------------|
| `rec_charge`     | `std::vector<int>`     | Charge of the reconstructed particle             |
| `rec_vx`        | `std::vector<double>`  | X-coordinate of the reconstructed vertex         |
| `rec_vy`        | `std::vector<double>`  | Y-coordinate of the reconstructed vertex         |
| `rec_vz`        | `std::vector<double>`  | Z-coordinate of the reconstructed vertex         |
| `rec_px`        | `std::vector<double>`  | X-component of the reconstructed momentum        |
| `rec_py`        | `std::vector<double>`  | Y-component of the reconstructed momentum        |
| `rec_pz`        | `std::vector<double>`  | Z-component of the reconstructed momentum        |
| `rec_x_st1`     | `std::vector<double>`  | X-coordinate at station 1                        |
| `rec_y_st1`     | `std::vector<double>`  | Y-coordinate at station 1                        |
| `rec_z_st1`     | `std::vector<double>`  | Z-coordinate at station 1                        |
| `rec_px_st1`    | `std::vector<double>`  | X-momentum at station 1                          |
| `rec_py_st1`    | `std::vector<double>`  | Y-momentum at station 1                          |
| `rec_pz_st1`    | `std::vector<double>`  | Z-momentum at station 1                          |
| `rec_x_st3`     | `std::vector<double>`  | X-coordinate at station 3                        |
| `rec_y_st3`     | `std::vector<double>`  | Y-coordinate at station 3                        |
| `rec_z_st3`     | `std::vector<double>`  | Z-coordinate at station 3                        |
| `rec_px_st3`    | `std::vector<double>`  | X-momentum at station 3                          |
| `rec_py_st3`    | `std::vector<double>`  | Y-momentum at station 3                          |
| `rec_pz_st3`    | `std::vector<double>`  | Z-momentum at station 3                          |
| `rec_dimu_vx`   | `std::vector<double>`  | X-coordinate of the reconstructed dimuon vertex  |
| `rec_dimu_vy`   | `std::vector<double>`  | Y-coordinate of the reconstructed dimuon vertex  |
| `rec_dimu_vz`   | `std::vector<double>`  | Z-coordinate of the reconstructed dimuon vertex  |
| `rec_dimu_px`   | `std::vector<double>`  | X-component of the reconstructed dimuon momentum |
| `rec_dimu_py`   | `std::vector<double>`  | Y-component of the reconstructed dimuon momentum |
| `rec_dimu_pz`   | `std::vector<double>`  | Z-component of the reconstructed dimuon momentum |
| `rec_dimu_mass` | `std::vector<double>`  | Mass of the reconstructed dimuon system          |
| `rec_dimu_xf`   | `std::vector<double>`  | Xf for the reconstructed dimuon |
| `rec_dimu_x1`   | `std::vector<double>`  | X1 for the reconstructed dimuon |
| `rec_dimu_x2`   | `std::vector<double>`  | X2 for the reconstructed dimuon |


``` Compilation before running the Fun4All macro                                                      
source setup.sh                                                                                       
cmake-this                                                                                            
make-this                                                                                             
```                                                                                                   
### Uses                                                                                              

#Converting DST to RUS
                                                                                                      
1. **Single DST File**                                                                                
   To convert a single DST file to RUS format, you can use the `Fun4Test.C` macro.                    
   Note: This macro is not configured to perform reconstruction or vertexing. Instead, it uses a vertexed DST file for processing into RUS format.  
                                                                                                      
   ```bash                                                                                            
   cd Convert                                                                                         
   root -b Fun4Test.C                                                                                 
                                                                                                      
2. **Processing Multiple DST Files**                                                                  
   If you have many DST files and want to process them locally or on the grid, use the following command:  

   ```bash
   ./gridsub.sh -j 1-5 -g -o

	1. `-j 1-5`: Specifies the range of input DST files to process (from the 1st row to the 5th one).
 	2. `-g`: Enables grid mode for processing.
	3. `-o`: Overwrites existing files.
