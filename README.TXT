The tcc applications are now installed in /gemini/tcc/tcc. So:

	/gemini/tcc/tcc/tsd &

starts the status displays and

	/gemini/tcc/tcc/tcc -init /gemini/tcc/tcc/data &

starts the consoles and loads the sample targets, instrument definitions etc.

Similarly, the pointing test and start of night callibration applications are
started by:

	/gemini/tcc/tcc/pointtest &

and

	/gemini/tcc/tcc/pointupdate &

respectively.

To connect to a copy of the tcs with epics records starting with something
other than tcs: - e.g. tc1: - do:

	setenv TCC_TCSNAME tc1:

before starting the applications and verify that the print the message 

	using tc1:

as they start.

If you save your own definitions to a file you can loaded them when you
next start the consoles by adding another -init option to the command line.
e.g.:

	/gemini/tcc/tcc/tcc -init /gemini/tcc/tcc/data -init mystuff.dat &

If the -init option is a directory then all the files in the directory are
read.

To start either the consoles or the status screens using a previously saved
layout (but read the bugs section of this note first) use the -layout option:

	/gemini/tcc/tcc/tsd -layout layout.dat &

There are a number of star catalogues suitable for pointing tests in 
/gemini/tcc/tcc/pointcats.

BUGS
----
1) There is a bug in the version of Skycat that is built into ocswish so that
   the first time you try to plot a field (with the "plot" button on the
   preview window) skycat starts and then an error dialog box pops up with
   the message "can't read "ra_" no such variable". It is then immediately
   covered up by the image server window. To correct the problem, press the
   close button on the image server window, then press the Ok button in the
   error dialog and finally the plot button in the preview window again. It
   will then work ok until you close the main skycat window and exit from the
   skycat application.

If you get a tcl error dialog box (one with with "OK, "Skip Messages" and
"Stack Trace" buttons) in any other circumstances, you have found a bug I
don't know about.  Please click on "Stack Trace" and cut and paste the
contents of the "Stack Trace for Error" box into a file and mail it to me
(d.terrett@rl.ac.uk). It is helpful if you can resize the window to
eliminate the line wrapping before cutting the text (this isn't always
possible).

2) In the "Export" configuration function such that the save file dialog box 
   is the wrong size and you have to resize it to see the directory and file 
   lists properly.

3) "Wave front sensor targets" is missing from the check buttons in the 
   "Export" dialog box. (but this is probably going to change anyway).

4) When a target with no wavelength defined is loaded the undefinition (!)
   of the wavelength isn't propogated to the wavelength editing window.

5) The update button on the preview window re-maps the observation editor
   window.

6) The sub-component apply buttons don't log the command if there is no
   sub-component name.

7) The skycatSearch define target commands don't check that the coodinate
   system object the refer to exists (all the GSC uses FK5/J2000 which is
   defined in the data files in the data subdirectory so this isn't a
   problem for the moment.

8) The consoles sometimes get into a state where all commands sent to the
   TCS time out (after about 2 seconds) although the commands are in fact
   being accepted by the TCS. (This may well be a bug in either the TCS or
   ocswish).

9) The controls for entering temperature etc. on the system control panel
   should be disabled when the weather data source is set to "server".

10) If an object refers to a component that has been deleted, the component
    is ignored without any message when the object is "applied".

11) The pointtest application needs a way of setting the slew options used
    when the "slew to next object" button is pressed.

12) The behaviour of the entry field for setting a custom step size on the
    handsets is confusing and buggy (<Return should "press" the custom 
    button but doesn't).

13) When the model is updated the guide adjustments should be zeroed.

Unconfirmed bugs:

1) The absorb button on the guide handset doesn't work.

Obscure internal bugs:

1) The -font option of the SelectButton widget doesn't work because the
   widget has no components when it is first created.

THINGS STILL TO DO
------------------
The following is a list of things that I know I still have to do (in no 
particular order) so you don't have to tell me that they are missing. Some
of them are obscure internal things so don't worry if you don't understand
what some of them mean.

1) Extend the configurations to include offsets, chopping, filters, solar
   and system objects.

2) Add input validation to all data entry fields. Try daft wavelengths to
   see how the validation works.

3) Add a help menu to each window that sends a URL to a web browser.

4) Add a one line help to the bottom of each of the configuration editing
   windows that displays a message when the mouse is over a data entry field.

5) Put time-to-slew on the preview window.

6) Put the distance from the object to the moon and sun on the preview
   window.

7) Devise some mechanism for "locking" the consoles so that you can't send
   commands to the tcs by accident.

8) Improve the visibilty of the selection highlight.

9) Improve the labeling of the buttons and the titles of the windows (I
   need help on this one).

10) Draw the WFS probe arms properly on skycat so that the vignetting is
    shown and get the geometry correct.

11) Put the object to moon and sun distance on the status display.

12) Implement display of offset information.

13) Implement "next object" on status display.

14) Change the label of the "Set/Slew" button to Set or Slew depending on 
    whether a SlewOptions component has been selected.

15) Do something about management of pointing models.

16) Re-think the way merging of configurations work.

17) Add chopping information to status display.

18) Revise the "export" function. What is required here ? Just save everything
    or something more ?

19) Add a function to load new configuration/component definitions from 
    a file after the tcc has been started.

20) Implement control of nodding.

21) Allow tc1: or tc2. to be controlled instead of tcs:

22) The default directories for the open catalog and open log file
    dialog boxes need to be improved.

23) There needs to be a way of driving tc1: and tc2: as well as tcs:

There is of course more that I don't yet know how to do - acquisition of
guide stars for example - that will have to wait until we know more about
how the telescope will work.
