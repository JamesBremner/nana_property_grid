# nana_property_grid
A rudimentary property grid to handle properties ( name/value pairs ),

* Built on the nana C++ library.
* Maintains a vector of properties declared in the application code
* Displays a grid of property names and current values
* User clicks on property to edit the value.
* Values of properties in the application code vector automatically updated as they are edited.
* Property value types supported: string, integer, double, bool, set of optional strings, and category.
* A property of type category in the application code vector will assign following properties to the category.
