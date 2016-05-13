# spam

A set of externals and abstractions for spatialization techniques.

[![Build Status](https://travis-ci.org/pierreguillot/spam.svg?branch=master)](https://travis-ci.org/pierreguillot/spam) [![Build Status](https://ci.appveyor.com/api/projects/status/github/pierreguillot/spam?branch=master&svg=true)](https://ci.appveyor.com/project/pierreguillot/spam)   

Externals:
- spam~: The object loads several instances of a patch.
- spam.in: The inlet object for messages.
- spam.in~: The inlet object for signal.
- spam.out: The outlet object for messages.
- spam.out~: The outlet object for signal.
- spam.connect: The object that creates several connections in one click.
- spam : The all-in-one library

Abstractions:
- mat - matricial spatialization tools
- ylm - high order ambisonics spatialization tools

Installation:
- Download the latest [release](https://github.com/pierreguillot/spam/releases).
- Unzip the file.
- Put the *spam* folder in the [Pure Data library folder](https://puredata.info/docs/faq/how-do-i-install-externals-and-help-files).
- Add spam in the Pure Data's startup window from the menu (optional):
  - Pd/Preferences/Startup...  [New...]

Compilation
- cd build
- cmake ..
- cmake --build .

Author: Pierre Guillot  
Organizations: Université Paris 8 | CICM | Labex Arts H2H
Website: https://github.com/pierreguillot/spam  
License: BSD - see LICENSE
