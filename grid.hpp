#pragma once
#include <map>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/listbox.hpp>

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

    void ColTitle( int col, const std::string& value );

    /** Set cell value */
    void Set( int row, int col, const std::string& value );

protected:
    int myRowCount;
    int myColCount;

    /** true if row and col are included */
    bool CheckIndex( int row, int col );


};

namespace prop {

/** Property base class

This non-templated base class allows pointers to properties of any type
to be stored in a vector, avoiding the problem of 'type slicing'
cf https://stackoverflow.com/a/16527721/16582
*/

class property_base
{
public:
    std::string myName;
    virtual std::string ValueAsString() const = 0;
    virtual void SetValue( const std::string& sv ) = 0;
};

/** Property -  a name value pair with templated value type */

template <class T>
class property : public property_base
{
public:

    T myValue;

    property( const std::string& name,
              const T& value )
    {
        myName = name;
        myValue = value;
    }
    std::string ValueAsString() const
    {
        std::stringstream ss;
        ss.str("");
        ss << myValue;
        return ss.str();
    }

    void SetValue( const std::string& sv )
    {
        myValue = sv;
    }

};

/** Property template specialisation for integers */

template <>
class property<int> : public property_base
{
public:

    int myValue;

    property( const std::string& name,
              const int& value )
    {
        myName = name;
        myValue = value;
    }
    std::string ValueAsString() const
    {
        std::stringstream ss;
        ss.str("");
        ss << myValue;
        return ss.str();
    }

    void SetValue( const std::string& sv )
    {
        myValue = atoi( sv.c_str() );
    }

};

/** vector of pointers to properties */
typedef std::vector< std::shared_ptr< property_base > > vector_t;

/** Property grid for handling name/value pairs */

class grid : public nana::grid
{
public:
    grid( window wd, const rectangle& r = rectangle());

    /** Add properties from vector */
    void Add( vector_t& v );


    /** Change value of existing property */
    void Set(
        const std::string& name,
        const std::string& value );

    template <class T>
    void Set( const property<T>& prop )
    {
        Set( prop.myName, prop.ValueAsString() );
    }

    /** Return all properties in a vector of string pairs */
    std::vector< std::pair< std::string, std::string > >
    AllProps();


private:
    int myPropCount;
    std::map< std::string, int > myMap;
    vector_t * myVP;

};
}
}
