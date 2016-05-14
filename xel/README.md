# mat

A set of abstractions for sound spatialization in Pure Data based on a matricial representation of sound field.

- **xel.2d.syn.brush~**: The abstraction synthesizes the signals of the cells of a matrix for a sound source using amplitude gains depending on coordinates.
- **xel.2d.syn.bucket~**: The abstraction synthesizes the signals of the cells of a matrix for a sound source using variable delay lines without flanging effect.
- **xel.2d.syn.spray~**: The abstraction synthesizes the signals of the cells of a matrix for a sound source using a quasi-synchronous granular synthesizer (QSGS).
- **xel.2d.fx.eraser~**: The abstraction decreases the amplitude of the signals of a set of the cells of a matrix.
- **xel.2d.scope~**: The abstraction displays the rms amplitude of the signals of the cells of a matrix inside a canvas.
- **xel.2d.meter~**: The abstraction displays the contributions of a set of loudspeakers in a space.
- **xel.2d.projector.dbap~**: The abstraction projects the signals of the cells of a matrix onto a set of loudspeakers using distance based amplitude panning (DBAP).

**Author**: Pierre Guillot  
**Organizations**: Université Paris 8 | CICM | Labex Arts H2H   
**Website**: https://github.com/pierreguillot/spam
