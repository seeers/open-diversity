This project provides a simple arduino code to realize a diversity controller with small hardware requirements, which evaluates the video quality of two receivers by counting the number of vertical sync frames per second using a LM1881. The software compares the two video sources and selects the source, which is probably less perturbed.

The initial idea is presented [here](http://fpv-community.de/showthread.php?5699-Diversity-mit-Videoqualit%E4t-statt-RSSI-%28wie-EagleEyes-oder-Oracle%29-selber-bauen).
Hardware design by Daniel. Implementation and refinement by Nachbrenner, Nils, Rangarid.