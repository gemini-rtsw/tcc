# This file is for Cerro Pachon

# The name of the OCS session soap server.
set ocs_proxy http://gsodb.cl.gemini.edu:9000/wdba/services

# Where the tcs is stored.
set tcs_root_dir /gemini/epics3.13.4/tcs/tcs

# The root of the directory in when WFS images are stored.
set m1_tune_dir /gemini/epics3.13.4/tcs/tcs

# The default directory for storing HRWFS images.
#set default_wfs_dir /net/enif/export/home/wfs

# The directory in which HRWFS sky background images are stored.
set hrwfs_sky_dir /home/SSA/sky

# The name of the script that starts the HRWFS AO program.
set hrwfs_ao_prog /home/gemvx/bin/StartHrwfsAO

# The x/y/z position that corresponds to the A&G PWFS1 probeSel center
# command.

set pwfs1_center_x   0.101
set pwfs1_center_y -15.076
set pwfs1_center_z   0.0

# The x/y/z position that corresponds to the A&G PWFS2 probeSel center
# command.

set pwfs2_center_x -2.216
set pwfs2_center_y -2.760
set pwfs2_center_z  0.0

# The center position for the Acquisition camera window
set acqcam_x 479
set acqcam_y 513

