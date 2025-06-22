# Background Generation using `PHG4E1039TrackPairGen`
`PHG4E1039TrackPairGen` is a combinatoric background event generator built within the Fun4All framework, designed to simulate dimuon events. It supports two configurable modes for generating muon pairs:

- **Random kinematic generation**: Momenta and vertex positions are sampled from flat or Gaussian distributions as defined by the user.
- **Probability-based sampling**: Muon momenta are drawn from a pre-computed 6-dimensional histogram (`THnSparseD`) containing realistic momentum correlations derived from experimental data.

> To enable probability-based generation, set the flag `gen_particle_with_exp_pdf = true`.
>**Note:** This mode is under active development. Please validate your output distributions before use.
In random mode, each muon's momentum can be independently defined through user-specified 3D ranges:

```cpp
comb->set_par1_pxpypz_range(-6.0, 6.0, -4.0, 4.0, 5.0, 100.0);  // mu+
comb->set_par2_pxpypz_range(-6.0, 6.0, -4.0, 4.0, 5.0, 100.0);  // mu-
```

To restrict the opening angle between the generated muon pair, use:
```cpp
comb->set_max_opening_angle(10);  // angle in degrees
```

## Running the Macro

Running the Fun4All macro is the same as described here:
[https://github.com/uva-spin/DimuAnaRUS/tree/main/mc_gen](https://github.com/uva-spin/DimuAnaRUS/tree/main/mc_gen)
