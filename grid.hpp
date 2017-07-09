#include <map>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/listbox.hpp>

namespace nana
{


/** A grid, based on the listbox */
class gridlb  : public listbox
{
public:
    /** Constructor
        @param[in] wd parent window
        @param[in] r  grid size on screen pixels
    */
    gridlb( window wd, const rectangle& r = rectangle());

    /** Resize the grid */
    void Resize( int rows, int cols );

    void ColTitle( int col, const std::string& value );

    /** Set cell value */
    void Set( int row, int col, const std::string& value );

protected:
    int myRowCount;
    int myColCount;

    /** true if row and col are included */
    bool CheckIndex( int row, int col );


};

/** Property grid for handling name/value pairs */
class property_grid : public gridlb
{
public:
    property_grid( window wd, const rectangle& r = rectangle());

    /** Add a new property */
    void Add(
        const std::string& name,
        const std::string& value );


    /** Change value of existing property */
    void Set(
        const std::string& name,
        const std::string& value );


private:
    int myPropCount;
    std::map< std::string, int > myMap;

};
}
