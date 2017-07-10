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

template <class T>
class value
{
public:

    T myValue;

    std::string AsString() const
    {
        std::stringstream ss;
        ss << myValue;
        return ss.str();
    }
    void SetValue( const std::string& sv )
    {
        myValue = sv;
    }
};

template <>
class value<int>
{
public:

    int myValue;

    std::string AsString() const
    {
        std::stringstream ss;
        ss << myValue;
        return ss.str();
    }
    void SetValue( const std::string& sv )
    {
        myValue = atoi( sv.c_str() );
    }
};

template <>
class value<double>
{
public:

    double myValue;

    std::string AsString() const
    {
        std::stringstream ss;
        ss << myValue;
        return ss.str();
    }
    void SetValue( const std::string& sv )
    {
        myValue = atof( sv.c_str() );
    }
};

template <>
class value<bool>
{
public:

    bool myValue;

    std::string AsString() const
    {
        if( myValue )
            return "true";
        return "false";
    }
    void SetValue( const std::string& sv )
    {
        myValue = ( sv == "true" );
    }
};

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

    value<T> myValue;

    property( const std::string& name,
              const T& value )
    {
        myName = name;
        myValue.myValue = value;
    }
    std::string ValueAsString() const
    {
        return myValue.AsString();
    }

    void SetValue( const std::string& sv )
    {
        myValue.SetValue( sv );
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


private:

    /// map property names to indices
    std::map< std::string, int > myMap;

    /// pointer to external property vector
    vector_t * myVP;

};
}
}
