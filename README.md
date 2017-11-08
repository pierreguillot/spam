# spam

A set of externals and abstractions for sound spatialization.

![image](https://cloud.githubusercontent.com/assets/1409918/15268247/bf1d2056-19d8-11e6-8ab3-b1a26e3482a0.png)

#### Externals:
- **spam~**: The object loads several instances of a patch.
- **spam.in**: The inlet object for messages.
- **spam.in~**: The inlet object for signal.
- **spam.out**: The outlet object for messages.
- **spam.out~**: The outlet object for signal.
- **spam.connect**: The object that creates several connections in one click.
- **spam** : The all-in-one library

#### Spatialization:
- **soxel** - matrix based spatialization tools
- **ylm** - high order ambisonics spatialization tools
- **zpt** - point source spatialization tools

#### Installation:
- Download the [![Release](https://img.shields.io/github/release/pierreguillot/spam.svg)](https://github.com/pierreguillot/spam/releases/latest) .
- Unzip the file.
- Put the *spam* folder in the [Pure Data library folder](https://puredata.info/docs/faq/how-do-i-install-externals-and-help-files).
- Add *spam* in the Pure Data's startup window from the menu (optional):  
  `Pd → Preferences → Startup... → [New...] → spam`

#### Compilation

[![Travis](https://img.shields.io/travis/pierreguillot/spam.svg?label=travis)](https://travis-ci.org/pierreguillot/spam)  
[![Appveyor](https://img.shields.io/appveyor/ci/pierreguillot/spam.svg?label=appveyor)](https://ci.appveyor.com/project/pierreguillot/spam)
```
mkdir build
cd build
cmake ..
cmake --build .
```

**Author**: Pierre Guillot  
**Organizations**: Université Paris 8 | CICM | Labex Arts H2H   
**Website**: https://github.com/pierreguillot/spam    

[![license](https://img.shields.io/github/license/pierreguillot/spam.svg?maxAge=2592000)](https://github.com/pierreguillot/spam/blob/master/LICENSE)
