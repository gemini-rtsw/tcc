The tcc applications are now installed in /gemini/tcc/tcc. So:

	/gemini/tcc/tcc/tsd &

starts the status displays and

	/gemini/tcc/tcc/tcc  &

Similarly, the pointing test and probecal applications are started by:

	/gemini/tcc/tcc/pointtest &

and

	/gemini/tcc/tcc/probecal &

respectively.

To connect to a copy of the tcs with epics records starting with something
other than tcs: - e.g. a simulated tcs running as tc1: - do:

	setenv TCC_TCSNAME tc1

before starting the applications and verify that the print the message 

	using tc1:

as they start.i

You may have to also set the EPICS_CA_ADDR_LIST environment variable if
the IOC running the tcs is not on the same IP sub-network as your workstation.

If you save your own target etc. definitions to a file or have files generated
some other way you can loaded them when you next start the consoles by adding 
a -init option to the command line.
e.g.:

	/gemini/tcc/tcc/tcc -init mytargets.xml &

You can have more than one -init option if you want to read more than one file
or the -init option is a directory then all the files in the directory are
read.

To start the consoles using a previously saved layout use the -layout option:

	/gemini/tcc/tcc/tcc -layout layout.dat &

There is a default layout that is used if there is no -layout option; if 
you want to start with no layout defined put -layout as the last option on
the command line. e.g.:

	/gemini/tcc/tcc/tcc -layout &

There are a number of star catalogues suitable for engineering activities
and pointing tests in /gemini/tcc/tcc/pointcats.

If, when you try to start Skycat from the tcc, using either with the Plot 
button on the preview window or by selecting Skycat on the Windows menu of 
the Target Selector window, you get an error message box that says:

    Error: X server insecure (must use xauth-style 
    authorization); command ignored

you must turn off host based authorisation by doing the following on a
terminal window logged into your workstation:

xhost

this will print either:

    access control disabled, clients can connect from any host

possibly followed by a list of clients (e.g. INET:draco)

or
    access control enabled, only authorized clients can connect

followed by a list of clients.

Then type xhost followed by each of the clients listed with a minus sign 
prepended followed by a minus sign on its own. e.g.

xhost -draco -scott -

Type xhost again and you should get:

    access control enabled, only authorized clients can connect

with no clients listed. Exit from skycat and try again - there is no need to
restart the tcc.

INSTALLATION
------------

To install the tcc, check the code out of cvs, cd to the top level tcc
directory and then either:

	make site=MK
or
	make site=CP

according to which site you are at. This will generate the site-dependent
files, build and install the binary extensions and build the tcl library
indexes.

There are two site dependent files that may need to be edited; calparams.dat
and options. These are generated from calparams.MK or calparams.CP and
options.MK or options.CP respectively depending on the parameter to the 
make command.

"options" defines the labels for the ISS ports on the tsd display and the
names for various science fold mirror positions as used by the A&G - these
must match the A&G software if the tsd is to display the science fold
positions correctly.

"calparams.dat" defines the x/y/z position that the peripheral wave-front
sensors are sent to when the "center" button on the Acquisition and Guiding
screen is pressed and the instrument origin x/y that these positions 
correspond to. The x/y/z position should be set so that the rotary table and
probe arm angles both go to zero; the position will need to be edited 
whenever the probes are re-calibrated.

If changes are made they should be made to the .CP or .MK files and then
copied with the appropriate make commands and the modified files committed
to cvs so that the changes are not lost if a new version of the tcc is
installed.

BUGS
----

If you get a tcl error dialog box (one with with "OK, "Skip Messages" and
"Stack Trace" buttons) you have found a bug I don't know about.  Please 
click on "Stack Trace" and cut and paste the contents of the "Stack Trace 
for Error" box into a file and mail it to me (d.terrett@rl.ac.uk). It is 
helpful if you can resize the window to eliminate the line wrapping before 
cutting the text (this isn't always possible).
