# Event Generation and Reconstruction with RUS

This module allows you to perform event generation and reconstruction using the **`Fun4AllSim.C`** macro. Similar to the [SimChainDev module](https://github.com/E1039-Collaboration/e1039-analysis/tree/master/SimChainDev), this script facilitates event simulation, but with an **additional feature**: support for the **RUS file manager** for input and output handling.


## Steps to Run MC Simulations:
1. Clone the repository:
   git clone https://github.com/uva-spin/DimuAnaRUS 

2. **Compile the script** (if you haven’t done so already):
    ```bash
    cd DimuAnaRUS/mc_gen 
    source ../setup.sh    
    cmake-this
    make-this
    ```
3. **Run a test job locally** and check the output files:
    ```bash
    root -b 'Fun4AllSim.C(5)'
    ```
4. **If everything looks correct**, run the job on the Rivanna HPC with a few events:
    ```bash
    ./jobscript.sh test 1 10
    ```
5. **For large-scale submissions**, compute the event processing as explained in the following link:  
   [SpinQuest Monte Carlo Generation on Rivanna](https://confluence.admin.virginia.edu/display/twist/SpinQuest+Monte+Carlo+Generation+on+Rivanna),  
   and submit a job using the following example:
    ```bash
    ./jobscript.sh DYTarget 100 100
    ```
6. **Check your output files** at:
    ```bash
    /sfs/weka/scratch/<user_name>/MC

# RUS File Options in `Fun4AllSim.C`

To configure the RUS file options in `Fun4AllSim.C`, use the following settings:

```cpp
DimuAnaRUS* dimuAna = new DimuAnaRUS();
dimuAna->SetTreeName("tree");          // Set tree name
dimuAna->SetMCTrueMode(true);          // Set to false if true particle info is not needed
dimuAna->SetSaveOnlyDimuon(true);      // Set to false if not saving dimuons
dimuAna->SetRecoMode(true);            // Set to false if reconstruction is not needed
dimuAna->SetOutputFileName("RUS.root");
se->registerSubsystem(dimuAna);

