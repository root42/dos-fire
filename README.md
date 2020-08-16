# dos-fire

Part of my Let's Code MS DOS series.

It’s summer and it’s hot. And what better to code than another classic
demo scene effect: the ubiquitous fire. But we will do it with a
twist. The naive implementation is slow, very slow. Especially on
older 286-486 computers. We will optimize the actual algorithm a bit
and utilize a hidden VGA function that gives us scaling in the
X-direction by a factor of 4 for free.

Needs Turbo C 2.0 and an MS DOS machine with a VGA card (or DOSBox).