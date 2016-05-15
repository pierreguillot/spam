# spam

A set of externals and abstractions for sound spatialization.

[![Build Status](https://travis-ci.org/pierreguillot/spam.svg?branch=master)](https://travis-ci.org/pierreguillot/spam) [![Build Status](https://ci.appveyor.com/api/projects/status/github/pierreguillot/spam?branch=master&svg=true)](https://ci.appveyor.com/project/pierreguillot/spam)   

![image](https://cloud.githubusercontent.com/assets/1409918/15268247/bf1d2056-19d8-11e6-8ab3-b1a26e3482a0.png)

#### Externals:
- **spam~**: The object loads several instances of a patch.
- **spam.in**: The inlet object for messages.
- **spam.in~**: The inlet object for signal.
- **spam.out**: The outlet object for messages.
- **spam.out~**: The outlet object for signal.
- **spam.connect**: The object that creates several connections in one click.
- **spam** : The all-in-one library

#### Abstractions:
- **xel** - matricial spatialization tools
- **ylm** - high order ambisonics spatialization tools
- **zpt** - point source spatialization tools

#### Installation:
- Download the latest [release](https://github.com/pierreguillot/spam/releases).
- Unzip the file.
- Put the *spam* folder in the [Pure Data library folder](https://puredata.info/docs/faq/how-do-i-install-externals-and-help-files).
- Add spam in the Pure Data's startup window from the menu (optional):  
  Pd → Preferences → Startup... → [New...] → *spam*

#### Compilation
- cd build
- cmake ..
- cmake --build .

**Author**: Pierre Guillot  
**Organizations**: Université Paris 8 | CICM | Labex Arts H2H   
**Website**: https://github.com/pierreguillot/spam   
**License**: BSD - see LICENSE   
