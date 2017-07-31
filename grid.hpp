#pragma once
#include <map>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include "properties.hpp"

namespace nana
{


/** A grid, based on the listbox */

class grid  : public listbox
{
public:
    /** Constructor
        @param[in] wd parent window
        @param[in] r  grid size on screen pixels
    */
    grid( window wd, const rectangle& r = rectangle());

    /** Resize the grid */
    void Resize( int rows, int cols );

    /** Title the column
        @param[in] col zero-based column index
        @param[in] value the column title
    */
    void ColTitle( int col, const std::string& value );

    /** Specify column width
     @param[in] col zero-based column index
     @param[in] width in pixels
    */
    void ColWidth( int col, int width );

    /** Set cell value */
    void Set( int row, int col, const std::string& value );

protected:
    int myRowCount;
    int myColCount;

    /** true if row and col are included */
    bool CheckIndex( int row, int col );


};

namespace prop
{


/** vector of pointers to properties */
typedef std::shared_ptr< property_base > prop_t;
typedef std::vector< prop_t > vector_t;

/** Property grid for handling name/value pairs */

class grid : public nana::grid
{
public:
    grid( window wd, const rectangle& r = rectangle());

    /** Add properties vector vector
        @param[in] v vector of properties

        The properties in this vector will be used to initialize the grid.
        When user edits property values in the grid
        the property values in this vector will be updated.

        A pointer to the vector is stored, so the calling code
        must ensure that the vector does not go out of scope
        before the grid does
     */
    void Set( vector_t& v );

    void Set( property_container& pc )
    {
        Set( pc.get() );
    }

    /** Collapse or expand a category

    @param[in] category_name
    @param[in] fCollapse true for collapse, false for expand, default is true
    */
    void Collapse(
        const std::string& category_name,
        bool fCollapse = true );

    /** Change value of existing property */
//    void Set(
//        const std::string& name,
//        const std::string& value );

//    template <class T>
//    void Set( const property<T>& prop )
//    {
//        Set( prop.myName, prop.ValueAsString() );
//    }


private:

    /// pointer to external property vector
    vector_t * myVP;

};
}
}
