# mat

A set of abstractions for sound spatialization in Pure Data based on a matricial representation of sound field.

* mat.2d.encoder~
* mat.2d.scope~

ToDo List:

* mat.2d.decoder~
* mat.2d.vbap~ (for decoder)
* clean the scope and add colors and line width properties and use env~ instead of snapshot~.
* Helps of the main abstractions
* Add spread value for encoding
  [t b f]
  |     |
  [f w] [min w]
  |     |
  [-    ]
